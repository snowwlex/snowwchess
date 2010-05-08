#ifndef GUI_HISTORY_VIEW_H_
#define GUI_HISTORY_VIEW_H_

#include <QWidget>
#include <QListWidget>
#include <QPushButton>

#include "../snowwchess.h"
#include "../subscriber.h"
#include "../model/history.h"
#include "../model/game.h"

class GuiHistoryView : public QWidget, public Subscriber {
    Q_OBJECT

public:
    GuiHistoryView(QWidget *parent = 0);
    ~GuiHistoryView();

public:
    void setHistory(History* history);

public slots:
    void makeUndo();


public: //listener methods
    virtual void updateIt(); // history listener

public: //ui methods
    virtual QSize sizeHint() const;

private: //help method
 static QString moveToString(const Move& move);
 static QString makeComment(const Move& move);

private: //ui setting methods
    void setupUi();

private: //fields
    History* myHistory;


private: //ui
    QListWidget* listWidget;
    QPushButton* undoButton;

};


#endif /* GUI_HISTORY_VIEW_H_ */
