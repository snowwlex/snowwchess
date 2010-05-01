#ifndef DIALOGNEWGAME_H
#define DIALOGNEWGAME_H

#include <QtGui/QDialog>
#include <QString>
#include "../../ui_dialog_new_game.h"

#include "../snowwchess.h"

class DialogNewGame : public QDialog
{
    Q_OBJECT

public:
    DialogNewGame(QWidget *parent = 0);
    ~DialogNewGame();

public:
    QString getFileName();
    PlayerType getPlayerType(int color);
    int getAILevel(int color);

public:
    void setLoadFileText(QString caption, QString dir, QString filter);

private:
    Ui::DialogNewGameClass ui;
    QString myCaption, myDir, myFilter;

private slots:
	void on_buttonSelectFile_clicked();

};

#endif // DIALOGNEWGAME_H
