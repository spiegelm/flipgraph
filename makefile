CC       = g++
CC_FLAGS = -O3 -std=c++11
MAIN     = main.cpp
SRC      = $(wildcard src/*.cpp)
OBJ      = $(addprefix obj/,$(notdir $(SRC:.cpp=.o)))

flipgraph: $(MAIN) $(OBJ)
	$(CC) $(CC_FLAGS) $^ -o $@

obj/%.o: src/%.cpp
	$(CC) $(CC_FLAGS) -c $< -o $@

clean:
	rm obj/*.o
