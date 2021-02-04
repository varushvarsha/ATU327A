



LDLIBS += -lasan -lm -lcurl -lpthread
#LDLIBS += -fsanitize=thread -lm -lcurl -lpthread 
# CXX     =   clang++-10
# CC      =   clang++-10
CXX     =   g++-10
CC      =   g++-10

CXXFLAGS= -std=gnu++20 -fsanitize=address -W -Wall -fPIC -g -O2 -pedantic -I. -I./include/

main: main.o 
main.o: main.cpp

clean:
	rm -rf *.o *~ main
flush:
	rm -rf *~ main
