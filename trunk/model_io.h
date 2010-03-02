/*
 * io.h
 *
 *
 *      Author: snowwlex
 */

#ifndef MODEL_IO_H_
#define MODEL_IO_H_

class ModelIO {
	private:
		Model *model;
	public:
		ModelIO(Model *_model);
		void Load(std::string file = "");
};



#endif /* MODEL_IO_H_ */
