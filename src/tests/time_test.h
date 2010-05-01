#ifndef TEST_H_
#define TEST_H_

#include <ctime>

#include "../player/ai_player.h"
#include "experiment.h"

class TimeTest {

	public:
		virtual ~TimeTest();

	public:
		void setCovar(double covar);
		void setMinExp(int minExp);
		void setMaxExp(int maxExp);

	public:
		void runTest(); // main use function
		const Experiment& getExpResult() const;

	protected:
		virtual void testing() = 0;

	protected:
		TimeTest();

	protected:
		bool testingComplete;

	private:
		Experiment myExperiment;


};


#endif /* TEST_H_ */
