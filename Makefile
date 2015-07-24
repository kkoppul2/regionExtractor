CXXFLAGS = -Wall -Werror -Wno-write-strings -std=c++11 -pedantic-errors
CXXFLAGS += -g
CXXFLAGS += $(shell pkg-config --cflags opencv)
LIBS += $(shell pkg-config --libs opencv)

regionExtractor: regionExtractor.o
	$(CXX) -o bin/regionExtractor regionExtractor.o $(CXXFLAGS) $(LIBS) 

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f *.o
	rm -f bin/regionExtractor