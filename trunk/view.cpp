/*
 * view.cpp
 *
 *
 *      Author: snowwlex
 */

#include <ncurses.h>
#include <string>
#include <vector>
#include <map>

#include "snowwchess.h"
#include "model.h"
#include "view.h"

CLIView::CLIView(int height,int width,int y,int x, int color, bool scroll, Model *model) {
	myWindow = newwin(height,width,y,x);
	myColor = color;
	myModel = model;
	wbkgd(myWindow,COLOR_PAIR(myColor));
	scrollok(myWindow, scroll);
	wrefresh(myWindow);
}
void CLIView::render(std::string msg) {
	if (msg != "") {
		wprintw(myWindow,"%s",msg.c_str());
	}
	wrefresh(myWindow);
}
void CLIView::show() {
	wbkgd(myWindow,COLOR_PAIR(myColor));
	wrefresh(myWindow);
	refresh();
}
void CLIView::clear() {
	wclear(myWindow);
	wrefresh(myWindow);
}
void CLIView::hide() {
	wbkgd(myWindow,COLOR_PAIR(10));
	wrefresh(myWindow);
	refresh();
}
std::string CLIView::ask(std::string msg) {
	char inputBuffer[1024];
	curs_set(1);
	wprintw(myWindow, "%s",msg.c_str());
	wrefresh(myWindow);
	wscanw(myWindow,"%s", inputBuffer);
	return std::string(inputBuffer);
}
void CLIView::wait() {
	wgetch(myWindow);
}
int CLIView::getKey() {
	keypad(myWindow, TRUE);
	curs_set(0);
	noecho();
	int c = wgetch(myWindow);
	echo();
	return c;

}
CLIView::~CLIView() {
	wclear(myWindow);
	wbkgd(myWindow,COLOR_PAIR(5));
	wrefresh(myWindow);
	refresh();
	delwin(myWindow);

}

MainMenuCLIView::MainMenuCLIView(int height,int width,int y,int x, int _color, bool scroll, Model *model):
								CLIView(height,width,y,x,_color,scroll, model) {
	myHighlight = 0;
	keypad(myWindow, TRUE);
	myChoices[0] = "New Game";
	myChoices[1] = "Load Game";
	myChoices[2] = "Exit";
	nChoices = 3;
}

void MainMenuCLIView::render(std::string msg) {
	int x,y,i;
	wclear(myWindow);
	curs_set(0);
	wprintw(myWindow,"--- Main Menu ---\n");

	for(i = 0,x=2,y=2; i < nChoices; ++i){
		if(myHighlight == i)	{
			wattron(myWindow, A_REVERSE);
			mvwprintw(myWindow, y, x, "  %s  ", myChoices[i].c_str());
			wattroff(myWindow, A_REVERSE);
		}
		else {
			mvwprintw(myWindow, y, x, "  %s  ", myChoices[i].c_str());

		}
		++y;
	}
	wrefresh(myWindow);
}

std::string MainMenuCLIView::ask(std::string msg) {
	int choice = 0,c;
	bool is_choiced = false;
	while(is_choiced == false) {
		noecho();
		c = wgetch(myWindow);
		switch(c)	{
			case KEY_UP:
				if(myHighlight == 0)
					myHighlight = nChoices-1;
				else
					--myHighlight;
				break;
			case KEY_DOWN:
				if(myHighlight == (nChoices-1))
					myHighlight = 0;
				else
					++myHighlight;
				break;
			case 10:
				choice = myHighlight;
				is_choiced = true;
				break;
			default:
				break;
		}
		render();
		echo();
	}
	return std::string(1,(char)(choice+'0'));
}


void BoardCLIView::render(std::string msg) {

	int color;

	wclear(myWindow);

	wprintw(myWindow, "   ");
	for (int i = 0; i < myModel->getBoardSizeX(); ++i) {
		wprintw(myWindow, "%c", i+'a');
	}
	wprintw(myWindow, "\n");
	for (int i = 0; i < myModel->getBoardSizeY(); ++i) {
		wprintw(myWindow, "%2d ", myModel->getBoardSizeY()-i);
		for (int j = 0; j < myModel->getBoardSizeX(); ++j) {
			color = ((i+j)%2 == 0) + 1 ;

			wattron(myWindow, COLOR_PAIR(color) | A_BOLD);
			wprintw(myWindow, " ");
			wattroff(myWindow, COLOR_PAIR(color) | A_BOLD);
		}
		wprintw(myWindow, " %2d\n",myModel->getBoardSizeY()-i);
	}
	wprintw(myWindow, "   ");
	for (int i = 0; i < myModel->getBoardSizeX(); ++i) {
		wprintw(myWindow, "%c", i+'a');
	}
	wprintw(myWindow, "\n");

	//рендерим фигуры

	FIGURES::const_iterator itFigure;

	for (int i=0; i<2; ++i) {
		for ( itFigure=myModel->getSetFigures(i).begin(); itFigure != myModel->getSetFigures(i).end(); ++itFigure ) {
			if (itFigure->captured == false) {
				color = ((itFigure->position.myX+itFigure->position.myY)%2 == 0) + 1 ;
				if (i==1) color += 2;
				wattron(myWindow, COLOR_PAIR(color) | A_BOLD);
				wmove(myWindow,itFigure->position.myY+1,itFigure->position.myX+3);

				BoardCell bc = myModel->myBoard(itFigure->position.myX, itFigure->position.myY);
				Figure fig = myModel->getSetFigures(bc.player).at(bc.setId-1);
				wprintw(myWindow, "%c", myModel->getFigureData(fig.id).letter);

				//wprintw(myWindow, "%c", myModel->getFigureData(itFigure->id).letter);

				wattroff(myWindow, COLOR_PAIR(color) | A_BOLD);
			}
		}
	}


	wrefresh(myWindow);

}

void BoardCLIView::highlight(Position position, int color) {
	mvwchgat(myWindow,position.myY+1,position.myX+3, 1, A_BOLD, color, NULL);
	wrefresh(myWindow);
}


