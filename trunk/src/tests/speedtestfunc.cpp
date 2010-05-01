#include <iostream>
#include <vector>
#include <QString>

#include "ai_speed_test.h"
#include "../player/alphabeta_parallel_search_ai_player.h"
#include "../player/alphabeta_search_ai_player.h"

#include "../model/io/rules_io.h"
#include "../model/io/model_io.h"


void onlyWarningMessageOutput(QtMsgType type, const char * msg) {
	switch ( type ) {
		case QtDebugMsg:
			break;
		case QtWarningMsg:
			std::cout << msg << std::endl;
			break;
		case QtFatalMsg:
			abort();
	}
}

static QString mittelshpielTest(QString testName, AIPlayer* player, bool abEqualPruning) {
	player->setABEqualPruning(abEqualPruning);

	AISpeedTest test;
	test.setPlayer(player);
	test.setCovar(0.5);
	test.setMinExp(1);
	test.setMaxExp(1);

	RulesIO rulesIO;
	ModelIO modelIO;
	rulesIO.load("rules/classic.xml");
	modelIO.load("saves/save.xml");

	Rules rules;
	Model model;
	model.setRules(&rules);

	rulesIO.updateRules(rules);
	modelIO.updateModel(model);
	model.init(false);

	player->setModel(&model);
	player->setColor(model.getFirstTurnPlayer());
	player->addListener(&test);
	test.runTest();

	double time = test.getExpResult().getMean();
	QString string = "Mittelshpiel: " + testName + ": time is " + QVariant(time).toString();
	std::cout << string.toStdString();
	delete player;
	return string;
}

static QString debutTest(QString testName, AIPlayer* player, bool abEqualPruning) {
	player->setABEqualPruning(abEqualPruning);

	AISpeedTest test;
	test.setPlayer(player);
	test.setCovar(0.5);
	test.setMinExp(1);
	test.setMaxExp(1);

	RulesIO rulesIO;
	rulesIO.load("rules/classic.xml");

	Rules rules;
	Model model;
	model.setRules(&rules);

	rulesIO.updateRules(rules);
	model.init(true);

	player->setModel(&model);
	player->setColor(model.getFirstTurnPlayer());
	player->addListener(&test);
	test.runTest();

	double time = test.getExpResult().getMean();
	QString string = "Debute: " + testName + ": time is " + QVariant(time).toString();
	std::cout << string.toStdString() << std::endl;
	delete player;
	return string;
}


int makeAISpeedTest() {

	qInstallMsgHandler( onlyWarningMessageOutput );

	std::vector<QString> results;

	results.push_back( debutTest("AB search, deep 3", new AlphaBetaSearchAIPlayer(3,0) , false ) );
	results.push_back( debutTest("AB search, deep 4", new AlphaBetaSearchAIPlayer(4,0) , false ) );
	results.push_back( debutTest("AB search, deep 5", new AlphaBetaSearchAIPlayer(5,0) , false ) );
	results.push_back( debutTest("AB search, deep 6", new AlphaBetaSearchAIPlayer(6,0) , false ) );

	results.push_back( debutTest("AB search, deep 3, a==b pruning", new AlphaBetaSearchAIPlayer(3,0), true ) );
	results.push_back( debutTest("AB search, deep 4, a==b pruning", new AlphaBetaSearchAIPlayer(4,0), true ) );\
	results.push_back( debutTest("AB search, deep 5, a==b pruning", new AlphaBetaSearchAIPlayer(5,0), true ) );
	results.push_back( debutTest("AB search, deep 6, a==b pruning", new AlphaBetaSearchAIPlayer(6,0), true ) );

	results.push_back( debutTest("AB parallel, deep 3", new AlphaBetaParallelSearchAIPlayer(3,0), false ) );
	results.push_back( debutTest("AB parallel, deep 4", new AlphaBetaParallelSearchAIPlayer(4,0), false ) );
	results.push_back( debutTest("AB parallel, deep 5", new AlphaBetaParallelSearchAIPlayer(5,0), false ) );
	results.push_back( debutTest("AB parallel, deep 6", new AlphaBetaParallelSearchAIPlayer(6,0), false ) );

	results.push_back( debutTest("AB parallel, deep 3, a==b pruning", new AlphaBetaParallelSearchAIPlayer(3,0), true ) );
	results.push_back( debutTest("AB parallel, deep 4, a==b pruning", new AlphaBetaParallelSearchAIPlayer(4,0), true ) );
	results.push_back( debutTest("AB parallel, deep 5, a==b pruning", new AlphaBetaParallelSearchAIPlayer(5,0), true ) );
	results.push_back( debutTest("AB parallel, deep 6, a==b pruning", new AlphaBetaParallelSearchAIPlayer(6,0), true ) );



	/**************************************************************************************************/

	results.push_back( mittelshpielTest("AB search, deep 3", new AlphaBetaSearchAIPlayer(3,0) , false ) );
	results.push_back( mittelshpielTest("AB search, deep 4", new AlphaBetaSearchAIPlayer(4,0) , false ) );
	results.push_back( mittelshpielTest("AB search, deep 5", new AlphaBetaSearchAIPlayer(5,0) , false ) );
	results.push_back( mittelshpielTest("AB search, deep 6", new AlphaBetaSearchAIPlayer(6,0) , false ) );

	results.push_back( mittelshpielTest("AB search, deep 3, a==b pruning", new AlphaBetaSearchAIPlayer(3,0), true ) );
	results.push_back( mittelshpielTest("AB search, deep 4, a==b pruning", new AlphaBetaSearchAIPlayer(4,0), true ) );\
	results.push_back( mittelshpielTest("AB search, deep 5, a==b pruning", new AlphaBetaSearchAIPlayer(5,0), true ) );
	results.push_back( mittelshpielTest("AB search, deep 6, a==b pruning", new AlphaBetaSearchAIPlayer(6,0), true ) );

	results.push_back( mittelshpielTest("AB parallel, deep 3", new AlphaBetaParallelSearchAIPlayer(3,0), false ) );
	results.push_back( mittelshpielTest("AB parallel, deep 4", new AlphaBetaParallelSearchAIPlayer(4,0), false ) );
	results.push_back( mittelshpielTest("AB parallel, deep 5", new AlphaBetaParallelSearchAIPlayer(5,0), false ) );
	results.push_back( mittelshpielTest("AB parallel, deep 6", new AlphaBetaParallelSearchAIPlayer(6,0), false ) );

	results.push_back( mittelshpielTest("AB parallel, deep 3, a==b pruning", new AlphaBetaParallelSearchAIPlayer(3,0), true ) );
	results.push_back( mittelshpielTest("AB parallel, deep 4, a==b pruning", new AlphaBetaParallelSearchAIPlayer(4,0), true ) );
	results.push_back( mittelshpielTest("AB parallel, deep 5, a==b pruning", new AlphaBetaParallelSearchAIPlayer(5,0), true ) );
	results.push_back( mittelshpielTest("AB parallel, deep 6, a==b pruning", new AlphaBetaParallelSearchAIPlayer(6,0), true ) );

	std::cout << std::endl;
	std::cout << "Total info: " << std::endl;
	for (size_t i = 0; i != results.size(); ++i) {
		std::cout << results[i].toStdString() << std::endl;
	}


	return 0;


}
