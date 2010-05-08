#include <QApplication>
#include "view/main_window.h"

int main(int argc, char *argv[]) {

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    int ret = a.exec();
    Game::deleteInstance();
    return ret;
}

