all:
	g++ fish-sim.pb.cc main.cpp Simulation.cpp PathFinder.cpp Flocking.cpp -std=c++0x -D__LINUX_COMPILE -lprotobuf

buffer:
	protoc fish-sim.proto --cpp_out=. --python_out=tubing/
