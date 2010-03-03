/*
 * model_io.cpp
 *
 *
 *      Author: snowwlex
 */

#include <cstdio>
#include <ncurses.h>
#include <expat.h>
#include <string>
#include <vector>
#include <map>

#include "rules.h"
#include "model.h"
#include "io.h"
#include "model_io.h"

ModelIO::ModelIO(Model *_model): model(_model) { }

void ModelIO::Load(std::string file) {
	model->InitBoard();
	model->SetInitFigures(WHITE);
	model->SetInitFigures(BLACK);
}


