#ifndef TIME_TEST_H_
#define TIME_TEST_H_

#include <ctime>
#include "time_test.h"

TimeTest::TimeTest() {

}

TimeTest::~TimeTest() {

}
void TimeTest::runTest() {
	time_t start, end;
	myExperiment.clearExpData();
	while (myExperiment.isReady() ==  0) {
		testingComplete = false;
		time(&start);
		testing();
		while (testingComplete == false);
		time(&end);
		myExperiment.addValue( difftime(end,start) );
	}
}

void TimeTest::setCovar(double covar) {
	myExperiment.setCovar(covar);
}
void TimeTest::setMinExp(int minExp) {
	myExperiment.setMinExp(minExp);
}
void TimeTest::setMaxExp(int maxExp) {
	myExperiment.setMaxExp(maxExp);
}

const Experiment& TimeTest::getExpResult() const {
	return myExperiment;
}

#endif /* TIME_TEST_H_ */
