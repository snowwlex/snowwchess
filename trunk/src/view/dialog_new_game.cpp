#include <QFileDialog>
#include <QString>

#include "dialog_new_game.h"


DialogNewGame::DialogNewGame(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);
	QStringList players;
	players << "Human" << "AI";
	ui.cbPlayerWhite->addItems(players);
	ui.cbPlayerBlack->addItems(players);
	QStringList level;
	level << "Low" << "Middle" << "Hard";
	ui.cbLevelWhite->addItems(level);
	ui.cbLevelBlack->addItems(level);
}

DialogNewGame::~DialogNewGame()
{

}

PlayerType DialogNewGame::getPlayerType(int color) {
	QString player;
	if (color == WHITE) {
		player = ui.cbPlayerWhite->currentText();
	} else {
		player = ui.cbPlayerBlack->currentText();
	}

	if (player == "Human") {
		return HUMAN;
	}
	return AI;
}

int DialogNewGame::getAILevel(int color) {
	int level;
	if (color == WHITE) {
		level = ui.cbLevelWhite->currentIndex();
	} else {
		level = ui.cbLevelBlack->currentIndex();
	}
	return level;
}

QString DialogNewGame::getFileName() {
	return ui.lineEditFileName->text();
}

void DialogNewGame::setLoadFileText(QString caption, QString dir, QString filter) {
	myCaption = caption;
	myDir = dir;
	myFilter = filter;
}

void DialogNewGame::on_buttonSelectFile_clicked() {
	QString fileName = QFileDialog::getOpenFileName(this,myCaption, myDir, myFilter);
	ui.lineEditFileName->setText(fileName);
	return;
}
