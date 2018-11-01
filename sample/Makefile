default: sample.out

%.o: %.cpp
	g++ -c -I ../include/  $<

%.out: %.o
	g++ $<  -o  $@   -lGLEW -lGL -lglfw   -L ../lib -lcommon

clean:
	rm *.o *.out *~
