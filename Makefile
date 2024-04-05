CC = g++ -std=c++17 -lstdc++fs
CFLAGS = -I/usr/local/include/ -I/usr/include/
CFLAGS += -Wall -Wextra -Wpedantic \
          -Wformat=2 -Wno-unused-parameter -Wshadow \
          -Wwrite-strings -Wredundant-decls -Wmissing-include-dirs
CFLAGS += -O2
LDFLAGS = -L/usr/local/lib/ -L/usr/lib/x86_64-linux-gnu/ -lgd -ljpeg -lm -static
OBJECTS = main.o cImage.o cGrid.o cArgs.o

# GCC warnings that Clang doesn't provide:
ifeq ($(CC),gcc)
    CFLAGS += -Wjump-misses-init -Wlogical-op
endif


all: binary

main.o : main.cpp
	$(CC) $(CFLAGS) -c main.cpp -o main.o

cImage.o : cImage.cpp
	$(CC) $(CFLAGS) -c cImage.cpp -o cImage.o

cGrid.o : cGrid.cpp
	$(CC) $(CFLAGS) -c cGrid.cpp -o cGrid.o

cArgs.o : cArgs.cpp
	$(CC) $(CFLAGS) -c cArgs.cpp -o cArgs.o

binary: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o bin/sandpiles-cpp $(LDFLAGS)

clean:
	rm $(OBJECTS) 
	rm bin/*
	
	