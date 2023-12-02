CXX = g++
CXXFLAGS = -std=c++11 -Wall
SRC = tp2.cpp Graph.cpp
OBJ = $(SRC:.cpp=.o)
EXECUTABLE = tp2

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean run

clean:
	rm -f $(OBJ) $(EXECUTABLE)

run: $(EXECUTABLE)
	./$(EXECUTABLE)
