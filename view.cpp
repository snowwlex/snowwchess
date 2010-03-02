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
#include "rules.h"
#include "model.h"
#include "view.h"

CLIView::CLIView(int height,int width,int y,int x, int _color, bool scroll, Model *model) {
	myWindow = newwin(height,width,y,x);
	myColor = _color;
	myModel = model;
	wbkgd(myWindow,COLOR_PAIR(myColor));
	scrollok(myWindow, scroll);
	wrefresh(myWindow);
}
void CLIView::Render(std::string msg) {
	if (msg != "") {
		wprintw(myWindow,"%s",msg.c_str());
	}
	wrefresh(myWindow);
	refresh();
}
void CLIView::Show() {
	wbkgd(myWindow,COLOR_PAIR(myColor));
	refresh();
}
void CLIView::Hide() {
	wbkgd(myWindow,COLOR_PAIR(9));
	refresh();
}
std::string CLIView::Ask(std::string msg) {
	char input_buffer[1024];
	wprintw(myWindow, "%s",msg.c_str());
	wrefresh(myWindow);
	wscanw(myWindow,"%s", input_buffer);
	return std::string(input_buffer);
}
CLIView::~CLIView() {
	wclear(myWindow);
	wbkgd(myWindow,COLOR_PAIR(5));
	wrefresh(myWindow);
	refresh();
	delwin(myWindow);
}


void MainMenuCLIView::Render(std::string msg) {
	wprintw(myWindow,"          Main Menu\n");
	wprintw(myWindow," * New Game\n");
	//wrefresh(myWindow);
	wgetch(myWindow);
}




void BoardCLIView::Render(std::string msg) {

	int color;

	wclear(myWindow);

	wprintw(myWindow, "  ");
	for (int i = 0; i < BOARD_SIZE; ++i) {
		wprintw(myWindow, "%c", i+'a');
	}
	wprintw(myWindow, "\n");

	for (int i = 0; i < BOARD_SIZE; ++i) {
		wprintw(myWindow, "%d ", 8-i);
		for (int j = 0; j < BOARD_SIZE; ++j) {
			color = ((i+j)%2 == 0) + 1 ;
			wattron(myWindow, COLOR_PAIR(color) | A_BOLD);
			wprintw(myWindow, " "); //myBoard(j,i));
			wattroff(myWindow, COLOR_PAIR(color) | A_BOLD);
		}
		wprintw(myWindow, " %d\n",8-i);
	}

	wprintw(myWindow, "  ");
	for (int i = 0; i < BOARD_SIZE; ++i) {
		wprintw(myWindow, "%c", i+'a');
	}
	wprintw(myWindow, "\n");

	//рендерим фигуры

	std::vector<Figure>::const_iterator it;

	for ( it=myModel->getSetFigures(0).begin(); it != myModel->getSetFigures(0).end(); ++it ) {
		color = ((it->position.x+it->position.y)%2 == 0) + 1 ;
		wattron(myWindow, COLOR_PAIR(color) | A_BOLD);
		wmove(myWindow,it->position.y+1,it->position.x+2);
		wprintw(myWindow, "%c", myModel->GetFigureData(it->id).letter);
		wattroff(myWindow, COLOR_PAIR(color) | A_BOLD);
	}
	for ( it=myModel->getSetFigures(1).begin(); it != myModel->getSetFigures(1).end(); ++it ) {
		color = ((it->position.x+it->position.y)%2 == 0) + 1 ;
		color += 2; // для черных фигур
		wattron(myWindow, COLOR_PAIR(color) | A_BOLD);
		wmove(myWindow,it->position.y+1,it->position.x+2);
		wprintw(myWindow, "%c", myModel->GetFigureData(it->id).letter);
		wattroff(myWindow, COLOR_PAIR(color) | A_BOLD);
	}

	wrefresh(myWindow);

}


