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
SOURCES +=  ai_full_search.cpp \
    ai_ab_search.cpp \ 
    ai_parallel_search.cpp \
    guiboardview.cpp \
    dialognewgame.cpp \
    mainwindow.cpp \
    main.cpp \
    rules_io.cpp \
    rules.cpp \
    model.cpp \
    game.cpp \
    player.cpp \
    ai.cpp
FORMS += dialognewgame.ui \
    mainwindow.ui
RESOURCES += pieces.qrc
