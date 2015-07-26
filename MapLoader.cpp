#include "MapLoader.h"
#include "Vector.h"
#include <cstring>
#include <openvdb/openvdb.h>
#include <stdlib.h>
#include <fstream>
using namespace std;


bool**
MapLoader::loadVDBMap(char* filename)
{
	openvdb::initialize();

	// Create a VDB file object.
	openvdb::io::File file(filename);

	// Open the file. This reads the file header, but not any grids.
	file.open();

	openvdb::GridBase::Ptr baseGrid;
	baseGrid = file.readGrid("SDF");
	file.close();

	//cast the generic grid pointer to a FloatGrid pointer.
	openvdb::FloatGrid::Ptr grid = openvdb::gridPtrCast<openvdb::FloatGrid>(baseGrid);

    string boundx = grid->metaValue<string>("boundx");
    string boundy = grid->metaValue<string>("boundy");
    string startx = grid->metaValue<string>("startx");
    string starty = grid->metaValue<string>("starty");
    string start_radius = grid->metaValue<string>("start_radius");
    string endx = grid->metaValue<string>("endx");
    string endy = grid->metaValue<string>("endy");
    string end_radius = grid->metaValue<string>("end_radius");

    numCols = stoi(boundx);
    numRows = stoi(boundy);

    startRadius = stof(start_radius);
    endRadius = stof(end_radius);

  	int x_bound = numCols;
	int y_bound = numRows;
    int z_bound = 0;

    // Create the map data
    mapData = (bool**)malloc(sizeof(bool*) * numRows);
    grid_array = (float**)malloc(sizeof(float*) * numRows);
    for(int i=0;i<numRows;i++) {
    	grid_array[i] = (float*)malloc(sizeof(float) * numCols);
        mapData[i] = (bool*)malloc(sizeof(bool) * numCols);
        memset(mapData[i], true, numCols);
        memset(grid_array[i],-999,numCols);
    }

    for (openvdb::FloatGrid::ValueOnCIter iter = grid->cbeginValueOn(); iter; ++iter) {
		openvdb::Coord pos = iter.getCoord();
		if((pos.x()>=x_bound||pos.y()>=y_bound||pos.x()<0||pos.y()<0)&&(pos.z()!=z_bound)) {
			continue;
		}

		grid_array[pos.x()][pos.y()]=*iter;
		if(*iter<=0)
			mapData[pos.x()][pos.y()]=false;
	}


    // TODO Figure out how to get start and end positions
    posStart = math::Vec2f(stof(startx), stof(starty));
    posEnd = math::Vec2f(stof(endx),stof(endy));
    loaded = true;
    return mapData;
}

float**
MapLoader::getSDF(){
	return grid_array;
}

MapLoader::MapLoader() {
    this->loaded = false;
    this->mapData = nullptr;
}

Vec2f
MapLoader::getStartPosition() {
    return posStart;
}

Vec2f
MapLoader::getEndPosition() {
    return posEnd;
}

unsigned int
MapLoader::getNumRows() {
    return numRows;
}

unsigned int
MapLoader::getNumCols() {
    return numCols;
}

unsigned int
MapLoader::getStartRadius() {
    return startRadius;
}

unsigned int
MapLoader::getEndRadius() {
    return endRadius;
}
