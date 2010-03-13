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
	
		FIGURES_RULES myMoveRules;
		FIGURES_DATA myFiguresData;
		PLAYERS_DATA myPlayersData;
		FIGURES myInitFigures[2];
		CASTLERULES myCastlesRules;
		std::string myRulesName;
		int myFirstTurn;
		int mySpecialFigure;
		int myBoardSizeX, myBoardSizeY;
	public:

		const MOVERULES& getMoveRules(int figureId);
		const FigureData& getFigureData(int figureId);
		const FIGURES& getInitFigures(int playerId) const;
		std::string getRulesName() const;
		int getFirstTurn() const;
		std::string getPlayerData(int playerId);
		int getSpecialFigure() const;
		int getBoardSizeX() const;
		int getBoardSizeY() const;

		void setPlayerData(int playerId, std::string name);
		void setSpecialFigure(int figureId);
		void setFirstTurn(int firstTurnId);
		void setBoardSize(int sizex, int sizey);
		void setRulesName(std::string rulesName);
		void setInitFigure(int playerId, const Figure& figure);
		void setMoveRule(int figureId, const MoveRule& moverule);
		void setFigureData(int figureId, const FigureData& figuredata);

		const CastleRule& getCastleRule(int dx,int dy, int player) const;
		void setCastleRule(const CastleRule& castlerule);
};

#endif /* RULES_H_ */
