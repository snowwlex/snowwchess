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
}
void CLIView::Show() {
	wbkgd(myWindow,COLOR_PAIR(myColor));
	wrefresh(myWindow);
	refresh();
}
void CLIView::Hide() {
	wbkgd(myWindow,COLOR_PAIR(10));
	wrefresh(myWindow);
	refresh();
}
std::string CLIView::Ask(std::string msg) {
	char input_buffer[1024];
	curs_set(1);
	wprintw(myWindow, "%s",msg.c_str());
	wrefresh(myWindow);
	wscanw(myWindow,"%s", input_buffer);
	return std::string(input_buffer);
}
void CLIView::Wait() {
	wgetch(myWindow);
}
int CLIView::GetKey() {
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
	highlight = 0;
	keypad(myWindow, TRUE);
	choices[0] = "New Game";
	choices[1] = "Load Game";
	choices[2] = "Exit";
	n_choices = 3;
}

void MainMenuCLIView::Render(std::string msg) {
	int x,y,i;
	wclear(myWindow);
	curs_set(0);
	wprintw(myWindow,"--- Main Menu ---\n");

	for(i = 0,x=2,y=2; i < n_choices; ++i){
		if(highlight == i)	{
			wattron(myWindow, A_REVERSE);
			mvwprintw(myWindow, y, x, "  %s  ", choices[i].c_str());
			wattroff(myWindow, A_REVERSE);
		}
		else {
			mvwprintw(myWindow, y, x, "  %s  ", choices[i].c_str());

		}
		++y;
	}
	wrefresh(myWindow);
}

std::string MainMenuCLIView::Ask(std::string msg) {
	int choice = 0,c;
	bool is_choiced = false;
	while(is_choiced == false) {
		noecho();
		c = wgetch(myWindow);
		switch(c)	{
			case KEY_UP:
				if(highlight == 0)
					highlight = n_choices-1;
				else
					--highlight;
				break;
			case KEY_DOWN:
				if(highlight == (n_choices-1))
					highlight = 0;
				else
					++highlight;
				break;
			case 10:
				choice = highlight;
				is_choiced = true;
				break;
			default:
				break;
		}
		Render();
		echo();
	}
	return std::string(1,(char)(choice+'0'));
}


void BoardCLIView::Render(std::string msg) {

	int color;

	wclear(myWindow);

	wprintw(myWindow, "   ");
	for (int i = 0; i < myModel->GetBoardSizeX(); ++i) {
		wprintw(myWindow, "%c", i+'a');
	}
	wprintw(myWindow, "\n");
	for (int i = 0; i < myModel->GetBoardSizeY(); ++i) {
		wprintw(myWindow, "%2d ", myModel->GetBoardSizeY()-i);
		for (int j = 0; j < myModel->GetBoardSizeX(); ++j) {
			color = ((i+j)%2 == 0) + 1 ;

			wattron(myWindow, COLOR_PAIR(color) | A_BOLD);
			wprintw(myWindow, " ");
			wattroff(myWindow, COLOR_PAIR(color) | A_BOLD);
		}
		wprintw(myWindow, " %2d\n",myModel->GetBoardSizeY()-i);
	}
	wprintw(myWindow, "   ");
	for (int i = 0; i < myModel->GetBoardSizeX(); ++i) {
		wprintw(myWindow, "%c", i+'a');
	}
	wprintw(myWindow, "\n");

	//рендерим фигуры

	std::vector<Figure>::const_iterator it;

	for ( it=myModel->getSetFigures(WHITE).begin(); it != myModel->getSetFigures(WHITE).end(); ++it ) {
		color = ((it->position.x+it->position.y)%2 == 0) + 1 ;
		wattron(myWindow, COLOR_PAIR(color) | A_BOLD);
		wmove(myWindow,it->position.y+1,it->position.x+3);
		wprintw(myWindow, "%c", myModel->GetFigureData(it->id).letter);
		wattroff(myWindow, COLOR_PAIR(color) | A_BOLD);
	}
	for ( it=myModel->getSetFigures(BLACK).begin(); it != myModel->getSetFigures(BLACK).end(); ++it ) {
		color = ((it->position.x+it->position.y)%2 == 0) + 1 ;
		color += 2; // для черных фигур
		wattron(myWindow, COLOR_PAIR(color) | A_BOLD);
		wmove(myWindow,it->position.y+1,it->position.x+3);
		wprintw(myWindow, "%c", myModel->GetFigureData(it->id).letter);
		wattroff(myWindow, COLOR_PAIR(color) | A_BOLD);
	}

	wrefresh(myWindow);

}

void BoardCLIView::Highlight(Position position, int color) {
	int figure_id = myModel->GetBoard(position.x,position.y);
	char figure_letter = ' ';
	if (figure_id > 0) {
		figure_letter = myModel->GetFigureData(figure_id).letter;
	}
	mvwchgat(myWindow,position.y+1,position.x+3, 1, A_BOLD, color, NULL);

	wrefresh(myWindow);
}


