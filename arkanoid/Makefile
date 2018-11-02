GCC = g++ -std=c++11 -Wall -Wextra -g
GL = -lGLEW -lGL -lglfw 
LIBS = -L../lib/ -L/home/anl/lib/
INCLUDES = -I../include/ -I/home/anl/include/
ALL_OBJ = Shapes.o Mesh.o Actor.o arkanoid.o 

arkanoid.out: $(ALL_OBJ)
	$(GCC) -o arkanoid.out $(ALL_OBJ) $(GL) $(LIBS) -lcommon

arkanoid.o: arkanoid.cpp Actor.h Mesh.h Shapes.h
	$(GCC) -c  $(INCLUDES) arkanoid.cpp

Actor.o: Actor.cpp Actor.h Mesh.h
	$(GCC) -c  $(INCLUDES) Actor.cpp

Mesh.o: Mesh.cpp Mesh.h
	$(GCC) -c  $(INCLUDES) Mesh.cpp

Shapes.o: Shapes.cpp Shapes.h Actor.h
	$(GCC) -c  $(INCLUDES) Shapes.cpp

clean:
	rm -f *.o *.out

	
