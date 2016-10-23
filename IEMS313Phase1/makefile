all : phase1

clean :
	rm *.o
	rm phase1

phase1 : main.o factory.o machine.o
	g++ main.o factory.o machine.o -o phase1
	
main.o : main.cpp machine.h factory.h
	g++ -c main.cpp
	
factory.o : factory.cpp factory.h machine.h
	g++ -c factory.cpp
	
machine.o : machine.cpp machine.h
	g++ -c machine.cpp