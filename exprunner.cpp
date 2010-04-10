/*
 * exprunner.cpp
 *
 *  Created on: 03.04.2010
 *      Author: snowwlex
 */

#include <vector>
#include <string>
#include <ctime>
#include <cstdio>
#include <map>

#include "snowwchess.h"
#include "experiment.h"
#include "exprunner.h"


//ExpRunner::ExpRunner(const Experiment& experiment) : myExperiment(experiment) {
//
//}
//
//void ExpRunner::run(expFunction function, std::vector<int> initNumbers, std::string file) {
//	time_t start, end;
//
//	FILE *outfile = fopen(file.c_str(), "wt");
//
//	for (std::vector<int>::const_iterator itNumber = initNumbers.begin(); itNumber != initNumbers.end(); ++itNumber) {
//		myExperiment.clear();
//		while (myExperiment.isReady() == 0) {
//			time(&start);
//			function(*itNumber);
//			time(&end);
//			myExperiment.addValue( difftime(end,start) );
//		}
//		sprintf(buffer, "Number: %d, mean: %f\n",*itNumber, myExperiment.getMean());
//		fputs(buffer,outfile);
//	}
//	fclose(outfile);
//}

