GCC = g++ -std=c++11 -Wall -Wextra -g
GL = -lGLEW -lGL -lglfw 
LIBS = -L../lib/ -L/home/anl/lib/
INCLUDES = -I../include/ -I/home/anl/include/
ALL_OBJ = Globals.o Mesh.o Actor.o Ball.o Shapes.o arkanoid.o 

arkanoid.out: $(ALL_OBJ)
	$(GCC) -o arkanoid.out $(ALL_OBJ) $(GL) $(LIBS) -lcommon

arkanoid.o: arkanoid.cpp  Globals.h  Mesh.h Actor.h Ball.h Shapes.h 
	$(GCC) -c  $(INCLUDES) arkanoid.cpp

Globals.o: Globals.cpp Globals.h
	$(GCC) -c  $(INCLUDES) Globals.cpp

Mesh.o: Mesh.cpp Mesh.h
	$(GCC) -c  $(INCLUDES) Mesh.cpp

Actor.o: Actor.cpp Actor.h Mesh.h
	$(GCC) -c  $(INCLUDES) Actor.cpp

Ball.o: Ball.cpp Ball.h Actor.h
	$(GCC) -c  $(INCLUDES) Ball.cpp

Shapes.o: Shapes.cpp Shapes.h Actor.h
	$(GCC) -c  $(INCLUDES) Shapes.cpp

clean:
	rm -f *.o *.out

	
