#include "MapLoader.h"
#include "Vector.h"
#include <cstring>

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
