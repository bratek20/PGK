GCC = g++ -std=c++14 -Wall -Wextra -g
GL = -lGLEW -lGL -lglfw 
LIBS = -L../lib/ -L/home/anl/lib/
INCLUDES = -I../include/ -I/home/anl/include/
ALL_OBJ = Globals.o Window.o Input.o Mesh.o Collider.o Actor.o aquarium.o 

aquarium.out: $(ALL_OBJ)
	$(GCC) -o aquarium.out $(ALL_OBJ) $(GL) $(LIBS) -lcommon

aquarium.o: aquarium.cpp  Globals.h  Window.h Mesh.h
	$(GCC) -c  $(INCLUDES) aquarium.cpp

Globals.o: Globals.cpp Globals.h
	$(GCC) -c  $(INCLUDES) Globals.cpp

Window.o: Window.cpp Window.h
	$(GCC) -c  $(INCLUDES) Window.cpp

Input.o: Input.cpp Input.h Window.h
	$(GCC) -c  $(INCLUDES) Input.cpp

Mesh.o: Mesh.cpp Mesh.h
	$(GCC) -c  $(INCLUDES) Mesh.cpp

Collider.o: Collider.cpp Collider.h
	$(GCC) -c  $(INCLUDES) Collider.cpp

Actor.o: Actor.cpp Actor.h Mesh.h Collider.h
	$(GCC) -c  $(INCLUDES) Actor.cpp

clean:
	rm -f *.o *.out

	
