/*
 * model_io.cpp
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
#include "model.h"
#include "io.h"
#include "model_io.h"

ModelIO::ModelIO(Model *model): myModel(model) { }

void modelIOStartElementHandler(void *userData, const char *name, const char **atts) {
	ModelIOXMLStorage *storage = (ModelIOXMLStorage *)userData;
	int i;

	std::string tag = name, attr, value;
	if (tag == "positions") {
		 storage->rulesName = "";
		 for (i = 0; atts[i]; i += 2)  {
			attr = atts[i];	value = atts[i+1];
			if (attr == "rules") {  storage->rulesName = value; }
			if (attr == "turn") {  storage->firstTurn = makeInt(value)-1; }
		 }
	} else if (tag == "player") {
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
		ModelIOXMLStorage::FigureInfo figure_info;
		figure_info.id = storage->curFigureId;
		figure_info.wasMoved = false;
		figure_info.captured = false;
		for (i = 0; atts[i]; i += 2)  {
			attr = atts[i]; value = atts[i+1];
			if (attr == "cell") {  figure_info.cell = value; }
			if (attr == "unmoved") { figure_info.wasMoved = false; }
			if (attr == "captured") { figure_info.captured = true; }
		}
		storage->setFiguresInfo[storage->curPlayerId].push_back(figure_info);
	}
}

void modelIOEndElementHandler(void *userData, const char *name) {
	return;
}

const ModelIOXMLStorage& ModelIO::getStorage() const {
	return myStorage;
}

void ModelIO::updateModel() {
	int boardSizeY = myModel->getBoardSizeY();

	ModelIOXMLStorage::FIGURES_INFO::iterator it;
	Figure tmpFigure;

	myModel->setCurrentPlayer(myStorage.firstTurn);

	for (int i=0; i<2; ++i) {
		for ( it=myStorage.setFiguresInfo[i].begin(); it != myStorage.setFiguresInfo[i].end(); ++it ) {
			tmpFigure.id = it->id;
			tmpFigure.position.myX = it->cell[0]-'a';
			tmpFigure.position.myY = boardSizeY - makeInt(it->cell.substr(1,2));
			tmpFigure.wasMoved = it->wasMoved;
			tmpFigure.captured = it->captured;
			myModel->setFigure(i, tmpFigure);
		}
	}


}

void ModelIO::load(std::string file) {

	FILE *infile = fopen(file.c_str(), "rt");
	int done, length;
	char buffer[1024];
	XML_Parser parser = XML_ParserCreate(NULL);
	XML_SetElementHandler(parser, modelIOStartElementHandler, modelIOEndElementHandler);
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

void ModelIO::save(std::string file) {

	//ISSUE: add save last move

	FILE *outfile = fopen(file.c_str(), "wt");
	char posX, posY, player;
	int depth,i, curFigureId;
	const char TAB = '\t';
	std::string bufferStr;
	fputs("<?xml version='1.0' encoding='UTF-8'?>\n", outfile);
	player = myModel->getCurrentPlayer() + 1 + '0';
	bufferStr = "<positions rules='"+myModel->getRulesName()+"' turn='"+player+"'>\n";
	fputs(bufferStr.c_str(),outfile);

	FIGURES::const_iterator it;
	for (i=0, depth=1; i<2; ++i) {
		player = i+1+'0';
		bufferStr = std::string(depth,TAB) + "<player id='"+player+"'>\n";
		fputs(bufferStr.c_str(),outfile);
		++depth;
		for (curFigureId = 0, it=myModel->getSetFigures(i).begin(); it != myModel->getSetFigures(i).end(); ++it ) {
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
			posY = myModel->getBoardSizeY() - it->position.myY+'0';
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


