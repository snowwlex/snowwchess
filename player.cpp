#include <ncurses.h>
#include "vector"
#include "string"
#include "map"
#include "model.h"
#include "player.h"

HumanPlayer::HumanPlayer(PlayerColor color, WINDOW * user_w, WINDOW* info_w, Model* m):  myColor(color), user_window(user_w), info_window(info_w), model(m) { }

bool HumanPlayer::YourTurn() {

	char input_command[256];
	std::string command;
	int x1,y1, x2,y2;
	std::vector< Move > moves;
	bool exit = false;
	curs_set(1);
	do {
	    
		wprintw(user_window, "> ");	wrefresh(user_window);
		wscanw(user_window,"%s", input_command);
		command = input_command;
		
		if (command.length() == 2) {
			x1 = command[0]-'a';
			y1 = command[1]-'1';
			
			moves.clear();
			moves = model->moves(Position(x1,y1));
			wprintw(info_window, "Available moves: \n");
			std::vector < Move >::iterator it;
			for ( it=moves.begin() ; it < moves.end(); it++ ) {
				wprintw(info_window, "[%c%c-%c%c (%s)] ", x1+'a',y1+'1',it->square_pos.x+'a', it->square_pos.y+'1', it->mode == EAT ? "eat" : "move" );		
			}
			wprintw(info_window, "\n");
			wrefresh(info_window);
		} else if (command == "exit") {
			exit = true;		
		} else { // обычная команда
			x1 = input_command[0]-'a';
			y1 = input_command[1]-'1';
			x2 = input_command[3]-'a';
			y2 = input_command[4]-'1';

			bool result = model->move(Position(x1,y1), Position(x2,y2));
			if (result == false) {
				wprintw(user_window, "It's not available to move at this square!\n");
				wrefresh(user_window);	
			}
			else {
				exit = true;			
			}
		}
		
	} while( !exit );

	if (command == "exit") {
		return true;
	}
	return false;
}
