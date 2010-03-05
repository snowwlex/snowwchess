/*
 * rules.h
 *
 *
 *      Author: snowwlex
 */

#ifndef RULES_H_
#define RULES_H_

class Rules {
	private:
		std::map<int, std::vector<MoveRule> > myMoveRules;
		std::map < int , FigureData > myFiguresData;
		std::map <int, std::string> myPlayersData;
		std::vector<Figure> myInitFigures[2];
		std::string myRulesName;
		int myFirstTurn;
		int mySpecialFigure;
		int myBoardSizeX, myBoardSizeY;
	public:

		const std::vector < MoveRule >& GetMoveRules(int figure_id);
		const FigureData& GetFigureData(int figure_id);
		const std::vector<Figure>& GetInitFigures(int player_id) const;
		std::string GetRulesName() const;
		int GetFirstTurn() const;
		std::string GetPlayerData(int player_id);
		int GetSpecialFigure() const;
		int GetBoardSizeX() const;
		int GetBoardSizeY() const;

		void SetPlayerData(int player_id, std::string name);
		void SetSpecialFigure(int figure_id);
		void SetFirstTurn(int first_turn_id);
		void SetBoardSize(int sizex, int sizey);
		void SetRulesName(std::string rules_name);
		void SetInitFigure(int player_id, const Figure& figure);
		void SetMoveRule(int figure_id, const MoveRule& _moverule);
		void SetFigureData(int figure_id, const FigureData& figuredata);
};

#endif /* RULES_H_ */
