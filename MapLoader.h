#pragma once
#include "Vector.h"

using namespace math;

class MapLoader
{
public:
    bool** loadMap(char* filename);
    bool** loadVDBMap(char* filename);
    Vec2f getStartPosition();
    Vec2f getEndPosition();
    unsigned int getNumRows();
    unsigned int getNumCols();
    float** getSDF(); // call this only if loadVDBMap() was called
    MapLoader();

private:
    bool** mapData;         // Stores the passibility of each block
    Vec2f  posStart;        // The starting point
    Vec2f  posEnd;          // The ending point
    bool loaded;
    unsigned int numRows, numCols;
	float** grid_array;
};
