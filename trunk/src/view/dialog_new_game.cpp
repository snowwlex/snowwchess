#include <QFileDialog>
#include <QLabel>
#include <QString>
#include <qt4/QtGui/qgridlayout.h>

#include "dialog_new_game.h"

DialogNewGame::DialogNewGame(QWidget *parent)
: QDialog(parent) {
    this->setupUi();

    QStringList players;
    players << "Human" << "AI";
    this->cbPlayerWhite->addItems(players);
    this->cbPlayerBlack->addItems(players);
    QStringList level;
    level << "Low" << "Middle" << "Hard";
    this->cbLevelWhite->addItems(level);
    this->cbLevelBlack->addItems(level);


    this->cbLevelWhite->setEnabled(false);
    this->cbLevelBlack->setEnabled(false);

}

DialogNewGame::~DialogNewGame() {

}

void DialogNewGame::setupUi() {
    QLabel* lbl;

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->setSpacing(15);
    gridLayout->setMargin(5);
    this->setLayout(gridLayout);


    // RULES SELECTION

    lbl = new QLabel("<b>Select file with rules:</b>");
    gridLayout->addWidget(lbl, 0, 0, 1, 2, Qt::AlignCenter);

    lineEditFileName = new QLineEdit;
    lineEditFileName->setReadOnly(true);
    gridLayout->addWidget(lineEditFileName, 1, 0);

    buttonSelectFile = new QPushButton;
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/images/interface/open.png"), QSize(), QIcon::Normal, QIcon::Off);
    buttonSelectFile->setIcon(icon);
    gridLayout->addWidget(buttonSelectFile, 1, 1);


    // PLAYERS
    lbl = new QLabel("<b>Players:</b>");
    gridLayout->addWidget(lbl, 2, 0, 1, 2, Qt::AlignCenter);

    lbl = new QLabel("<b>White</b>");
    cbPlayerWhite = new QComboBox;
    gridLayout->addWidget(lbl, 3, 0);
    gridLayout->addWidget(cbPlayerWhite, 3, 1);


    lbl = new QLabel("<b>Black</b>");
    cbPlayerBlack = new QComboBox;
    gridLayout->addWidget(lbl, 4, 0);
    gridLayout->addWidget(cbPlayerBlack, 4, 1);


    // AI LEVEL
    lbl = new QLabel("<b>Choose AI Level:</b>");
    gridLayout->addWidget(lbl, 5, 0, 1, 2, Qt::AlignCenter);

    lbl = new QLabel("<b>White</b>");
    cbLevelWhite = new QComboBox;

    gridLayout->addWidget(lbl, 6, 0);
    gridLayout->addWidget(cbLevelWhite, 6, 1);

    lbl = new QLabel("<b>Black</b>");
    cbLevelBlack = new QComboBox;

    gridLayout->addWidget(lbl, 7, 0);
    gridLayout->addWidget(cbLevelBlack, 7, 1);


    // OK AND CANCEL

    buttonBox = new QDialogButtonBox();
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);


    gridLayout->addWidget(buttonBox, 8, 0, 1, 2, Qt::AlignCenter);

    retranslateUi();

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(buttonSelectFile, SIGNAL(clicked()), this, SLOT(selectFile()));

    connect(cbPlayerWhite, SIGNAL(activated(const QString&)), this, SLOT(whitePlayerChanged(const QString&)));
    connect(cbPlayerBlack, SIGNAL(activated(const QString&)), this, SLOT(blackPlayerChanged(const QString&)));

}

void DialogNewGame::retranslateUi() {
    this->setWindowTitle(QApplication::translate("DialogNewGameClass", "New game", 0, QApplication::UnicodeUTF8));
    buttonSelectFile->setText(QString());
}

PlayerType DialogNewGame::getPlayerType(int color) {
    QString player;
    if (color == WHITE) {
        player = this->cbPlayerWhite->currentText();
    } else {
        player = this->cbPlayerBlack->currentText();
    }

    if (player == "Human") {
        return HUMAN;
    }
    return AI;
}

int DialogNewGame::getAILevel(int color) {
    int level;
    if (color == WHITE) {
        level = this->cbLevelWhite->currentIndex();
    } else {
        level = this->cbLevelBlack->currentIndex();
    }
    return level;
}

QString DialogNewGame::getFileName() {
    return this->lineEditFileName->text();
}

void DialogNewGame::setLoadFileText(QString caption, QString dir, QString filter) {
    myCaption = caption;
    myDir = dir;
    myFilter = filter;
}

void DialogNewGame::selectFile() {
    QString fileName = QFileDialog::getOpenFileName(this, myCaption, myDir, myFilter);
    this->lineEditFileName->setText(fileName);
    return;
}

void DialogNewGame::whitePlayerChanged(const QString& string) {
    if (string == "AI") {
        this->cbLevelWhite->setEnabled(true);
    } else {
        this->cbLevelWhite->setEnabled(false);
    }
}

void DialogNewGame::blackPlayerChanged(const QString& string) {

    if (string == "AI") {
        this->cbLevelBlack->setEnabled(true);
    } else {
        this->cbLevelBlack->setEnabled(false);
    }

}