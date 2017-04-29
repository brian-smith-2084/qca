route : route.o pathweights.o vcg.o node.o net.o zone.o edge.o edgekeyed.o cyclebreaker.o doglegs.o
	g++ -W -g -o route route.o pathweights.o vcg.o node.o net.o zone.o edgekeyed.o cyclebreaker.o edge.o doglegs.o

route.o : route.cpp route.h net.h node.h vcg.h zone.h quicksort.h
	g++ -W -g -c route.cpp

pathweights.o : pathweights.cpp pathweights.h net.h node.h
	g++ -W -g -c pathweights.cpp

vcg.o : vcg.cpp vcg.h net.h node.h pathweights.h
	g++ -W -g -c vcg.cpp

node.o : node.cpp node.h net.h edge.h
	g++ -W -g -c node.cpp

edgekeyed.o : edgekeyed.cpp edgekeyed.h edge.h
	g++ -W -g -c edgekeyed.cpp

edge.o : edge.h edge.cpp
	g++ -W -g -c edge.cpp

net.o : net.cpp net.h node.h zone.h
	g++ -W -g -c net.cpp

zone.o: zone.cpp zone.h net.h
	g++ -W -g -c zone.cpp

cyclebreaker.o : cyclebreaker.h cyclebreaker.cpp
	g++ -W -g -c cyclebreaker.cpp

doglegs.o : doglegs.h doglegs.cpp net.h vcg.h
	g++ -W -g -c doglegs.cpp 
