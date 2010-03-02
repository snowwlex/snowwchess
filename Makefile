CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		main.o rules.o model.o io.o rules_io.o model_io.o view.o game.o player.o

LIBS =		-lncurses -lexpat

TARGET =	snowwchess

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
	


	