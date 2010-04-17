#include "dialognewgame.h"
#include <QFileDialog>

DialogNewGame::DialogNewGame(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);

}

DialogNewGame::~DialogNewGame()
{

}

QString DialogNewGame::getRulesFileName() {
	return ui.lineEditRulesFileName->text();
}


void DialogNewGame::on_buttonSelectFile_clicked() {
	QString fileName = QFileDialog::getOpenFileName(this,tr("Opening file with rules"), "", tr("Rules in xml (*.xml)"));
	ui.lineEditRulesFileName->setText(fileName);
	return;
}
