
#include <QtCore>
#include <QtXml>
#include <QtGui>
#include <QApplication>

#include "snowwchess.h"
#include "rules.h"
#include "rules_io.h"
#include "model.h"
#include "player.h"
#include "guiboardview.h"
#include "game.h"
#include "mainwindow.h"



int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    mainWindow w;
    w.show();
    return a.exec();
}
