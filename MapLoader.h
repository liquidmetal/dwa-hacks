#pragma once
#include "Vector.h"

class MapLoader
{
public:
    bool** loadMap(char* filename);
    bool** loadVDBMap(char* filename);
    math::Vec2d getStartPosition();
    math::Vec2d getEndPosition();
    long getNumRows();
    long getNumCols();
    float** getSDF(); // call this only if loadVDBMap() was called
    MapLoader();

private:
    bool** mapData;         // Stores the passibility of each block
    math::Vec2d  posStart;        // The starting point
    math::Vec2d  posEnd;          // The ending point
    bool loaded;
    long numRows, numCols;
	float** grid_array;
};
