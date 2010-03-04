/*
 * rules_io.cpp
 *
 *
 *      Author: snowwlex
 */

#include <cstdio>
#include <expat.h>
#include <string>
#include <vector>
#include <map>

#include "snowwchess.h"
#include "rules.h"
#include "io.h"
#include "rules_io.h"

RulesIO::RulesIO(Rules *_rules): myRules(_rules) { }


void XMLCALL RulesIOstartElementHandler(void *userData, const char *name, const char **atts) {
	RulesIOXMLStorage *storage = (RulesIOXMLStorage *)userData;
	int i;

	std::string tag = name, attr, value;
	if (tag == "rules") {
		 storage->rules_name = "";
		 for (i = 0; atts[i]; i += 2)  {
			attr = atts[i];	value = atts[i+1];
			if (attr == "name") {  storage->rules_name = value; }
		 }
	} else if (tag == "board") {
		storage->boardsize_x = storage->boardsize_y = 0;
		for (i = 0; atts[i]; i += 2)  {
			attr = atts[i]; value = atts[i+1];
			if (attr == "sizex") {  storage->boardsize_x = makeInt(value); }
			if (attr == "sizey") {  storage->boardsize_y = makeInt(value); }
		}
	} else if (tag == "players") {
		storage->section = "players";
		int id = 0;
		for (i = 0; atts[i]; i += 2)  {
			attr = atts[i]; value = atts[i+1];
			if (attr == "turns") {  id = makeInt(value); }
			storage->first_turn = id-1;
		}
	} else if (tag == "figures") {
		storage->section = "figures";
		int id = 0;
		for (i = 0; atts[i]; i += 2)  {
			attr = atts[i]; value = atts[i+1];
			if (attr == "special") {  id = makeInt(value); }
			storage->special_figure = id;
		}
	} else if (tag == "positions") {
		storage->section = "positions";
	} else if (tag == "moves") {
		storage->section = "moves";
	} else if (storage->section != "") {
		if (storage->section == "players") {
			if (tag == "player") {
				int id = 0;
				std::string name;
				for (i = 0; atts[i]; i += 2)  {
					attr = atts[i]; value = atts[i+1];
					if (attr == "id") {  id = makeInt(value); }
					if (attr == "name") { name  = value; }
					storage->PlayersData[id] = name;
				}
			}
		}else if (storage->section == "figures") {
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
					storage->FiguresData[id] = figure_data;
				}

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
					if (attr == "cell") {  figure.position.x = value[0]-'a'; figure.position.y = storage->boardsize_y - value[1] + '0' ; }
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
					if (attr == "dx") {  moverule.delta_x = makeInt(value); }
					if (attr == "dy") {  moverule.delta_y = makeInt(value); }
					if (attr == "type") {  moverule.move_type = (value == "move" ? MOVE : EAT); }
					if (attr == "player") { moverule.player = (value == "1" ? WHITE:BLACK); }
				}
				storage->tmp_move_rule.push_back(moverule);
			}
		}
	}
}

void XMLCALL RulesIOendElementHandler(void *userData, const char *name) {
	RulesIOXMLStorage *storage = (RulesIOXMLStorage *)userData;
	if (storage->section == "moves") {
		if (std::string(name) == "figure") {
			if (storage->tmp_move_rule.size() > 0) {
						storage->myMoveRulesIO[storage->cur_figure_id] = storage->tmp_move_rule;
			}
		}
	}
}

const RulesIOXMLStorage& RulesIO::getStorage() const {
	return myStorage;
}

void RulesIO::UpdateRules() {
	myRules->SetRulesName(myStorage.rules_name);
	myRules->SetFirstTurn(myStorage.first_turn);
	myRules->SetSpecialFigure(myStorage.special_figure);
	myRules->SetBoardSize(myStorage.boardsize_x, myStorage.boardsize_y);
	myRules->SetInitFigures(WHITE, myStorage.SetFigures[WHITE]);
	myRules->SetInitFigures(BLACK, myStorage.SetFigures[BLACK]);
	for (std::map < int , FigureData >::iterator it=myStorage.FiguresData.begin(); it!=myStorage.FiguresData.end(); ++it) {
		myRules->SetFigureData(it->first, it->second);
	}
	for (std::map < int , std::string >::iterator it=myStorage.PlayersData.begin(); it!=myStorage.PlayersData.end(); ++it) {
		myRules->SetPlayerData(it->first, it->second);
	}
	for (std::map< int, std::vector<MoveRule> >::iterator it=myStorage.myMoveRulesIO.begin(); it!=myStorage.myMoveRulesIO.end(); ++it) {
		myRules->SetMoveRule(it->first, it->second);
	}
}

void RulesIO::Load(std::string file) {

	FILE *infile = fopen(file.c_str(), "rt");

	int done, length;
	char buffer[1024];
	myStorage.section = "";
	XML_Parser parser = XML_ParserCreate(NULL);
	XML_SetElementHandler(parser, RulesIOstartElementHandler, RulesIOendElementHandler);
	XML_SetUserData(parser, &myStorage);
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

}


