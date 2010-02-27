/*
 * io.cpp
 *
 *
 *      Author: snowwlex
 */

#include <cstdio>
#include <ncurses.h>
#include <expat.h>
#include <string>
#include <vector>
#include <map>

#include "rules.h"
#include "model.h"
#include "io.h"

RulesIO::RulesIO(Rules *_rules): rules(_rules) { }
ModelIO::ModelIO(Model *_model): model(_model) { }


void XMLCALL RulesIOstartElementHandler(void *userData, const char *name, const char **atts) {
	XMLStorage *storage = (XMLStorage *)userData;
	int i;

	std::string tag = name, attr, value;
	if (tag == "rules") {
		 storage->rules_name = "classic";
		 for (i = 0; atts[i]; i += 2)  {
			attr = atts[i];	value = atts[i+1];
			if (attr == "name") {  storage->rules_name = value; }
		 }
	} else if (tag == "board") {
		storage->boardsize_x = storage->boardsize_y = 8;
		for (i = 0; atts[i]; i += 2)  {
			attr = atts[i]; value = atts[i+1];
			if (attr == "sizex") {  storage->boardsize_x = makeInt(value); }
			if (attr == "sizey") {  storage->boardsize_y = makeInt(value); }
		}
	} else if (tag == "players") {
		storage->section = "players";
	} else if (tag == "figures") {
		storage->section = "figures";
	} else if (tag == "positions") {
		storage->section = "positions";
	} else if (tag == "moves") {
		storage->section = "moves";
	} else if (storage->section != ""){
		if (storage->section == "figures") {
			if (tag == "figure") {
				FigureData figure_data;
				int id = 0;
				figure_data.special = false;
				for (i = 0; atts[i]; i += 2)  {
					attr = atts[i]; value = atts[i+1];
					if (attr == "id") {  id = makeInt(value); }
					if (attr == "name") {  figure_data.name = value; }
					if (attr == "letter") {  figure_data.letter = value[0]; }
					if (attr == "special") {  figure_data.special = true; }
				}
				storage->FiguresData[id] = figure_data;
			}
		} else if (storage->section == "positions") {
			if (tag == "player") {
				int id = 0;
				for (i = 0; atts[i]; i += 2)  {
					attr = atts[i]; value = atts[i+1];
					if (attr == "id") {  id = makeInt(value); }
				}
				storage->cur_player_id = id-1;
			} else	if (tag == "figure") {
				int id = 0;
				for (i = 0; atts[i]; i += 2)  {
					attr = atts[i]; value = atts[i+1];
					if (attr == "id") {  id = makeInt(value); }
				}
				storage->cur_figure_id = id;
			} else if (tag == "position") {
				Figure figure;
				figure.id = storage->cur_figure_id;
				figure.position.x = 0;
				figure.position.y = 0;
				for (i = 0; atts[i]; i += 2)  {
					attr = atts[i]; value = atts[i+1];
					if (attr == "x") {  figure.position.x = makeInt(value); }
					if (attr == "y") {  figure.position.y = makeInt(value); }
				}
				storage->SetFigures[storage->cur_player_id].push_back(figure);
			}
		}  else if (storage->section == "moves") {
			if (tag == "figure") {
				int id = 0;
				storage->tmp_move_rule.clear();
				for (i = 0; atts[i]; i += 2)  {
					attr = atts[i]; value = atts[i+1];
					if (attr == "id") {  id = makeInt(value); }
				}
				storage->cur_figure_id = id;
			} else	if (tag == "jump" || tag == "direction") {
				MoveRule moverule(0,0);
				moverule.rule_type = (tag=="jump"?JUMP:DIRECTION);
				for (i = 0; atts[i]; i += 2)  {
					attr = atts[i]; value = atts[i+1];
					if (attr == "x") {  moverule.delta_x = makeInt(value); }
					if (attr == "y") {  moverule.delta_y = makeInt(value); }
					if (attr == "type") {  moverule.move_type = (value == "move" ? MOVE : EAT); }
					if (attr == "player") { moverule.player = (value == "1" ? WHITE:BLACK); }
				}
				storage->tmp_move_rule.push_back(moverule);
			}
		}
	}
}

void XMLCALL RulesIOendElementHandler(void *userData, const char *name) {
	XMLStorage *storage = (XMLStorage *)userData;
	if (storage->section == "moves") {
		if (std::string(name) == "figure") {
			if (storage->tmp_move_rule.size() > 0) {
						storage->myMoveRulesIO[storage->cur_figure_id] = storage->tmp_move_rule;
			}
		}
	}
}


int makeInt(std::string value) {
	char* end_ptr;
	long int val = strtol(value.c_str(), &end_ptr, 10);
	if (*end_ptr) { end_ptr++; }
	return (int)val;

}

void ModelIO::Load(std::string file) {
	model->SetInitFigures(WHITE);
	model->SetInitFigures(BLACK);
}

void RulesIO::Load() {

	FILE *infile = fopen("rules/classic.xml", "r");

	int done, length;
	char buffer[1024];
	XMLStorage storage;
	storage.section = "";
	XML_Parser parser = XML_ParserCreate(NULL);
	XML_SetElementHandler(parser, RulesIOstartElementHandler, RulesIOendElementHandler);
	XML_SetUserData(parser, &storage);

	do  {
		length = fread(buffer, 1, sizeof(buffer), infile);
		done = feof(infile);
		if (XML_Parse(parser, buffer, length, done) == XML_STATUS_ERROR) {
			//wprintw(rules->myWindow, "Error: %s at line %d\n", XML_ErrorString(XML_GetErrorCode(parser)),  (int)XML_GetCurrentLineNumber(parser));
			//wrefresh(rules->myWindow);
			//wgetch(rules->myWindow);
     		done = 1;
	    }
	} while (!done);


	XML_ParserFree(parser);
	fclose(infile);

	rules->SetInitFigures(WHITE, storage.SetFigures[WHITE]);
	rules->SetInitFigures(BLACK, storage.SetFigures[BLACK]);

	for (std::map < int , FigureData >::iterator it=storage.FiguresData.begin(); it!=storage.FiguresData.end(); ++it) {
		rules->SetFiguresData(it->first, it->second);
	}

	for (std::map< int, std::vector<MoveRule> >::iterator it=storage.myMoveRulesIO.begin(); it!=storage.myMoveRulesIO.end(); ++it) {
		rules->SetMoveRule(it->first, it->second);
	}

}


