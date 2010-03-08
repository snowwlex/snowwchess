/*
 * io.h
 *
 *
 *      Author: snowwlex
 */

#ifndef MODEL_IO_H_
#define MODEL_IO_H_

struct ModelIOXMLStorage {
	struct FigureInfo {
		int id;
		std::string cell;
		bool wasMoved;
	};
	int curPlayerId;
	int curFigureId;
	int firstTurn;
	std::string rulesName;
	std::vector<FigureInfo> setFiguresInfo[2];
};



class ModelIO {
	private:
		Model *myModel;
		ModelIOXMLStorage myStorage;
	public:
		ModelIO(Model *_model);
		void load(std::string file);
		void save(std::string file);
		void updateModel();

		const ModelIOXMLStorage& getStorage() const;

		friend void modelIOStartElementHandler(void *userData, const char *name, const char **atts);
		friend void modelIOEndElementHandler(void *userData, const char *name);
};



#endif /* MODEL_IO_H_ */
