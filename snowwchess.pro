TEMPLATE = app
TARGET = snowwchess

OBJECTS_DIR = bin
MOC_DIR = bin


QT += core \
    gui \
    xml
HEADERS += src/view/gui_history_view.h \
    src/view/cli_view.h \
    src/view/main_window.h \
    src/view/gui_board_view.h \
    src/view/dialog_new_game.h \
    src/player/alphabeta_search_ai_player.h \
    src/player/alphabeta_parallel_search_ai_player.h \
    src/player/full_search_ai_player.h \
    src/player/ai_player.h \
    src/player/human_player.h \
    src/model/io/rules_io.h \
    src/model/io/model_io.h \
    src/model/board.h \
    src/model/rules.h \
    src/model/model.h \
    src/player/player.h \
    src/model/io/model_io.h \
    src/tests/experiment.h \
    src/tests/time_test.h \
    src/tests/ai_speed_test.h \
    src/publisher.h \
    src/subscriber.h \
    src/model/game.h \
    src/model/history.h \
    src/snowwchess.h
SOURCES += src/view/gui_history_view.cpp \
    src/view/main_window.cpp \
    src/view/gui_board_view.cpp \
    src/view/dialog_new_game.cpp \
    src/player/player.cpp \
    src/player/human_player.cpp \
    src/player/full_search_ai_player.cpp \
    src/player/alphabeta_search_ai_player.cpp \
    src/player/alphabeta_parallel_search_ai_player.cpp \
    src/player/ai_player.cpp \
    src/model/io/rules_io.cpp \
    src/model/io/model_io.cpp \
    src/model/board.cpp \
    src/model/rules.cpp \
    src/model/model.cpp \
    src/main.cpp \
    src/model/history.cpp \
    src/model/game.cpp
RESOURCES += interface.qrc \
    pieces.qrc
