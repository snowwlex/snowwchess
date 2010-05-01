#include <QApplication>
#include "view/main_window.h"
#include "tests/ai_speed_test.h"

int main(int argc, char *argv[]) {

	int curArg = 1;
	bool test = false;
	while (curArg < argc) {
		if (argv[curArg][0] == '-') {
			switch(argv[curArg][1]) {
				case 't':
					test = true;
					break;
				default:
					break;
			}
		}
		++curArg;
	}

	if (test == true) {
		makeAISpeedTest();
		return 0;
	}

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

