#include <math.h>
#include <cassert>

#include "experiment.h"


Experiment::Experiment() : myCovar(0), myMinExp(0), myMaxExp(0) {
	clearExpData();
};

void Experiment::setCovar(double covar) {
	myCovar = covar;
}
void Experiment::setMinExp(int minExp) {
	myMinExp = minExp;
}
void Experiment::setMaxExp(int maxExp) {
	myMaxExp = maxExp;
}

void Experiment::addValue(double value) {
	++myExpCounter;
	mySumX += value;
	mySumXSquare += pow(value,2);
}

int Experiment::isReady() const {
	// 1 - if maxExperiment is achieved
	// 2 - if covariant < setted
	// 0 - if it's not ready

	assert(myMaxExp > 0);
	assert(myMinExp > 0);
	assert(myCovar > 0);

	if (myExpCounter >= myMaxExp) {
		return 1;
	}
	if (myExpCounter > 0 && myExpCounter >= myMinExp && countCovar() < myCovar) {
		return 2;
	}
	return 0;
}

void Experiment::clearExpData() {
	myExpCounter = 0;
	mySumX = 0;
	mySumXSquare = 0;
}

double Experiment::getMean() const {
	return mySumX / myExpCounter;
}

double Experiment::countCovar() const {
	return sqrt(countVariance())/getMean();

}

double Experiment::countVariance() const {
	return mySumXSquare/myExpCounter - pow(mySumX/myExpCounter,2);
}






