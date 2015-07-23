#include "MapLoader.h"
#include "Vector.h"
#include <cstring>
#include <openvdb/openvdb.h>
#include <stdlib.h>
#include <fstream>
using namespace std;

bool**
MapLoader::loadMap(char* filename)
{
    FILE *fp = fopen(filename, "r");
    fseek(fp, 0, SEEK_END);
    long fileLength = ftell(fp);

    // Rewind to the beginning
    fseek(fp, 0, SEEK_SET);

    char line[1024];

    fgets(line, 1024, fp);
    numCols = strlen(line);
    numRows = fileLength / (numCols);  // Add 1 to compensate for \n
    numCols -= 1;

    // Create the map data
    mapData = (bool**)malloc(sizeof(bool*) * numRows);
    for(int i=0;i<numRows;i++) {
        mapData[i] = (bool*)malloc(sizeof(bool) * numCols);
        memset(mapData[i], false, numCols);
    }

    // Actually read the map
    long currentRow = 0;
    do {
        for(int i=0;i<numCols;i++) {
            if(line[i] == '.') {
                // Mark it passable
                mapData[currentRow][i] = true;
            } else {
                // Let it remain impassable (from memset)
            }
        }
        currentRow += 1;
    } while(fgets(line, 1024, fp));

    // TODO Figure out how to get start and end positions
    posStart = math::Vec2d(0, 0);
    posEnd = math::Vec2d(numCols-1, numRows-1);
    loaded = true;
    return mapData;
}

bool**
MapLoader::loadVDBMap(char* filename)
{
	openvdb::initialize();

	// Create a VDB file object.
	openvdb::io::File file(filename);

	// Open the file. This reads the file header, but not any grids.
	file.open();

	// Loop over all grids in the file and retrieve a shared pointer
	// to the one named "LevelSetSphere". (This can also be done
	// more simply by calling file.readGrid("LevelSetSphere").)
	openvdb::GridBase::Ptr baseGrid;
	baseGrid = file.readGrid("SDF");
	file.close();

	//cast the generic grid pointer to a FloatGrid pointer.
	openvdb::FloatGrid::Ptr grid = openvdb::gridPtrCast<openvdb::FloatGrid>(baseGrid);

    // TODO Is this a constant?
    numCols = 100;
    numRows = 100;
    
  	int x_bound = numRows;
	int y_bound = 1;
	int z_bound = numCols;


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
		if((pos.x()>=x_bound||pos.z()>=z_bound||pos.x()<0||pos.z()<0)&&(pos.y()!=y_bound)) {
			continue;
		}

		grid_array[pos.x()][pos.z()]=*iter;
		if(*iter<=0)
			mapData[pos.x()][pos.z()]=false;
	}

	
	/* Write out .map file
    ofstream outMapFile;
    std::string outFileName = std::string(filename)+".map";
    outMapFile.open (outFileName);
	
	for(int x =x_bound-1;x>=0;x-=1)
	{
		for(int z =0;z<z_bound;z+=1)
		{	
			
			if(mapData[x][z])
				outMapFile<<".";
			else
				outMapFile<<"#";

		}
		outMapFile<<"\n";
	}
	
	outMapFile.close();
	*/
    
    // TODO Figure out how to get start and end positions
    posStart = math::Vec2d(0, 0);
    posEnd = math::Vec2d(numCols-1, numRows-1);
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

math::Vec2d 
MapLoader::getStartPosition() {
    return posStart;
}

math::Vec2d 
MapLoader::getEndPosition() {
    return posEnd;
}

long
MapLoader::getNumRows() {
    return numRows;
}

long
MapLoader::getNumCols() {
    return numCols;
}
