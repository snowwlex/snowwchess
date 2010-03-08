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

// для debug view
#include <ncurses.h>
#include "view.h"

RulesIO::RulesIO(Rules *rules): myRules(rules) { }


void XMLCALL rulesIOStartElementHandler(void *userData, const char *name, const char **atts) {
	RulesIOXMLStorage *storage = (RulesIOXMLStorage *)userData;
	int i;



	std::string tag = name, attr, value;
	if (tag == "rules") {
		 storage->rulesName = "";
		 for (i = 0; atts[i]; i += 2)  {
			attr = atts[i];	value = atts[i+1];
			if (attr == "name") {  storage->rulesName = value; }
		 }
	} else if (tag == "board") {
		storage->boardSizeX = storage->boardSizeY = 0;
		for (i = 0; atts[i]; i += 2)  {
			attr = atts[i]; value = atts[i+1];
			if (attr == "sizex") {  storage->boardSizeX = makeInt(value); }
			if (attr == "sizey") {  storage->boardSizeY = makeInt(value); }
		}
	} else if (tag == "players") {
		storage->section = "players";
		int id = 0;
		for (i = 0; atts[i]; i += 2)  {
			attr = atts[i]; value = atts[i+1];
			if (attr == "turns") {  id = makeInt(value); }
			storage->firstTurn = id-1;
		}
	} else if (tag == "figures") {
		storage->section = "figures";
		int id = 0;
		for (i = 0; atts[i]; i += 2)  {
			attr = atts[i]; value = atts[i+1];
			if (attr == "special") {  id = makeInt(value); }
			storage->specialFigure = id;
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
					storage->playersData[id] = name;
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
					if (attr == "explosion") {  if (value == "0") figure_data.explosion = false; }
					storage->figuresData[id] = figure_data;
				}

			}
		} else if (storage->section == "positions") {
			if (tag == "player") {
				int id = 0;
				for (i = 0; atts[i]; i += 2)  {
					attr = atts[i]; value = atts[i+1];
					if (attr == "id") {  id = makeInt(value); }
				}
				storage->curPlayerId = id-1;
			} else	if (tag == "figure") {
				int id = 0;
				for (i = 0; atts[i]; i += 2)  {
					attr = atts[i]; value = atts[i+1];
					if (attr == "id") {  id = makeInt(value); }
				}
				storage->curFigureId = id;
			} else if (tag == "position") {
				RulesIOXMLStorage::FigureInfo figure_info;
				figure_info.id = storage->curFigureId;
				for (i = 0; atts[i]; i += 2)  {
					attr = atts[i]; value = atts[i+1];
					if (attr == "cell") {  figure_info.cell = value; }
				}
				storage->setFiguresInfo[storage->curPlayerId].push_back(figure_info);
			}
		}  else if (storage->section == "moves") {
			if (tag == "figure") {
				int id = 0;
				storage->tmpMoveRule.clear();
				for (i = 0; atts[i]; i += 2)  {
					attr = atts[i]; value = atts[i+1];
					if (attr == "id") {  id = makeInt(value); }
				}
				storage->curFigureId = id;
			} else	if (tag == "jump" || tag == "slide") {
				MoveRule moveRule;
				RulesIOXMLStorage::CastleRuleInfo castleRule;

				moveRule.moveType = MOVE | CAPTURE;
				moveRule.player = ALL;
				moveRule.ruleType = (tag=="jump"?JUMP:SLIDE);
				for (i = 0; atts[i]; i += 2)  {
					attr = atts[i]; value = atts[i+1];
					if (attr == "dx") {  moveRule.dx = makeInt(value); }
					if (attr == "dy") {  moveRule.dy = makeInt(value); }
					if (attr == "type") {
						moveRule.moveType = 0;
						if (value == "move") moveRule.moveType = MOVE;
						if (value == "capture") moveRule.moveType = CAPTURE;
						if (value == "inpassing") moveRule.moveType = INPASSING;
					}
					if (attr == "effect") {
						if (value == "longmove") moveRule.moveEffect =  LONGMOVE;
						if (value =="castle") {  moveRule.moveEffect = CASTLE; }
						if (value =="explosion") {  moveRule.moveEffect = EXPLOSION; }
					}
					if (attr == "player") { moveRule.player = (value == "1" ? WHITE:BLACK); }
					if (attr == "limit") { moveRule.limit = makeInt(value); }
					if (attr == "on") { castleRule.kingCell=value; }
					if (attr == "with") { castleRule.rookCellStart=value; }
					if (attr == "to") {castleRule.rookCellEnd=value;  }
				}
				storage->tmpMoveRule.push_back(moveRule);

				if (moveRule.moveEffect == CASTLE) {
					castleRule.dx = moveRule.dx;
					castleRule.dy = moveRule.dy;
					castleRule.player = moveRule.player;
					storage->castleRules.push_back(castleRule);
				}
			}
		}
	}
}

void XMLCALL rulesIOEndElementHandler(void *userData, const char *name) {
	RulesIOXMLStorage *storage = (RulesIOXMLStorage *)userData;
	if (storage->section == "moves") {
		if (std::string(name) == "figure") {
			if (storage->tmpMoveRule.size() > 0) {
						storage->myMoveRulesIO[storage->curFigureId] = storage->tmpMoveRule;
			}
		}
	}
}

const RulesIOXMLStorage& RulesIO::getStorage() const {
	return myStorage;
}

void RulesIO::updateRules() {
	myRules->setRulesName(myStorage.rulesName);
	myRules->setFirstTurn(myStorage.firstTurn);
	myRules->setSpecialFigure(myStorage.specialFigure);
	myRules->setBoardSize(myStorage.boardSizeX, myStorage.boardSizeY);

	std::vector<RulesIOXMLStorage::FigureInfo>::iterator it;
	Figure tmpFigure;
	for (int i=0; i<2; ++i) {
		for ( it=myStorage.setFiguresInfo[i].begin(); it != myStorage.setFiguresInfo[i].end(); ++it ) {
			tmpFigure.id = it->id;
			tmpFigure.position.myX = it->cell[0]-'a';
			tmpFigure.position.myY = myStorage.boardSizeY - makeInt(it->cell.substr(1,2));
			tmpFigure.wasMoved = false;
			myRules->setInitFigure(i,tmpFigure);
		}
	}

	for (std::vector<RulesIOXMLStorage::CastleRuleInfo>::iterator it=myStorage.castleRules.begin(); it != myStorage.castleRules.end(); ++it) {
		CastleRule castleRule;
		castleRule.dx=it->dx;
		castleRule.dy=it->dy;
		castleRule.player=it->player;
		castleRule.kingCell.myX = it->kingCell[0]-'a';
		castleRule.kingCell.myY = myStorage.boardSizeY - makeInt(it->kingCell.substr(1,2));
		castleRule.rookCellEnd.myX=it->rookCellEnd[0]-'a';
		castleRule.rookCellEnd.myY= myStorage.boardSizeY - makeInt(it->rookCellEnd.substr(1,2));
		castleRule.rookCellStart.myX=it->rookCellStart[0]-'a';
		castleRule.rookCellStart.myY= myStorage.boardSizeY - makeInt(it->rookCellStart.substr(1,2));
		myRules->setCastleRule(castleRule);
	}

	for (std::map < int , FigureData >::iterator it=myStorage.figuresData.begin(); it!=myStorage.figuresData.end(); ++it) {
		myRules->setFigureData(it->first, it->second);
	}
	for (std::map < int , std::string >::iterator it=myStorage.playersData.begin(); it!=myStorage.playersData.end(); ++it) {
		myRules->setPlayerData(it->first, it->second);
	}
	for (std::map< int, std::vector<MoveRule> >::iterator itMap=myStorage.myMoveRulesIO.begin(); itMap!=myStorage.myMoveRulesIO.end(); ++itMap) {
		for (std::vector<MoveRule>::iterator itVector = itMap->second.begin(); itVector!=itMap->second.end(); ++itVector) {
			myRules->setMoveRule(itMap->first,*itVector);
		}

	}
}

void RulesIO::load(std::string file) {

	FILE *infile = fopen(file.c_str(), "rt");

	int done, length;
	char buffer[1024];
	myStorage.section = "";
	XML_Parser parser = XML_ParserCreate(NULL);
	XML_SetElementHandler(parser, rulesIOStartElementHandler, rulesIOEndElementHandler);
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


