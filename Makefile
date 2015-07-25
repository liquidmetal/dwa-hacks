all:
	g++ display.cpp Boid.cpp Flocking.cpp Simulation.cpp PathFinder.cpp MapLoader.cpp fish-sim.pb.cc -std=c++0x -D__LINUX_COMPILE -lprotobuf -o flock-solve -lopenvdb -lHalf -ltbb -lGL -lglut -lGLU

buffer:
	protoc fish-sim.proto --cpp_out=. --python_out=tubing/

vdb_map_converter:
	g++ convertVDBtoMAP.cpp -std=c++0x -D__LINUX_COMPILE -lprotobuf -lopenvdb -lHalf -ltbb -o vdb_map_converter
