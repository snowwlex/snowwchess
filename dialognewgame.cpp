#include "dialognewgame.h"
#include <QFileDialog>

dialogNewGame::dialogNewGame(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);

}

dialogNewGame::~dialogNewGame()
{

}

QString dialogNewGame::getRulesFileName() {
	return ui.lineEditRulesFileName->text();
}


void dialogNewGame::on_buttonSelectFile_clicked() {
	QString fileName = QFileDialog::getOpenFileName(this,tr("Opening file with rules"), "", tr("Rules in xml (*.xml)"));
	ui.lineEditRulesFileName->setText(fileName);
	return;
}
