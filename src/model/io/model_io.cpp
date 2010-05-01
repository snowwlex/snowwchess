#include <cassert>
#include <cstdlib> // for atoi()
#include "model_io.h"

ModelIO::ModelIO() {

}

XmlModelParser::XmlModelParser(ModelIOXMLStorage* storage) {
	myStorage = storage;
}

bool XmlModelParser::startElement(const QString&, const QString&,const QString& tag, const QXmlAttributes& attrs) {
	if (tag == "positions") {
		positionsTag(tag, attrs);
	} else if (tag == "player") {
		playerTag(tag, attrs);
	} else	if (tag == "figure") {
		figureTag(tag, attrs);
	} else if (tag == "position") {
		positionTag(tag, attrs);
	}
	return true;
}

bool XmlModelParser::endElement(const QString&, const QString&, const QString&) {
	return true;
}

bool XmlModelParser::fatalError(const QXmlParseException& exception) {
	qDebug() << "Error of Model Save file parsing!"
			 << "Line:"      << exception.lineNumber()
			 << ", Column:"  << exception.columnNumber()
			 << ", Message:" << exception.message();
	return false;
}

void XmlModelParser::positionsTag(const QString& , const QXmlAttributes& attrs) {
	QString attr, value;
	myStorage->rulesName = "";
	for(int i = 0; i < attrs.count(); ++i) {
		attr = attrs.localName(i); value = attrs.value(i);
		if (attr == "rules") {  myStorage->rulesName = value.toStdString(); }
		if (attr == "turn") {  myStorage->firstTurn = value.toInt()-1; }
	}
}
void XmlModelParser::playerTag(const QString& , const QXmlAttributes& attrs) {
	QString attr, value;
	int id = 0;
	for(int i = 0; i < attrs.count(); ++i) {
		attr = attrs.localName(i); value = attrs.value(i);
		if (attr == "id") {  id = value.toInt(); }
	}
	assert(id > 0);
	myStorage->curPlayerId = id-1;
}
void XmlModelParser::figureTag(const QString& , const QXmlAttributes& attrs) {
	QString attr, value;
	int id = 0;
	for(int i = 0; i < attrs.count(); ++i) {
		attr = attrs.localName(i); value = attrs.value(i);
		if (attr == "id") {  id = value.toInt(); }
	}
	myStorage->curFigureId = id;
}
void XmlModelParser::positionTag(const QString& , const QXmlAttributes& attrs) {
	QString attr, value;
	ModelIOXMLStorage::FigureInfo figure_info;
	figure_info.id = myStorage->curFigureId;
	figure_info.wasMoved = false;
	figure_info.captured = false;
	for (int i = 0; i < attrs.count(); ++i) {
		attr = attrs.localName(i); value = attrs.value(i);
		if (attr == "cell") {  figure_info.cell = value.toStdString(); }
		if (attr == "unmoved") { figure_info.wasMoved = false; }
		if (attr == "captured") { figure_info.captured = true; }
	}
	myStorage->setFiguresInfo[myStorage->curPlayerId].push_back(figure_info);
}


const ModelIOXMLStorage& ModelIO::getStorage() const {
	return myStorage;
}

void ModelIO::updateModel(Model& model) const {
	model.setFirstTurnPlayer(myStorage.firstTurn);
	updateFiguresInfo(model);
}


void ModelIO::updateFiguresInfo(Model& model) const {
	int boardSizeY = model.getBoardSizeY();
	ModelIOXMLStorage::FIGURES_INFO::const_iterator it;
	Figure tmpFigure;
	for (int i=0; i<2; ++i) {
		for ( it=myStorage.setFiguresInfo[i].begin(); it != myStorage.setFiguresInfo[i].end(); ++it ) {
			tmpFigure.id = it->id;
			tmpFigure.position.myX = it->cell[0]-'a';
			tmpFigure.position.myY = boardSizeY - atoi(it->cell.substr(1,2).c_str());
			tmpFigure.wasMoved = it->wasMoved;
			tmpFigure.captured = it->captured;
			model.setFigure(i, tmpFigure);
		}
	}
}

void ModelIO::load(std::string filename) {

	XmlModelParser handler(&myStorage);
	QFile file(filename.c_str());
	QXmlInputSource   source(&file);
	QXmlSimpleReader  parser;

	parser.setContentHandler(&handler);
	parser.parse(source);

}

void ModelIO::save(std::string file, const Model& model) const {

	//ISSUE: add save last move

	FILE *outfile = fopen(file.c_str(), "wt");
	char posX, posY, player;
	int depth,i, curFigureId;
	const char TAB = '\t';
	std::string bufferStr;
	fputs("<?xml version='1.0' encoding='UTF-8'?>\n", outfile);
	player = model.getFirstTurnPlayer() + 1 + '0';
	bufferStr = "<positions rules='"+model.getRulesName()+"' turn='"+player+"'>\n";
	fputs(bufferStr.c_str(),outfile);

	FIGURES::const_iterator it;
	for (i=0, depth=1; i<2; ++i) {
		player = i+1+'0';
		bufferStr = std::string(depth,TAB) + "<player id='"+player+"'>\n";
		fputs(bufferStr.c_str(),outfile);
		++depth;
		for (curFigureId = 0, it=model.getSetFigures(i).begin(); it != model.getSetFigures(i).end(); ++it ) {
			if (curFigureId != it->id) {
				if (curFigureId != 0) {
					--depth;
					bufferStr = std::string(depth,TAB) + "</figure>\n";
					fputs(bufferStr.c_str(),outfile);
				}
				curFigureId = it->id;
				bufferStr = std::string(depth,TAB) + "<figure id='"+(char)(curFigureId+'0')+"'>\n";
				fputs(bufferStr.c_str(),outfile);
				++depth;
			}

			posX = it->position.myX+'a';
			posY = model.getBoardSizeY() - it->position.myY+'0';
			bufferStr = std::string(depth,TAB) + "<position cell='"+posX + posY +"'";
			if (it->wasMoved == false) bufferStr += " unmoved='1'";
			if (it->captured == true) bufferStr += " captured='1'";
			bufferStr += "/>\n";
			fputs(bufferStr.c_str(),outfile);


		}
		--depth;
		bufferStr = std::string(depth,TAB) + "</figure>\n";
		fputs(bufferStr.c_str(),outfile);
		--depth;
		bufferStr = std::string(depth,TAB) + "</player>\n";
		fputs(bufferStr.c_str(),outfile);
	}

	fclose(outfile);
}


