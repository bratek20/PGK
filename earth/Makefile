GCC = g++ -std=c++14 -Wall -Wextra -g
GL = -lGLEW -lGL -lglfw 
LIBS = -L../lib/ -L/home/anl/lib/
INCLUDES = -I../include/ -I/home/anl/include/
ALL_OBJ = Globals.o Color.o Window.o Input.o Mesh.o Collider.o Actor.o Camera.o Light.o Player.o Scene.o DataReader.o MapSegment.o earth.o 

earth.out: $(ALL_OBJ)
	$(GCC) -o earth.out $(ALL_OBJ) $(GL) $(LIBS) -lcommon

earth.o: earth.cpp  Globals.h  Window.h Input.h Mesh.h Player.h Color.h Scene.h MapSegment.h
	$(GCC) -c  $(INCLUDES) earth.cpp

Globals.o: Globals.cpp Globals.h
	$(GCC) -c  $(INCLUDES) Globals.cpp

DataReader.o: DataReader.cpp DataReader.h
	$(GCC) -c  $(INCLUDES) DataReader.cpp

Color.o: Color.cpp Color.h Globals.h
	$(GCC) -c  $(INCLUDES) Color.cpp

Window.o: Window.cpp Window.h
	$(GCC) -c  $(INCLUDES) Window.cpp

Input.o: Input.cpp Input.h Window.h
	$(GCC) -c  $(INCLUDES) Input.cpp

Mesh.o: Mesh.cpp Mesh.h Color.h
	$(GCC) -c  $(INCLUDES) Mesh.cpp

Collider.o: Collider.cpp Collider.h
	$(GCC) -c  $(INCLUDES) Collider.cpp

Actor.o: Actor.cpp Actor.h Mesh.h Collider.h
	$(GCC) -c  $(INCLUDES) Actor.cpp

Camera.o: Camera.cpp Camera.h Actor.h
	$(GCC) -c  $(INCLUDES) Camera.cpp

Light.o: Light.cpp Light.h Actor.h Color.h
	$(GCC) -c  $(INCLUDES) Light.cpp

Player.o: Player.cpp Player.h Actor.h Light.h Globals.h Input.h
	$(GCC) -c  $(INCLUDES) Player.cpp

Scene.o: Scene.cpp Scene.h Camera.h Light.h Actor.h Color.h
	$(GCC) -c  $(INCLUDES) Scene.cpp

MapSegment.o: MapSegment.cpp MapSegment.h DataReader.h
	$(GCC) -c  $(INCLUDES) MapSegment.cpp

clean:
	rm -f *.o *.out