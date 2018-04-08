myserver: server.o functions.o User.o
	g++ -std=c++11 server.o functions.o User.o -o myserver

server.o: server.cpp
	g++ -std=c++11 -c server.cpp

functions.o: functions.cpp functions.h
	g++ -std=c++11 -c functions.cpp

User.o: User.cpp User.h
	g++ -std=c++11 -c User.cpp

clean:
	rm *.o myserver
