#ifndef SNOWWCHESS_H
#define SNOWWCHESS_H

#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>

#include "dialog_new_game.h"
#include "../model/game.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void on_actionNewGame_activated();
    void on_actionSaveGame_activated();
    void on_actionLoadGame_activated();
    void on_actionExit_activated();

private slots:

signals:

private:
    void loadGame(DialogNewGame* dialog);
    void newGame(DialogNewGame* dialog);
    void saveLastGame();
    bool restoreLastGame();

private:
    void setupUi();
    void retranslateUi();

private: //fields
    
private: //ui

    QAction *actionNewGame;
    QAction *actionSaveGame;
    QAction *actionLoadGame;
    QAction *actionExit;
    GuiBoardView *boardView;
    GuiHistoryView *historyView;
    QMenu* menu;





};

#endif // SNOWWCHESS_H
