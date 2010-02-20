#include <ncurses.h>
#include <cstdlib>
#include "vector"
#include "string"
#include "map"
#include "model.h"
#include "player.h"

int main(int argc, char* argv[]) {

	// инициализация графики
	initscr();

	start_color();
	//белые фигуры
	init_pair(1,COLOR_RED,COLOR_BLACK);
    init_pair(2,COLOR_RED,COLOR_WHITE);
	// черные фигуры
    init_pair(3,COLOR_CYAN,COLOR_BLACK);
    init_pair(4,COLOR_CYAN,COLOR_WHITE);
	//default:
    init_pair(5,COLOR_WHITE,COLOR_BLACK); // фон
	init_pair (6, COLOR_YELLOW, COLOR_BLACK); // окно доски
	init_pair(7,COLOR_BLACK,COLOR_WHITE); // окно выводом инфы
	init_pair(8,COLOR_WHITE,COLOR_RED); // окно игрока с белыми фигурами
	init_pair(9,COLOR_WHITE,COLOR_CYAN); // окно игрока с черными фигурами

	//окна
	WINDOW *board_window, *board_window_border; 
	WINDOW *info_window;
	WINDOW *user_window[2+1];
	//создаем
	board_window_border = newwin(12,15,1,1);
	board_window = subwin(board_window_border, 10,13,2,2);
	info_window = newwin(15, 41, 1,20);
	user_window[WHITE] = newwin(10, 60, 17, 1);
	user_window[BLACK] = newwin(10, 60, 28, 1);
	//устанавливаем цвета
	wbkgd(stdscr, COLOR_PAIR(5));
	wbkgd(board_window_border, COLOR_PAIR(5)); 
	//box(board_window_border,ACS_VLINE,ACS_HLINE);
	wborder(board_window_border, '|', '|', '-', '-', '+', '+', '+', '+');
	wbkgd(board_window,COLOR_PAIR(6));
	wbkgd(info_window,COLOR_PAIR(7));
	wbkgd(user_window[WHITE],COLOR_PAIR(8));
	wbkgd(user_window[BLACK],COLOR_PAIR(9));
    // скроллинг
	scrollok(user_window[WHITE], TRUE);
	scrollok(user_window[BLACK], TRUE);
	scrollok(info_window, TRUE);

	//инициализация модели
	Model model;
	model.Init();
	model.Render(board_window);

	
	// выводим все на экран
	wrefresh(stdscr);
	wrefresh(board_window_border);
	wrefresh(board_window);
	wrefresh(user_window[WHITE]);
	wrefresh(user_window[BLACK]);
	wrefresh(info_window);

	Player *game_player[3];
	game_player[WHITE] = new HumanPlayer(WHITE, user_window[WHITE], info_window, &model);
	game_player[BLACK] = new HumanPlayer(BLACK, user_window[BLACK], info_window, &model);
	wprintw(info_window,"Game started! White player turns! \n");
	wrefresh(info_window);
	PlayerColor cur_player = WHITE;
	bool is_end = false;
	do {

		is_end = game_player[cur_player]->YourTurn();

		wclear(board_window);
		model.Render(board_window);
		curs_set(0);
		wrefresh(board_window);

		//bool is_win = Model.is_win();

		cur_player = (cur_player == WHITE ? BLACK : WHITE);

		wprintw(info_window,"Turn of Player %d! \n",cur_player);
		wrefresh(info_window);
	
	} while(!is_end);

	wprintw(info_window,"Press any key to exit\n");
	wrefresh(info_window);
	getch();
	endwin(); 

	return 0;	
}
