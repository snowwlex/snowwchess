TEMPLATE = app
TARGET = snowwchess
QT += core \
    gui \
    xml
HEADERS += sender.h \
    listener.h \
    guiboardview.h \
    tester.h \
    dialognewgame.h \
    ai.h \
    experiment.h \
    exprunner.h \
    game.h \
    history.h \
    io.h \
    model.h \
    model_io.h \
    player.h \
    rules.h \
    rules_io.h \
    snowwchess.h \
    view.h \
    mainwindow.h
SOURCES += guiboardview.cpp \
    dialognewgame.cpp \
    mainwindow.cpp \
    main.cpp \
    rules_io.cpp \
    rules.cpp \
    model.cpp \
    game.cpp \
    player.cpp
FORMS += dialognewgame.ui \
    mainwindow.ui
RESOURCES += pieces.qrc
