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
	struct CastleRuleInfo {
		int dx;
		int dy;
		int player;
		std::string  kingCell;
		std::string rookCellStart,rookCellEnd;
	};
	int curPlayerId;
	int curFigureId;
	int firstTurn;
	int specialFigure;
	std::string rulesName;
	std::string section;
	int boardSizeX;
	int boardSizeY;
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
