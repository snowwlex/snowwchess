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
		inline FigureInfo() {id=0; }
	};
	struct PromotionInfo {
		int figure;
		int horizontal;
		int player;
		int promotionFigure;
		inline PromotionInfo () {figure = 0; horizontal = 0; player = 0; promotionFigure=0; }
	};
	struct CastleRuleInfo {
		int dx;
		int dy;
		int player;
		std::string  kingCell;
		std::string rookCellStart,rookCellEnd;
		inline CastleRuleInfo () {dx=0; dy=0; player=0; }
	};
	int curPlayerId;
	int curFigureId;
	int firstTurn;
	int specialFigure;
	std::string rulesName;
	std::string section;
	int boardSizeX;
	int boardSizeY;
	std::vector<PromotionInfo> promotionData;
	std::vector<MoveRule> tmpMoveRule;
	std::map < int , std::string > playersData;
	std::map < int , FigureData > figuresData;
	std::vector<FigureInfo> setFiguresInfo[2];
	std::vector<CastleRuleInfo> castleRules;
	std::map< int, std::vector<MoveRule> > myMoveRulesIO;
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
