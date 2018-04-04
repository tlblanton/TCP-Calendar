calendarserver: server.o functions.o
	g++ -std=c++11 server.o functions.o -o calendarserver

server.o: server.cpp
	g++ -std=c++11 -c server.cpp

DAL.o: DAL.cpp DAL.h
	g++ -std=c++11 -c functions.cpp

clean:
	rm *.o calendarserver
