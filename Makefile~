all:
	g++ fish-sim.pb.cc main.cpp MapLoader.cpp Simulation.cpp PathFinder.cpp Flocking.cpp -std=c++0x -D__LINUX_COMPILE -lprotobuf -o flock-solve

buffer:
	protoc fish-sim.proto --cpp_out=. --python_out=tubing/

vdb_map_converter:
	g++ convertVDBtoMAP.cpp -std=c++0x -D__LINUX_COMPILE -lprotobuf -lopenvdb -lHalf -ltbb -o vdb_map_converter
