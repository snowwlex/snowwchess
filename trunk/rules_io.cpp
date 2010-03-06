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
				RulesIOXMLStorage::FigureInfo figure_info;
				figure_info.id = storage->cur_figure_id;
				for (i = 0; atts[i]; i += 2)  {
					attr = atts[i]; value = atts[i+1];
					if (attr == "cell") {  figure_info.cell = value; }
				}
				storage->SetFiguresInfo[storage->cur_player_id].push_back(figure_info);
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
					if (attr == "type") {
						moverule.move_type = 0;
						if (value == "move") moverule.move_type = MOVE;
						if (value == "eat") moverule.move_type = EAT;
						if (value == "longmove") moverule.move_type = MOVE | LONGMOVE;
						if (value == "enpassant") moverule.move_type = EAT | ENPASSANT;
					}
					if (attr == "player") { moverule.player = (value == "1" ? WHITE:BLACK); }
					if (attr == "limit") { moverule.limit = makeInt(value); }
				}
				storage->tmp_move_rule.push_back(moverule);
			} else	if (tag == "castle") {
				MoveRule moverule(0,0);
				RulesIOXMLStorage::CastleRuleInfo castlerule;
				moverule.rule_type = DIRECTION;
				for (i = 0; atts[i]; i += 2)  {
					attr = atts[i]; value = atts[i+1];
					if (attr == "dx") {  castlerule.delta_x = moverule.delta_x = makeInt(value); }
					if (attr == "dy") {  castlerule.delta_y = moverule.delta_y = makeInt(value); }
					if (attr == "player") { castlerule.player = moverule.player = (value == "1" ? WHITE:BLACK); }
					if (attr == "on") { castlerule.kingcell = value; }
					if (attr == "with") { castlerule.rookcell_start = value; }
					if (attr == "to") {  castlerule.rookcell_end = value; }
				}
				moverule.limit = 1;
				moverule.move_type = MOVE | CASTLE;
				storage->tmp_move_rule.push_back(moverule);
				storage->CastlesRules.push_back(castlerule);
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

	std::vector<RulesIOXMLStorage::FigureInfo>::iterator it;
	Figure tmp_figure;
	for (int i=0; i<2; ++i) {
		for ( it=myStorage.SetFiguresInfo[i].begin(); it != myStorage.SetFiguresInfo[i].end(); ++it ) {
			tmp_figure.id = it->id;
			tmp_figure.position.x = it->cell[0]-'a';
			tmp_figure.position.y = myStorage.boardsize_y - makeInt(it->cell.substr(1,2));
			tmp_figure.unmoved = true;
			myRules->SetInitFigure(i,tmp_figure);
		}
	}

	for (std::vector<RulesIOXMLStorage::CastleRuleInfo>::iterator it=myStorage.CastlesRules.begin(); it != myStorage.CastlesRules.end(); ++it) {
		CastleRule cr;
		cr.delta_x=it->delta_x;
		cr.delta_y=it->delta_y;
		cr.player=it->player;
		cr.kingcell.x = it->kingcell[0]-'a';
		tmp_figure.position.y = myStorage.boardsize_y - makeInt(it->kingcell.substr(1,2));
		cr.rookcell_end.x=it->rookcell_end[0]-'a';
		cr.rookcell_end.y= myStorage.boardsize_y - makeInt(it->rookcell_end.substr(1,2));
		cr.rookcell_start.x=it->rookcell_start[0]-'a';
		cr.rookcell_start.y= myStorage.boardsize_y - makeInt(it->rookcell_start.substr(1,2));
		myRules->SetCastleRule(cr);
	}

	for (std::map < int , FigureData >::iterator it=myStorage.FiguresData.begin(); it!=myStorage.FiguresData.end(); ++it) {
		myRules->SetFigureData(it->first, it->second);
	}
	for (std::map < int , std::string >::iterator it=myStorage.PlayersData.begin(); it!=myStorage.PlayersData.end(); ++it) {
		myRules->SetPlayerData(it->first, it->second);
	}
	for (std::map< int, std::vector<MoveRule> >::iterator itMap=myStorage.myMoveRulesIO.begin(); itMap!=myStorage.myMoveRulesIO.end(); ++itMap) {
		for (std::vector<MoveRule>::iterator itVector = itMap->second.begin(); itVector!=itMap->second.end(); ++itVector) {
			myRules->SetMoveRule(itMap->first,*itVector);
		}

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


