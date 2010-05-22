#include <QMessageBox>
#include <QtXml>
#include <QLayout>
#include <QFileDialog>
#include <QSplitter>
#include <QSettings>
#include <cassert>

#include "main_window.h"
#include "dialog_new_game.h"

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent) {
    setupUi();
    Game::instance()->setBoardView(this->boardView);
    Game::instance()->setHistoryView(this->historyView);
    bool ok = restoreLastGame();
    if (ok == false) {
        qDebug() << ":MainWindow: " << "constructor " << "last game was not saved";
    }

}

MainWindow::~MainWindow() {

}

void MainWindow::setupUi() {
    this->resize(500, 500);
    this->setCentralWidget(new QWidget(this));
    this->setMenuBar(new QMenuBar(this));
    //this->setStatusBar( new QStatusBar(this) );

    // ACTIONS
    actionNewGame = new QAction(this);
    actionSaveGame = new QAction(this);
    actionLoadGame = new QAction(this);
    actionExit = new QAction(this);

    // MENU BAR
    menu = new QMenu(this->menuBar());

    // ADD MENUS
    this->menuBar()->addAction(menu->menuAction());
    menu->addAction(actionNewGame);
    menu->addSeparator();
    menu->addAction(actionSaveGame);
    menu->addAction(actionLoadGame);
    menu->addSeparator();
    menu->addAction(actionExit);

    // LAYOUTS
    QBoxLayout* boxLayout = new QBoxLayout(QBoxLayout::LeftToRight);
    this->centralWidget()->setLayout(boxLayout);

    // Board View
    boardView = new GuiBoardView();
    boardView->setCursor(QCursor(Qt::OpenHandCursor));
    boxLayout->addWidget(boardView);

    // HISTORY View
    historyView = new GuiHistoryView();
    historyView->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored);
    boxLayout->addWidget(historyView);


    setWindowIcon( QIcon(":/images/interface/chess.png")  );
    retranslateUi();

    // CONNECTS

    QObject::connect(this->actionExit, SIGNAL(activated()), this, SLOT(on_actionExit_activated()));
    QObject::connect(this->actionNewGame, SIGNAL(activated()), this, SLOT(on_actionNewGame_activated()));
    QObject::connect(this->actionLoadGame, SIGNAL(activated()), this, SLOT(on_actionLoadGame_activated()));
    QObject::connect(this->actionSaveGame, SIGNAL(activated()), this, SLOT(on_actionSaveGame_activated()));





}

void MainWindow::retranslateUi() {
    this->setWindowTitle(QApplication::translate("MainWindow", "SnowwChess", 0, QApplication::UnicodeUTF8));
    actionNewGame->setText(QApplication::translate("MainWindow", "New game", 0, QApplication::UnicodeUTF8));
    actionNewGame->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", 0, QApplication::UnicodeUTF8));
    actionSaveGame->setText(QApplication::translate("MainWindow", "Save", 0, QApplication::UnicodeUTF8));
    actionSaveGame->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
    actionLoadGame->setText(QApplication::translate("MainWindow", "Load", 0, QApplication::UnicodeUTF8));
    actionExit->setText(QApplication::translate("MainWindow", "Exit", 0, QApplication::UnicodeUTF8));
    actionExit->setShortcut(QApplication::translate("MainWindow", "Alt+X", 0, QApplication::UnicodeUTF8));
    menu->setTitle(QApplication::translate("MainWindow", "Game", 0, QApplication::UnicodeUTF8));

}

void MainWindow::on_actionSaveGame_activated() {

    QString file = QFileDialog::getSaveFileName(this, "Save board to", "saves", tr("Saves in xml (*.xml)"));
    Game::instance()->saveGame(file.toStdString());
    QMessageBox(QMessageBox::Information, tr("Ok!"), tr("File with board is saved"), QMessageBox::Ok).exec();

}

void MainWindow::on_actionNewGame_activated() {

    DialogNewGame* dialog = new DialogNewGame();
    dialog->setLoadFileText(tr("Opening file with rules"), "rules", tr("Rules in xml (*.xml)"));
    if (dialog->exec() != QDialog::Accepted) {
        delete dialog;
        return;
    }

    PlayerType playerTypes[2];
    int aiLevels[2];

    QString fileName = dialog->getFileName();

    playerTypes[WHITE] = dialog->getPlayerType(WHITE);
    playerTypes[BLACK] = dialog->getPlayerType(BLACK);

    aiLevels[WHITE] = dialog->getAILevel(WHITE);
    aiLevels[BLACK] = dialog->getAILevel(BLACK);

    delete dialog;

    bool ok = Game::instance()->newGame(fileName.toStdString(), playerTypes, aiLevels);

    if (ok == false) {
        QMessageBox(QMessageBox::Warning, tr("Error"), tr("File with rules is not valid"), QMessageBox::Ok).exec();
    }


}

void MainWindow::on_actionLoadGame_activated() {

    DialogNewGame* dialog = new DialogNewGame();
    dialog->setLoadFileText(tr("Opening file with board"), "saves", tr("Board in xml (*.xml)"));
    if (dialog->exec() != QDialog::Accepted) {
        delete dialog;
        return;
    }


    PlayerType playerTypes[2];
    int aiLevels[2];

    QString fileName = dialog->getFileName();

    playerTypes[WHITE] = dialog->getPlayerType(WHITE);
    playerTypes[BLACK] = dialog->getPlayerType(BLACK);

    aiLevels[WHITE] = dialog->getAILevel(WHITE);
    aiLevels[BLACK] = dialog->getAILevel(BLACK);

    delete dialog;

    bool ok = Game::instance()->loadGame(fileName.toStdString(), playerTypes, aiLevels);
    if (ok == false) {
        QMessageBox(QMessageBox::Warning, tr("Error"), tr("Can't load, because file with rules is not valid"), QMessageBox::Ok).exec();
    }
}

void MainWindow::on_actionExit_activated() {
    qDebug() << ":MainWindow: " << "on_actionExit_activated() ";

    saveLastGame();

    this->close();
}

bool MainWindow::restoreLastGame() {

    QSettings settings("Snowware", "Snowwchess");
    bool saved = settings.value("/Settings/LastGame/saved", "false").toBool();

    PlayerType playerTypes[2];
    int aiLevels[2];
   
    QString players[2];
    players[WHITE] = settings.value("/Settings/LastGame/Player/White", "HUMAN").toString();
    players[BLACK] = settings.value("/Settings/LastGame/Player/Black", "HUMAN").toString();

    aiLevels[WHITE] = settings.value("/Settings/LastGame/AI/WhiteLevel", "2").toInt();
    aiLevels[BLACK] = settings.value("/Settings/LastGame/AI/BlackLevel", "2").toInt();

    playerTypes[WHITE] = players[WHITE] == "HUMAN" ? HUMAN : AI;
    playerTypes[BLACK] = players[BLACK] == "HUMAN" ? HUMAN : AI;

    QString fileName = "saves/default.xml";
    
    Game::instance()->loadGame(fileName.toStdString(), playerTypes, aiLevels);

    return saved;
}


void MainWindow::saveLastGame() {
    Game::instance()->stopGameProcess();
    QSettings settings("Snowware", "Snowwchess");
    settings.setValue("/Settings/LastGame/saved", true);
    settings.setValue("/Settings/LastGame/AI/WhiteLevel", Game::instance()->getAILevel(WHITE));
    settings.setValue("/Settings/LastGame/AI/BlackLevel", Game::instance()->getAILevel(BLACK));
    QString whitePlayer = Game::instance()->getPlayerType(WHITE) == HUMAN ? "HUMAN" : "AI";
    QString blackPlayer = Game::instance()->getPlayerType(BLACK) == HUMAN ? "HUMAN" : "AI";
    settings.setValue("/Settings/LastGame/Player/White", whitePlayer);
    settings.setValue("/Settings/LastGame/Player/Black", blackPlayer);
    Game::instance()->saveGame("saves/default.xml");
}