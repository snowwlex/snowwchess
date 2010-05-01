#ifndef EXPERIMENT_H_
#define EXPERIMENT_H_

class Experiment {
	public:
		Experiment();

	public:
		void setCovar(double covar);
		void setMinExp(int minExp);
		void setMaxExp(int maxExp);

	public:
		void clearExpData();
		void addValue(double value);
		double getMean() const;

	public: // experiment's finish condition
		int isReady() const;
		// isReady() return:
		// 0 - if it's not ready
		// 1 - if maxExperiment is achieved
		// 2 - if covariant < setted

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

};

#endif /* EXPERIMENT_H_ */
