GCC = g++ -std=c++14 -Wall -Wextra -g
GL = -lGLEW -lGL -lglfw 
LIBS = -L../lib/ -L/home/anl/lib/
INCLUDES = -I../include/ -I/home/anl/include/
ALL_OBJ = Globals.o Window.o Input.o Camera.o DataReader.o MapSegment.o Map.o ShaderProgram.o Program2D.o Program3D.o earth.o 

earth.out: $(ALL_OBJ)
	$(GCC) -o earth.out $(ALL_OBJ) $(GL) $(LIBS) -lcommon

#%.o : %.cpp 
#	$(GCC) -c  $(INCLUDES) $^.cpp

earth.o: earth.cpp  Globals.h  Window.h Input.h MapSegment.h Map.h
	$(GCC) -c  $(INCLUDES) earth.cpp

Globals.o: Globals.cpp Globals.h
	$(GCC) -c  $(INCLUDES) Globals.cpp

DataReader.o: DataReader.cpp DataReader.h
	$(GCC) -c  $(INCLUDES) DataReader.cpp

Window.o: Window.cpp Window.h
	$(GCC) -c  $(INCLUDES) Window.cpp

Input.o: Input.cpp Input.h Window.h
	$(GCC) -c  $(INCLUDES) Input.cpp

Camera.o: Camera.cpp Camera.h Input.h Window.h
	$(GCC) -c  $(INCLUDES) Camera.cpp

MapSegment.o: MapSegment.cpp MapSegment.h DataReader.h Window.h Program2D.h
	$(GCC) -c  $(INCLUDES) MapSegment.cpp

Map.o: Map.cpp Map.h
	$(GCC) -c  $(INCLUDES) Map.cpp

ShaderProgram.o: ShaderProgram.cpp ShaderProgram.h
	$(GCC) -c  $(INCLUDES) ShaderProgram.cpp

Program2D.o: Program2D.cpp Program2D.h ShaderProgram.h
	$(GCC) -c  $(INCLUDES) Program2D.cpp

Program3D.o: Program3D.cpp Program3D.h ShaderProgram.h
	$(GCC) -c  $(INCLUDES) Program3D.cpp

clean:
	rm -f *.o *.out