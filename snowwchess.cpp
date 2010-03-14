/*
 * main.cpp
 *
 *
 *      Author: snowwlex
 */

//for randomizator
#include <cstdlib>
#include <ctime>


#include <ncurses.h>
#include <vector>
#include <map>
#include <string>

#include "snowwchess.h"
#include "model.h"
#include "view.h"
#include "game.h"

void initGraphic() {
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
	init_pair(11,COLOR_BLACK,COLOR_RED); // debug view & eat moves
	init_pair(12,COLOR_BLACK,COLOR_CYAN); // move moves
	init_pair(13,COLOR_BLACK,COLOR_YELLOW); // highlight
}

CLIView *debugView;
char buffer[1024] = {' '};
int randomizator=0;
FILE* statfile = 0;
/*
sprintf(buffer,"Highlighted: %d\n",highlight); debugView->render(buffer);
*/

int main(int argc, char* argv[]) {
	initGraphic();
	std::string option,name;
	int mode=0;
	Game game;
	CLIView *menuView = new MainMenuCLIView(8,30,2,20,6);
	CLIView *chooseView;
	debugView = new CLIView(38,70,2,75,11,true);
	debugView->render("debug view\n");

	do {
		menuView->render();
		option = menuView->ask();
		if (option == "0") {
			chooseView = new CLIView(5,30,15,20,6);
			chooseView->render("Enter name of rules:\n");
			name = chooseView->ask("> ");
			mode = 0;
			delete chooseView;
		}
		else if (option == "1") {
			chooseView = new CLIView(5,30,15,20,6);
			chooseView->render("Enter savename:\n");
			name = chooseView->ask("> ");
			mode = 1;
			delete chooseView;
		}

		if (option != "2") {
			srand (time(0));
			statfile = fopen("statfile2.txt", "wt");
			menuView->hide();
			for (int i=0; i< 1; ++i) {
				randomizator = rand()%10000+200;
				game.start(name,mode);
			}
			fclose(statfile);
			menuView->show();
		}

	} while(option != "2");

	delete menuView;
	delete debugView;
	endwin();
	return 0;
}