all: fastest-cpp-input-output

.PHONY: all clean

fastest-cpp-input-output: main.cpp
	g++ -O2 -std=c++11 $^ -o $@

clean:
	rm -f fastest-cpp-input-output
