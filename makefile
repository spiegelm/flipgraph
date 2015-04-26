CFLAGS = -O
CC     = g++
SRC    = main.cpp fg/options.cpp fg/flipgraph.cpp fg/triangulation.cpp
OBJ    = $(SRC:.cpp = .o)

all: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o main

clean:
	rm *.o