/*
 * io.h
 *
 *
 *      Author: snowwlex
 */

#ifndef IO_H_
#define IO_H_

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
class ModelIO {
	private:
		Model *model;
	public:
		ModelIO(Model *_model);
		void Load(std::string file = "");
};


class RulesIO {
	private:

	public:
		Rules *rules;
		RulesIO(Rules *_rules);
		void Load();
};

int makeInt(std::string value);


#endif /* IO_H_ */
