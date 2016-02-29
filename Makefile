all:
	g++ display.cpp Boid.cpp Flocking.cpp Simulation.cpp PathFinder.cpp MapLoader.cpp -std=c++0x -D__LINUX_COMPILE -o flock-solve -lopenvdb -lHalf -ltbb -lGL -lglut -lGLU

buffer:
	protoc fish-sim.proto --cpp_out=. --python_out=tubing/

vdb_map_converter:
	g++ convertVDBtoMAP.cpp -std=c++0x -D__LINUX_COMPILE -lprotobuf -lopenvdb -lHalf -ltbb -o vdb_map_converter

vdb_map_converter:
	g++ convertVDBtoMAP.cpp -std=c++0x -D__LINUX_COMPILE -lprotobuf -lopenvdb -lHalf -ltbb -o vdb_map_converter


metaVDB:
	g++ insertMetaVDB.cpp -std=c++0x -D__LINUX_COMPILE -lprotobuf -lopenvdb -lHalf -ltbb -o insertMetaVDB

