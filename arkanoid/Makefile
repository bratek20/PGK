GCC = g++ -std=c++11 -Wall -Wextra
GL = -lGLEW -lGL -lglfw 

arkanoid.out: arkanoid.o
	$(GCC) -o arkanoid.out arkanoid.o $(GL) -L../lib/ -L/home/anl/lib/ -lcommon

arkanoid.o: arkanoid.cpp
	$(GCC) -c -I../include/ -I/home/anl/include/  arkanoid.cpp

clean:
	rm -f *.o *.out

	
