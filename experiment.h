/*
 * experiment.h
 *
 *  Created on: 03.04.2010
 *      Author: snowwlex
 */

#ifndef EXPERIMENT_H_
#define EXPERIMENT_H_

class Experiment {
	private: // initial data
		double myCovar;
		int myMinExp;
		int myMaxExp;
	private: // experiment data
		int myExpCounter;
		double mySumX;
		double mySumXSquare;
	private: // help methods
		double countCovar() const;
		double countVariance() const;
	public:
		Experiment(double covar,int minExp, int maxExp);
		void clear();
		void addValue(double t);
		int isReady() const;
		double getMean() const;

};

#endif /* EXPERIMENT_H_ */
