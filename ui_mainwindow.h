/********************************************************************************
** Form generated from reading ui file 'mainwindow.ui'
**
** Created: Sat Apr 10 14:50:09 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>
#include "guiboardview.h"

QT_BEGIN_NAMESPACE

class Ui_mainWindowClass
{
public:
    QAction *actionNewGame;
    QAction *actionSaveGame;
    QAction *actionLoadGame;
    QAction *actionExit;
    QWidget *centralwidget;
    GuiBoardView *widget;
    QMenuBar *menubar;
    QMenu *menu;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *mainWindowClass)
    {
        if (mainWindowClass->objectName().isEmpty())
            mainWindowClass->setObjectName(QString::fromUtf8("mainWindowClass"));
        mainWindowClass->resize(800, 600);
        actionNewGame = new QAction(mainWindowClass);
        actionNewGame->setObjectName(QString::fromUtf8("actionNewGame"));
        actionSaveGame = new QAction(mainWindowClass);
        actionSaveGame->setObjectName(QString::fromUtf8("actionSaveGame"));
        actionLoadGame = new QAction(mainWindowClass);
        actionLoadGame->setObjectName(QString::fromUtf8("actionLoadGame"));
        actionExit = new QAction(mainWindowClass);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        centralwidget = new QWidget(mainWindowClass);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        widget = new GuiBoardView(centralwidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(70, 40, 331, 351));
        mainWindowClass->setCentralWidget(centralwidget);
        menubar = new QMenuBar(mainWindowClass);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 23));
        menu = new QMenu(menubar);
        menu->setObjectName(QString::fromUtf8("menu"));
        mainWindowClass->setMenuBar(menubar);
        statusbar = new QStatusBar(mainWindowClass);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        mainWindowClass->setStatusBar(statusbar);

        menubar->addAction(menu->menuAction());
        menu->addAction(actionNewGame);
        menu->addSeparator();
        menu->addAction(actionSaveGame);
        menu->addAction(actionLoadGame);
        menu->addSeparator();
        menu->addAction(actionExit);

        retranslateUi(mainWindowClass);
        QObject::connect(actionExit, SIGNAL(activated()), mainWindowClass, SLOT(close()));

        QMetaObject::connectSlotsByName(mainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *mainWindowClass)
    {
        mainWindowClass->setWindowTitle(QApplication::translate("mainWindowClass", "SnowwChess", 0, QApplication::UnicodeUTF8));
        actionNewGame->setText(QApplication::translate("mainWindowClass", "New game", 0, QApplication::UnicodeUTF8));
        actionNewGame->setShortcut(QApplication::translate("mainWindowClass", "Ctrl+N", 0, QApplication::UnicodeUTF8));
        actionSaveGame->setText(QApplication::translate("mainWindowClass", "Save", 0, QApplication::UnicodeUTF8));
        actionSaveGame->setShortcut(QApplication::translate("mainWindowClass", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        actionLoadGame->setText(QApplication::translate("mainWindowClass", "Load", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("mainWindowClass", "Exit", 0, QApplication::UnicodeUTF8));
        menu->setTitle(QApplication::translate("mainWindowClass", "Game", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class mainWindowClass: public Ui_mainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
