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

ModelIO::ModelIO(Model *_model): myModel(_model) { }

void XMLCALL ModelIOstartElementHandler(void *userData, const char *name, const char **atts) {
	ModelIOXMLStorage *storage = (ModelIOXMLStorage *)userData;
	int i;

	std::string tag = name, attr, value;
	if (tag == "positions") {
		 storage->rules_name = "";
		 for (i = 0; atts[i]; i += 2)  {
			attr = atts[i];	value = atts[i+1];
			if (attr == "rules") {  storage->rules_name = value; }
			if (attr == "turn") {  storage->first_turn = makeInt(value)-1; }
		 }
	} else if (tag == "player") {
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
		ModelIOXMLStorage::FigureInfo figure_info;
		figure_info.id = storage->cur_figure_id;
		figure_info.unmoved = false;
		for (i = 0; atts[i]; i += 2)  {
			attr = atts[i]; value = atts[i+1];
			if (attr == "cell") {  figure_info.cell = value; }
			if (attr == "unmoved") { figure_info.unmoved = true; }
		}
		storage->SetFiguresInfo[storage->cur_player_id].push_back(figure_info);
	}
}

void XMLCALL ModelIOendElementHandler(void *userData, const char *name) {
	return;
}

const ModelIOXMLStorage& ModelIO::getStorage() const {
	return myStorage;
}

void ModelIO::UpdateModel() {
	int boardsize_y = myModel->GetBoardSizeY();

	std::vector<ModelIOXMLStorage::FigureInfo>::iterator it;
	Figure tmp_figure;

	myModel->SetCurrentPlayer(myStorage.first_turn);

	for (int i=0; i<2; ++i) {
		for ( it=myStorage.SetFiguresInfo[i].begin(); it != myStorage.SetFiguresInfo[i].end(); ++it ) {
			tmp_figure.id = it->id;
			tmp_figure.position.x = it->cell[0]-'a';
			tmp_figure.position.y = boardsize_y - makeInt(it->cell.substr(1,2));
			tmp_figure.unmoved = it->unmoved;
			myModel->SetFigure(i, tmp_figure);
		}
	}


}

void ModelIO::Load(std::string file) {

	FILE *infile = fopen(file.c_str(), "rt");
	int done, length;
	char buffer[1024];
	XML_Parser parser = XML_ParserCreate(NULL);
	XML_SetElementHandler(parser, ModelIOstartElementHandler, ModelIOendElementHandler);
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

void ModelIO::Save(std::string file) {
	FILE *outfile = fopen(file.c_str(), "wt");
	char posX, posY, player;
	int depth,i, cur_figure_id;
	const char TAB = '\t';
	std::string buffer_str;
	fputs("<?xml version='1.0' encoding='UTF-8'?>\n", outfile);
	player = myModel->GetCurrentPlayer() + 1 + '0';
	buffer_str = "<positions rules='"+myModel->GetRulesName()+"' turn='"+player+"'>\n";
	fputs(buffer_str.c_str(),outfile);

	std::vector<Figure>::const_iterator it;
	for (i=0, depth=1; i<2; ++i) {
		player = i+1+'0';
		buffer_str = std::string(depth,TAB) + "<player id='"+player+"'>\n";
		fputs(buffer_str.c_str(),outfile);
		++depth;
		for (cur_figure_id = 0, it=myModel->getSetFigures(i).begin(); it != myModel->getSetFigures(i).end(); ++it ) {
			if (cur_figure_id != it->id) {
				if (cur_figure_id != 0) {
					--depth;
					buffer_str = std::string(depth,TAB) + "</figure>\n";
					fputs(buffer_str.c_str(),outfile);
				}
				cur_figure_id = it->id;
				buffer_str = std::string(depth,TAB) + "<figure id='"+(char)(cur_figure_id+'0')+"'>\n";
				fputs(buffer_str.c_str(),outfile);
				++depth;
			}
			posX = it->position.x+'a';
			posY = myModel->GetBoardSizeY() - it->position.y+'0';
			buffer_str = std::string(depth,TAB) + "<position cell='"+posX + posY +"'";
			if (it->unmoved == true) buffer_str += " unmoved='1'";
			buffer_str += "/>\n";
			fputs(buffer_str.c_str(),outfile);
		}
		--depth;
		buffer_str = std::string(depth,TAB) + "</figure>\n";
		fputs(buffer_str.c_str(),outfile);
		--depth;
		buffer_str = std::string(depth,TAB) + "</player>\n";
		fputs(buffer_str.c_str(),outfile);
	}

	fclose(outfile);
}


