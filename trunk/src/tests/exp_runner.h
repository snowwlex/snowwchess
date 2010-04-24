/*
 * exprunner.h
 *
 *  Created on: 03.04.2010
 *      Author: snowwlex
 */

#ifndef EXPRUNNER_H_
#define EXPRUNNER_H_

class ExpRunner {
	private:
		Experiment myExperiment;
	public:
		ExpRunner(const Experiment& experiment) : myExperiment(experiment) { }

		template <typename F>
		void run(F function, std::vector<int> initNumbers, std::string file ) {
			time_t start, end;

			FILE *outfile = fopen(file.c_str(), "wt");

			for (std::vector<int>::const_iterator itNumber = initNumbers.begin(); itNumber != initNumbers.end(); ++itNumber) {
				myExperiment.clear();
				while (myExperiment.isReady() == 0) {
					time(&start);
					function(*itNumber);
					time(&end);
					myExperiment.addValue( difftime(end,start) );
				}
				//sprintf(buffer, "Number: %d, mean: %f, %s\n",*itNumber, myExperiment.getMean(), myExperiment.isReady() == 1 ? "max experiments done": "covariant is less than setted");
				//sprintf(buffer, "%d %f\n",*itNumber, myExperiment.getMean() );//, myExperiment.isReady() == 1 ? "max experiments done": "covariant is less than setted");
				//fputs(buffer,outfile);
			}
			fclose(outfile);
		}

};

#endif /* EXPRUNNER_H_ */
