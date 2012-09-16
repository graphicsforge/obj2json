
CPP = g++
CPP_FLAGS = -g -D_REENTRANT -march=native -msse3 -Wall

default: objConverter clean

clean:
	- rm -f *.o

objConverter: ObjConverter.o Vertex.o
	$(CPP)  \
   -o $@ $^

ObjConverter.o : ObjConverter.cpp
	$(CPP) $(CPP_FLAGS)    \
		-c $<

Vertex.o : Vertex.cpp
	$(CPP) $(CPP_FLAGS)    \
		-c $<
