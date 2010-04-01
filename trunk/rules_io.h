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
		FigureInfo() {id=0; }
	};
	struct PromotionInfo {
		int figure;
		int horizontal;
		int player;
		int promotionFigure;
		PromotionInfo () {figure = 0; horizontal = 0; player = 0; promotionFigure=0; }
	};
	struct CastleRuleInfo {
		int dx;
		int dy;
		int player;
		std::string  kingCell;
		std::string rookCellStart,rookCellEnd;
		CastleRuleInfo () {dx=0; dy=0; player=0; }
	};
	
	typedef std::vector<FigureInfo> FIGURES_INFO;
	typedef	std::vector<PromotionInfo> PROMOTIONS_INFO;
	typedef	std::vector<CastleRuleInfo> CASTLERULES_INFO;
	int curPlayerId;
	int curFigureId;
	int firstTurn;
	int specialFigureId[2];
	std::string rulesName;
	std::string section;
	int boardSizeX;
	int boardSizeY;
	PROMOTIONS_INFO promotionData;
	MOVERULES tmpMoveRule;
	PLAYERS_DATA playersData;
	FIGURES_DATA figuresData;
	FIGURES_INFO setFiguresInfo[2];
	CASTLERULES_INFO castleRules;
	FIGURES_RULES myMoveRulesIO;
};



class RulesIO {
	private:
		Rules *myRules;
		RulesIOXMLStorage myStorage;
	public:
		RulesIO(Rules *rules);
		void load(std::string file);
		void updateRules();

		const RulesIOXMLStorage& getStorage() const;

		friend void rulesIOStartElementHandler(void *userData, const char *name, const char **atts);
		friend void rulesIOEndElementHandler(void *userData, const char *name);
};


#endif /* RULES_IO_H_ */
