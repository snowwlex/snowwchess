/*
 * main.cpp
 *
 *
 *      Author: snowwlex
 */

#include <ncurses.h>
#include <vector>
#include <map>
#include <string>

#include "snowwchess.h"
#include "model.h"
#include "view.h"
#include "game.h"

void init_graphic() {
	initscr();	start_color();
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
	init_pair(10,COLOR_BLACK,COLOR_BLACK); // для метода CLIView::Hide()
	init_pair(11,COLOR_BLACK,COLOR_RED); // debug view
}

CLIView *debug_view;

int main(int argc, char* argv[]) {
	init_graphic();
	std::string option,name;
	int mode=0;
	Game game;
	CLIView *menu_view = new MainMenuCLIView(8,30,2,20,6);
	CLIView *choose_view;
	debug_view = new CLIView(15,70,24,40,11,true);
	debug_view->Render("debug view\n");

	do {
		menu_view->Render();
		option = menu_view->Ask();
		if (option == "0") {
			choose_view = new CLIView(5,30,15,20,6);
			choose_view->Render("Enter name of rules:\n");
			name = choose_view->Ask("> ");
			mode = 0;
			delete choose_view;
		}
		else if (option == "1") {
			choose_view = new CLIView(5,30,15,20,6);
			choose_view->Render("Enter savename:\n");
			name = choose_view->Ask("> ");
			mode = 1;
			delete choose_view;
		}

		if (option != "2") {
			menu_view->Hide();
			game.Start(name,mode);
			menu_view->Show();
		}

	} while(option != "2");

	delete menu_view;
	delete debug_view;
	endwin();
	return 0;
}
