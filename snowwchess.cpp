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
	CLIView *view = new MainMenuCLIView(6,30,2,20,6);
	debug_view = new CLIView(15,70,24,40,11,true);
	debug_view->Render("debug view\n");
	view->Render();
	view->Wait();
	view->Hide();

	Game game;

	game.Start("classic", 0);
	//game.Start("save",1);

	view->Show();
	view->Render("Press enter\n");
	view->Wait();
	delete view;
	delete debug_view;
	endwin();
	return 0;
}
