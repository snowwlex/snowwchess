/*
 * io.h
 *
 *
 *      Author: snowwlex
 */

#ifndef MODEL_IO_H_
#define MODEL_IO_H_

#include "model.h"

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



class ModelIO {
	public:
		ModelIO(Model *_model);

	public:
		void load(std::string file);
		void save(std::string file);
		const ModelIOXMLStorage& getStorage() const;

	public: //update methods
		void updateModel();

	private:
		Model *myModel;
		ModelIOXMLStorage myStorage;

	friend void modelIOStartElementHandler(void *userData, const char *name, const char **atts);
	friend void modelIOEndElementHandler(void *userData, const char *name);
};



#endif /* MODEL_IO_H_ */
