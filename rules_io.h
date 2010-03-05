/*
 * io.h
 *
 *
 *      Author: snowwlex
 */

#ifndef RULES_IO_H_
#define RULES_IO_H_

struct RulesIOXMLStorage {
	struct FigureInfo {
		int id;
		std::string cell;
	};
	int cur_player_id;
	int cur_figure_id;
	int first_turn;
	int special_figure;
	std::string rules_name;
	std::string section;
	int boardsize_x;
	int boardsize_y;
	std::vector<MoveRule> tmp_move_rule;
	std::map < int , std::string > PlayersData;
	std::map < int , FigureData > FiguresData;
	std::vector<FigureInfo> SetFiguresInfo[2];
	std::map< int, std::vector<MoveRule> > myMoveRulesIO;
};



class RulesIO {
	private:
		Rules *myRules;
		RulesIOXMLStorage myStorage;
	public:
		RulesIO(Rules *_rules);
		void Load(std::string file);
		void UpdateRules();

		const RulesIOXMLStorage& getStorage() const;

		friend void RulesIOstartElementHandler(void *userData, const char *name, const char **atts);
		friend void RulesIOendElementHandler(void *userData, const char *name);
};


#endif /* RULES_IO_H_ */
