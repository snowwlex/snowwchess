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
		bool unmoved;
	};
	int cur_player_id;
	int cur_figure_id;
	int first_turn;
	std::string rules_name;
	std::vector<FigureInfo> SetFiguresInfo[2];
};



class ModelIO {
	private:
		Model *myModel;
		ModelIOXMLStorage myStorage;
	public:
		ModelIO(Model *_model);
		void Load(std::string file);
		void Save(std::string file);
		void UpdateModel();

		const ModelIOXMLStorage& getStorage() const;

		friend void ModelIOstartElementHandler(void *userData, const char *name, const char **atts);
		friend void ModelIOendElementHandler(void *userData, const char *name);
};



#endif /* MODEL_IO_H_ */
