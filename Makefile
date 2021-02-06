



#tested on Linux anmsk 5.4.0-65-generic #73~18.04.1-Ubuntu SMP Tue Jan 19 09:02:24 UTC 2021 x86_64 x86_64 x86_64 GNU/Linux
#with g++-10(preferred) and g++-8.4.0(default compiler w/ std flag modified to gnu++2a)
LDLIBS += -lasan -lm -lcurl -lpthread

CXX     =   g++-10 
CC      =   g++-10

CXXFLAGS= -std=gnu++20 -fsanitize=address -W -Wall -fPIC -g -O2 -pedantic -I. -I./include/

main: main.o 
main.o: main.cpp

clean:
	rm -rf *.o *~ ./include/*~ main
flush:
	rm -rf *~ ./include/*~ main
