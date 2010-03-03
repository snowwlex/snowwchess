/*
 * io.h
 *
 *
 *      Author: snowwlex
 */

#ifndef RULES_IO_H_
#define RULES_IO_H_

struct XMLStorage {
	int cur_player_id;
	int cur_figure_id;
	std::string rules_name;
	std::string section;
	int boardsize_x;
	int boardsize_y;
	std::vector<MoveRule> tmp_move_rule;
	std::map < int , FigureData > FiguresData;
	std::vector<Figure> SetFigures[2];
	std::map< int, std::vector<MoveRule> > myMoveRulesIO;
};


class RulesIO {
	private:
		Rules *rules;
	public:
		RulesIO(Rules *_rules);
		void Load(std::string rules_file);
};


#endif /* RULES_IO_H_ */
