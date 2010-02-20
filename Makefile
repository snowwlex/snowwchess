default: snowwchess

player.o: player.cpp player.h
	g++ -Wall -c player.cpp

model.o: model.cpp model.h
	g++ -Wall -c model.cpp

main.o: main.cpp model.h
	g++ -Wall -c main.cpp 

snowwchess: main.o model.o player.o
	g++ -Wall -o snowwchess -lncurses main.o model.o player.o

clean: 
	rm *.o snowwchess




