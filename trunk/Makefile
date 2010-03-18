CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		snowwchess.o rules.o model.o io.o rules_io.o model_io.o view.o game.o player.o ai.o

LIBS =		-lncurses -lexpat

TARGET =	snowwchess

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
	


	
