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

View::View(int x1,int x2,int y1,int y2, int _color, bool scroll, Model *model) {
	//белые фигуры
	init_pair(1,COLOR_RED,COLOR_BLACK);
	init_pair(2,COLOR_RED,COLOR_WHITE);
	//черные фигуры
	init_pair(3,COLOR_CYAN,COLOR_BLACK);
	init_pair(4,COLOR_CYAN,COLOR_WHITE);
	init_pair(5,COLOR_WHITE,COLOR_BLACK); // фон
	init_pair(6,COLOR_BLACK,COLOR_WHITE); // окно "черный текст на белом фоне"
	init_pair(7,COLOR_YELLOW,COLOR_BLACK); // окно доски
	init_pair(8,COLOR_WHITE,COLOR_RED); // окно игрока с белыми фигурами
	init_pair(9,COLOR_WHITE,COLOR_CYAN); // окно игрока с черными фигурами
	myWindow = newwin(x1,y1,x2,y2);
	myColor = _color;
	myModel = model;
	wbkgd(myWindow,COLOR_PAIR(myColor));
	scrollok(myWindow, scroll);
	wrefresh(myWindow);
}
void View::Render(std::string msg) {
	if (msg != "") {
		wprintw(myWindow,"%s\n",msg.c_str());
	}
	wrefresh(myWindow);
}
std::string View::Ask(std::string msg) {
	char input_buffer[1024];
	wprintw(myWindow, "%s",msg.c_str());
	wrefresh(myWindow);
	wscanw(myWindow,"%s", input_buffer);
	return std::string(input_buffer);
}
View::~View() {
	delwin(myWindow);
	wrefresh(stdscr);
}



MainMenuCLIView::MainMenuCLIView(int x1,int x2,int y1,int y2, int _color, bool scroll , Model *model): View(x1,y1,x2,y2,_color,scroll, model) {
	//wbkgd(stdscr, COLOR_PAIR(5)); wrefresh(stdscr);
	curs_set(0);
}


void MainMenuCLIView::Render(std::string msg) {
	wprintw(myWindow,"          Main Menu\n");
	wprintw(myWindow," * New Game\n");
	wrefresh(myWindow);
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
			wprintw(myWindow, "%c", ' '); //myBoard(j,i));
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


