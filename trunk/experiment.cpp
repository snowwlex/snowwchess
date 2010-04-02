/*
 * experiment.cpp
 *
 *  Created on: 03.04.2010
 *      Author: snowwlex
 */

#include <math.h>

#include "experiment.h"

Experiment::Experiment(double covar,int minExp, int maxExp) : myCovar(covar), myMinExp(minExp), myMaxExp(maxExp) {
	myExpCounter = 0;
	mySumX = 0;
	mySumXSquare = 0;
};

void Experiment::addValue(double t) {
	++myExpCounter;
	mySumX += t;
	mySumXSquare += pow(t,2);
}

int Experiment::isReady() {
	// 1 - if maxExperiment is achieved
	// 2 - if covariant >= setted
	// 0 - if it's not ready

	if (myExpCounter >= myMaxExp) {
		return 1;
	}
	if (myExpCounter >= myMinExp && countCovar() > myCovar) {
		return 2;
	}
	return 0;
}

double Experiment::getMean() {
	return mySumX / myExpCounter;
}

double Experiment::countCovar() {
	return sqrt(countVariance())/getMean();

}

double Experiment::countVariance() {
	return mySumXSquare/myExpCounter - pow(mySumX/myExpCounter,2);
}






