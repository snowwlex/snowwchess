#include "rules_io.h"

XmlRulesParser::XmlRulesParser(RulesIOXMLStorage* storage): myStorage(storage) { }

bool XmlRulesParser::startElement(const QString&, const QString&,const QString& tag, const QXmlAttributes& attrs) {
	if (tag == "rules") {
		 rulesTag(tag,attrs);
	} else if (tag == "board") {
		boardTag(tag, attrs);
	} else if (tag == "players") {
		myStorage->section = "players";
		playersTag(tag,attrs);
	} else if (tag == "figures") {
		myStorage->section = "figures";
	} else if (tag == "positions") {
		myStorage->section = "positions";
	} else if (tag == "moves") {
		myStorage->section = "moves";
	} else if (myStorage->section != "") {
		if (myStorage->section == "players") {
			sectionPlayers(tag, attrs);
		}else if (myStorage->section == "figures") {
			sectionFigures(tag,attrs);
		} else if (myStorage->section == "positions") {
			sectionPositions(tag,attrs);
		}  else if (myStorage->section == "moves") {
			sectionMoves(tag,attrs);
		}
	}
	return true;
}

void XmlRulesParser::playersTag(const QString& , const QXmlAttributes& attrs) {
	QString attr, value;
	int id = 0;
	for(int i = 0; i < attrs.count(); ++i) {
		attr = attrs.localName(i); value = attrs.value(i);
		if(attr == "turns") { id = value.toInt(); }
		myStorage->firstTurn = id-1;
	}
}

void XmlRulesParser::rulesTag(const QString& , const QXmlAttributes& attrs) {
	QString attr, value;
	myStorage->rulesName = "";
	for(int i = 0; i < attrs.count(); ++i) {
		attr = attrs.localName(i); value = attrs.value(i);
		if(attr == "name") { myStorage->rulesName = value.toStdString(); }
	}
}

void XmlRulesParser::boardTag(const QString& , const QXmlAttributes& attrs) {
	QString attr, value;
	myStorage->boardSizeX = myStorage->boardSizeY = 0;
	for(int i = 0; i < attrs.count(); ++i) {
		attr = attrs.localName(i); value = attrs.value(i);
		if(attr == "sizex") { myStorage->boardSizeX = value.toInt(); }
		if(attr == "sizey") { myStorage->boardSizeY = value.toInt(); }
	 }
}
void XmlRulesParser::sectionPlayers(const QString& tag, const QXmlAttributes& attrs) {
	if (tag == "player") {
		sectionPlayersTagPlayer(tag,attrs);
	}
}

void XmlRulesParser::sectionPlayersTagPlayer(const QString& , const QXmlAttributes& attrs) {
	QString attr, value;
	int id = 0;
	std::string name;
	for(int i = 0; i < attrs.count(); ++i) {
		attr = attrs.localName(i); value = attrs.value(i);
		if (attr == "id") {  id = value.toInt(); }
		if (attr == "name") { name  = value.toStdString(); }
		myStorage->playersData[id-1] = name;
	}
}
void XmlRulesParser::sectionFigures(const QString& tag, const QXmlAttributes& attrs)  {
	if (tag == "figure") {
		sectionFiguresTagFigure(tag,attrs);
	}
}


void XmlRulesParser::sectionFiguresTagFigure(const QString& , const QXmlAttributes& attrs) {
	QString attr, value;
	FigureData figureData;
	int id = 0;
	for(int i = 0; i < attrs.count(); ++i) {
		attr = attrs.localName(i); value = attrs.value(i);
		if (attr == "id") {  id = value.toInt(); }
		if (attr == "name") {  figureData.name = value.toStdString(); }
		if (attr == "letter") {  figureData.letter = value[0].toAscii(); }
		if (attr == "explosion") {  if (value == "0") figureData.explosion = false; }
		if (attr == "weight") {  figureData.weight = value.toInt();  }
		if (attr == "wpicture") {  figureData.picture[WHITE] = value.toStdString();  }
		if (attr == "bpicture") {  figureData.picture[BLACK] = value.toStdString();  }
	}
	myStorage->figuresData[id] = figureData;
}

void XmlRulesParser::sectionPositions(const QString& tag, const QXmlAttributes& attrs) {
	if (tag == "player") {
		sectionPositionsTagPlayer(tag, attrs);
	} else	if (tag == "figure") {
		sectionPositionsTagFigure(tag,attrs);
	} else if (tag == "position") {
		sectionPositionsTagPosition(tag,attrs);
	}

}

void XmlRulesParser::sectionPositionsTagPlayer(const QString& , const QXmlAttributes& attrs) {
	QString attr, value;
	int id = 0;
	for(int i = 0; i < attrs.count(); ++i) {
		attr = attrs.localName(i); value = attrs.value(i);
		if (attr == "id") {  id = value.toInt(); }
	}
	myStorage->curPlayerId = id-1;
}

void XmlRulesParser::sectionPositionsTagFigure(const QString& , const QXmlAttributes& attrs) {
	QString attr, value;
	int id = 0;
	for(int i = 0; i < attrs.count(); ++i) {
		attr = attrs.localName(i); value = attrs.value(i);
		if (attr == "id") {  id = value.toInt(); }
		if (attr == "special") {  myStorage->specialFigureId[myStorage->curPlayerId] = id; }
	}
	myStorage->curFigureId = id;
}


void XmlRulesParser::sectionPositionsTagPosition(const QString& , const QXmlAttributes& attrs) {
	QString attr, value;
	RulesIOXMLStorage::FigureInfo figure_info;
	figure_info.id = myStorage->curFigureId;
	for(int i = 0; i < attrs.count(); ++i) {
		attr = attrs.localName(i); value = attrs.value(i);
		if (attr == "cell") {  figure_info.cell = value.toStdString(); }
	}
	myStorage->setFiguresInfo[myStorage->curPlayerId].push_back(figure_info);
}

void XmlRulesParser::sectionMoves(const QString& tag, const QXmlAttributes& attrs) {
	if (tag == "figure") {
		sectionMovesTagFigure(tag,attrs);
	} else	if (tag == "jump" || tag == "slide") {
		sectionMovesTagJumpSlide(tag,attrs);
	} else if (tag == "promotion") {
		sectionMovesTagPromotion(tag,attrs);
	}
}


void XmlRulesParser::sectionMovesTagFigure(const QString& , const QXmlAttributes& attrs) {
	QString attr, value;
	int id = 0;
	myStorage->tmpMoveRule.clear();
	for(int i = 0; i < attrs.count(); ++i) {
		attr = attrs.localName(i); value = attrs.value(i);
		if (attr == "id") {  id = value.toInt(); }
	}
	myStorage->curFigureId = id;
}


void XmlRulesParser::sectionMovesTagJumpSlide(const QString& tag, const QXmlAttributes& attrs) {
	QString attr, value;
	MoveRule moveRule;
	RulesIOXMLStorage::CastleRuleInfo castleRule;

	moveRule.moveType = MOVE | CAPTURE;
	moveRule.player = ALL;
	moveRule.ruleType = (tag=="jump"?JUMP:SLIDE);
	for(int i = 0; i < attrs.count(); ++i) {
		attr = attrs.localName(i); value = attrs.value(i);
		if (attr == "dx") {  moveRule.dx = value.toInt(); }
		if (attr == "dy") {  moveRule.dy = value.toInt(); }
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
		if (attr == "limit") { moveRule.limit = value.toInt(); }
		if (attr == "on") { castleRule.kingCell=value.toStdString(); }
		if (attr == "with") { castleRule.rookCellStart=value.toStdString(); }
		if (attr == "to") {castleRule.rookCellEnd=value.toStdString();  }
	}
	myStorage->tmpMoveRule.push_back(moveRule);

	if (moveRule.moveEffect == CASTLE) {
		castleRule.dx = moveRule.dx;
		castleRule.dy = moveRule.dy;
		castleRule.player = moveRule.player;
		myStorage->castleRules.push_back(castleRule);
	}
}


void XmlRulesParser::sectionMovesTagPromotion(const QString& , const QXmlAttributes& attrs) {
	QString attr, value;
	RulesIOXMLStorage::PromotionInfo promotionInfo;
	promotionInfo.player = ALL;
	for(int i = 0; i < attrs.count(); ++i) {
		attr = attrs.localName(i); value = attrs.value(i);
		if (attr == "horizontal") {  promotionInfo.horizontal = value.toInt(); }
		if (attr == "figure") {  promotionInfo.promotionFigure = value.toInt(); }
		if (attr == "player") { promotionInfo.player = (value == "1" ? WHITE:BLACK); }
	}
	promotionInfo.figure = myStorage->curFigureId;
	myStorage->promotionData.push_back(promotionInfo);
}


bool XmlRulesParser::endElement(const QString&, const QString&, const QString& tagName) {
	if (myStorage->section == "moves") {
		if (tagName == "figure") {
			if (myStorage->tmpMoveRule.size() > 0) {
				myStorage->myMoveRulesIO[myStorage->curFigureId] = myStorage->tmpMoveRule;
			}
		}
	}
	return true;
}

bool XmlRulesParser::fatalError(const QXmlParseException& exception) {
	qDebug() << "Error of Rules file parsing!"
			 << "Line:"      << exception.lineNumber()
			 << ", Column:"  << exception.columnNumber()
			 << ", Message:" << exception.message();
	return false;
}


RulesIO::RulesIO() { }

const RulesIOXMLStorage& RulesIO::getStorage() const {
	return myStorage;
}


void RulesIO::updateInitFigures(Rules& rules) const {
	RulesIOXMLStorage::FIGURES_INFO::const_iterator it;
	Figure tmpFigure;
	for (int i=0; i<2; ++i) {
		for ( it=myStorage.setFiguresInfo[i].begin(); it != myStorage.setFiguresInfo[i].end(); ++it ) {
			tmpFigure.id = it->id;
			tmpFigure.position.myX = it->cell[0]-'a';

			tmpFigure.position.myY = myStorage.boardSizeY - QString( it->cell.substr(1,2).c_str() ).toInt();
			tmpFigure.wasMoved = false;
			rules.setInitFigure(i,tmpFigure);
		}
	}
}

void RulesIO::updateCastleRules(Rules& rules) const {
	for (RulesIOXMLStorage::CASTLERULES_INFO::const_iterator it=myStorage.castleRules.begin(); it != myStorage.castleRules.end(); ++it) {
		CastleRule castleRule;
		castleRule.dx=it->dx;
		castleRule.dy=it->dy;
		castleRule.player=it->player;
		castleRule.kingCell.myX = it->kingCell[0]-'a';
		castleRule.kingCell.myY = myStorage.boardSizeY - QString( it->kingCell.substr(1,2).c_str() ).toInt();
		castleRule.rookCellEnd.myX=it->rookCellEnd[0]-'a';
		castleRule.rookCellEnd.myY= myStorage.boardSizeY - QString( it->rookCellEnd.substr(1,2).c_str() ).toInt();
		castleRule.rookCellStart.myX=it->rookCellStart[0]-'a';
		castleRule.rookCellStart.myY= myStorage.boardSizeY - QString( it->rookCellStart.substr(1,2).c_str() ).toInt();
		rules.setCastleRule(castleRule);
	}
}

void RulesIO::setPromotionRules() {
	for (RulesIOXMLStorage::PROMOTIONS_INFO::const_iterator itPromotion = myStorage.promotionData.begin();itPromotion !=  myStorage.promotionData.end(); ++itPromotion) {
		if (itPromotion->player != ALL) {
			myStorage.figuresData[itPromotion->figure].promoting[itPromotion->player].figure = itPromotion->promotionFigure;
			myStorage.figuresData[itPromotion->figure].promoting[itPromotion->player].horizontal = myStorage.boardSizeY - itPromotion->horizontal;
		} else {
			for (int i=0; i<2; ++i) {
				myStorage.figuresData[itPromotion->figure].promoting[i].figure = itPromotion->promotionFigure;
				myStorage.figuresData[itPromotion->figure].promoting[i].horizontal = myStorage.boardSizeY - itPromotion->horizontal;
			}
		}
	}
}


void RulesIO::updateFiguresData(Rules& rules) const {
	for (FIGURES_DATA::const_iterator it=myStorage.figuresData.begin(); it!=myStorage.figuresData.end(); ++it) {
		rules.setFigureData(it->first, it->second);
	}
}

void RulesIO::updatePlayersData(Rules& rules) const {
	for (PLAYERS_DATA::const_iterator it=myStorage.playersData.begin(); it!=myStorage.playersData.end(); ++it) {
		rules.setPlayerData(it->first, it->second);
	}
}

void RulesIO::updateMoveRules(Rules& rules) const {
	for (FIGURES_RULES::const_iterator itMap=myStorage.myMoveRulesIO.begin(); itMap!=myStorage.myMoveRulesIO.end(); ++itMap) {
		for (MOVERULES::const_iterator itVector = itMap->second.begin(); itVector!=itMap->second.end(); ++itVector) {
			rules.setMoveRule(itMap->first,*itVector);
		}

	}
}

void RulesIO::updateRules(Rules& rules) const {
	rules.setRulesName(myStorage.rulesName);
	rules.setFirstTurnPlayer(myStorage.firstTurn);
	rules.setSpecialFigure(WHITE, myStorage.specialFigureId[WHITE]);
	rules.setSpecialFigure(BLACK, myStorage.specialFigureId[BLACK]);
	rules.setBoardSize(myStorage.boardSizeX, myStorage.boardSizeY);

	updateInitFigures(rules);
	updateCastleRules(rules);
	updateFiguresData(rules);
	updatePlayersData(rules);
	updateMoveRules(rules);

}

bool RulesIO::load(std::string filename) {

	myStorage.section = "";
	myStorage.specialFigureId[WHITE] = myStorage.specialFigureId[BLACK] = 0;

	XmlRulesParser handler(&myStorage);
	QFile file(filename.c_str());
	QXmlInputSource   source(&file);
	QXmlSimpleReader  parser;

	parser.setContentHandler(&handler);
	bool ok = parser.parse(source);

	if (ok == true) {
		if (myStorage.specialFigureId[WHITE] == 0 || myStorage.specialFigureId[BLACK] ==0) {
			qDebug() << "Special figures are not setted";
		}

		//after parsing processing:
		setPromotionRules(); //promotions rules add to figures data
	}

	return ok;

}


