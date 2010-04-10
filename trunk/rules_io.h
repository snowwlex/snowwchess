/*
 * io.h
 *
 *
 *      Author: snowwlex
 */

#ifndef RULES_IO_H_
#define RULES_IO_H_

#include <QtXml>

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


class XmlRulesParser : public QXmlDefaultHandler {
	public:
		XmlRulesParser(RulesIOXMLStorage* storagePtr);

	public:
		bool startElement(const QString&, const QString&, const QString& tagName, const QXmlAttributes& attrs);
		//bool characters(const QString& strText);
		bool endElement(const QString&, const QString&, const QString& tagName);
		bool fatalError(const QXmlParseException& exception);

	private:
		RulesIOXMLStorage* myStoragePtr;
};


class RulesIO {

	public:
		RulesIO();

	public:
		bool load(std::string file);
		void updateRules(Rules& rules) const;

	public:
		const RulesIOXMLStorage& getStorage() const;

	private:
		void updateInitFigures(Rules& rules) const;
		void updateCastleRules(Rules& rules) const;
		void updateFiguresData(Rules& rules) const;
		void updatePlayersData(Rules& rules) const;
		void updateMoveRules(Rules& rules) const;

	private: //after parsing processing
		void setPromotionRules();

	private:
		RulesIOXMLStorage myStorage;
};





#endif /* RULES_IO_H_ */
