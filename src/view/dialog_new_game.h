#ifndef DIALOGNEWGAME_H
#define DIALOGNEWGAME_H

#include <QtGui/QDialog>
#include <QString>
#include "../ui_dialog_new_game.h"

class DialogNewGame : public QDialog
{
    Q_OBJECT

public:
    DialogNewGame(QWidget *parent = 0);
    ~DialogNewGame();

public:
    QString getRulesFileName();

private:
    Ui::DialogNewGameClass ui;

private slots:
	void on_buttonSelectFile_clicked();

};

#endif // DIALOGNEWGAME_H
