#ifndef MODEL_IO_H_
#define MODEL_IO_H_

#include <QtXml>

#include "../../snowwchess.h"
#include "../model.h"

struct ModelIOXMLStorage {
	struct FigureInfo {
		int id;
		std::string cell;
		bool wasMoved;
		bool captured;
	};
	typedef std::vector<FigureInfo> FIGURES_INFO;
	int curPlayerId;
	int curFigureId;
	int firstTurn;
	std::string rulesName;
	ModelIOXMLStorage::FIGURES_INFO setFiguresInfo[2];
};


class XmlModelParser : public QXmlDefaultHandler {
	public:
		XmlModelParser(ModelIOXMLStorage* storage);

	public:
		bool startElement(const QString&, const QString&, const QString& tagName, const QXmlAttributes& attrs);
		bool endElement(const QString&, const QString&, const QString& tagName);
		bool fatalError(const QXmlParseException& exception);

	private: //help methods

		void positionsTag(const QString& tag, const QXmlAttributes& attrs);
		void playerTag(const QString& tag, const QXmlAttributes& attrs);
		void figureTag(const QString& tag, const QXmlAttributes& attrs);
		void positionTag(const QString& tag, const QXmlAttributes& attrs);

	private:
		ModelIOXMLStorage* myStorage;
};


class ModelIO {
	public:
		ModelIO();

	public:
		void load(std::string file);
		void save(std::string file, const Model& model) const;
		void updateModel(Model& model) const;

	public: //getters
		const ModelIOXMLStorage& getStorage() const;

	private: //update model methods
		void updateFiguresInfo(Model& model) const;

	private:
		ModelIOXMLStorage myStorage;

};



#endif /* MODEL_IO_H_ */
