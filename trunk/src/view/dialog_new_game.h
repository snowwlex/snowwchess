#ifndef DIALOGNEWGAME_H
#define DIALOGNEWGAME_H


#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QString>


#include "../snowwchess.h"

class DialogNewGame : public QDialog {
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
     void setWhatLoadingText(QString text);

public slots:
    void whitePlayerChanged(const QString& string);
    void blackPlayerChanged(const QString& string);
private:
    QString myCaption, myDir, myFilter;

private slots:
    void selectFile();

private: //ui setup methods
    void setupUi();
    void retranslateUi();

private: //ui

    QPushButton *buttonSelectFile;
    QLineEdit *lineEditFileName;
    QComboBox *cbPlayerWhite;
    QComboBox *cbPlayerBlack;
    QComboBox *cbLevelWhite;
    QComboBox *cbLevelBlack;
    QDialogButtonBox* buttonBox;
    QLabel* lblWhatLoading;



};

#endif // DIALOGNEWGAME_H
