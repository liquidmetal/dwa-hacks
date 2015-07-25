#pragma once
#include <vector>
#include "Vector.h"

class MapLoader
{
public:
    bool** loadMap(char* filename);
    math::Vec2d getStartPosition();
    math::Vec2d getEndPosition();
    long getNumRows();
    long getNumCols();
    MapLoader();
    std::vector<math::Vec2d> getFishesFromMap();

private:
    bool** mapData;         // Stores the passibility of each block
    math::Vec2d  posStart;        // The starting point
    math::Vec2d  posEnd;          // The ending point
    bool loaded;
    long numRows, numCols;
    std::vector<math::Vec2d> mFishPositions;
};
