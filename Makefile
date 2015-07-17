all:
	g++ main.cpp Simulation.cpp PathFinder.cpp Flocking.cpp -std=c++0x -D__LINUX_COMPILE

buffer:
	protoc fish-sim.proto --cpp_out=. --python_out=tubing/
