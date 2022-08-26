all: main

main: main.cpp definitions.cpp
	g++ -O2 --std=c++20 -I metal-cpp -framework Metal -framework QuartzCore -framework Foundation -o $@ $^

