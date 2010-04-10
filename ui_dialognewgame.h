/********************************************************************************
** Form generated from reading ui file 'dialognewgame.ui'
**
** Created: Sat Apr 10 14:50:09 2010
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_DIALOGNEWGAME_H
#define UI_DIALOGNEWGAME_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QFormLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_dialogNewGameClass
{
public:
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLineEdit *lineEditRulesFileName;
    QPushButton *buttonSelectFile;
    QLabel *labelRulesFile;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *dialogNewGameClass)
    {
        if (dialogNewGameClass->objectName().isEmpty())
            dialogNewGameClass->setObjectName(QString::fromUtf8("dialogNewGameClass"));
        dialogNewGameClass->resize(400, 300);
        formLayoutWidget = new QWidget(dialogNewGameClass);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(0, 0, 391, 291));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setSpacing(6);
        formLayout->setMargin(11);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        lineEditRulesFileName = new QLineEdit(formLayoutWidget);
        lineEditRulesFileName->setObjectName(QString::fromUtf8("lineEditRulesFileName"));
        lineEditRulesFileName->setReadOnly(true);

        formLayout->setWidget(2, QFormLayout::FieldRole, lineEditRulesFileName);

        buttonSelectFile = new QPushButton(formLayoutWidget);
        buttonSelectFile->setObjectName(QString::fromUtf8("buttonSelectFile"));

        formLayout->setWidget(1, QFormLayout::FieldRole, buttonSelectFile);

        labelRulesFile = new QLabel(formLayoutWidget);
        labelRulesFile->setObjectName(QString::fromUtf8("labelRulesFile"));

        formLayout->setWidget(1, QFormLayout::LabelRole, labelRulesFile);

        buttonBox = new QDialogButtonBox(formLayoutWidget);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(false);

        formLayout->setWidget(3, QFormLayout::FieldRole, buttonBox);


        retranslateUi(dialogNewGameClass);
        QObject::connect(buttonBox, SIGNAL(accepted()), dialogNewGameClass, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), dialogNewGameClass, SLOT(reject()));

        QMetaObject::connectSlotsByName(dialogNewGameClass);
    } // setupUi

    void retranslateUi(QDialog *dialogNewGameClass)
    {
        dialogNewGameClass->setWindowTitle(QApplication::translate("dialogNewGameClass", "dialogNewGame", 0, QApplication::UnicodeUTF8));
        buttonSelectFile->setText(QApplication::translate("dialogNewGameClass", "Open file", 0, QApplication::UnicodeUTF8));
        labelRulesFile->setText(QApplication::translate("dialogNewGameClass", "File with rules", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(dialogNewGameClass);
    } // retranslateUi

};

namespace Ui {
    class dialogNewGameClass: public Ui_dialogNewGameClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGNEWGAME_H
