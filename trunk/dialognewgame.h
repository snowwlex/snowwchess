#ifndef DIALOGNEWGAME_H
#define DIALOGNEWGAME_H

#include <QtGui/QDialog>
#include "ui_dialognewgame.h"

class dialogNewGame : public QDialog
{
    Q_OBJECT

public:
    dialogNewGame(QWidget *parent = 0);
    ~dialogNewGame();

public:
    QString getRulesFileName();

private:
    Ui::dialogNewGameClass ui;

private slots:
	void on_buttonSelectFile_clicked();

};

#endif // DIALOGNEWGAME_H
