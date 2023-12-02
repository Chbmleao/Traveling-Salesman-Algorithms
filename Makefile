CXX = g++
CXXFLAGS = -std=c++11 -Wall
SRC = tp2.cpp Graph.cpp
OBJ = $(addprefix bin/,$(SRC:.cpp=.o))
EXECUTABLE = bin/tp2
TIMEOUT = 1800

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@

bin/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean run

clean:
	rm -rf bin $(EXECUTABLE)

run: $(EXECUTABLE)
	timeout $(TIMEOUT) ./$(EXECUTABLE)
