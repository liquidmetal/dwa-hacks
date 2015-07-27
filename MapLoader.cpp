#include "MapLoader.h"
#include "Vector.h"
#include <cstring>
#include <openvdb/openvdb.h>
#include <stdlib.h>
#include <fstream>
using namespace std;

/************************ This loads the map from openVDB.
                          You can use the native viewer once you complete this.
                          There are comments for each snippet of code.
                          Hint: http://www.openvdb.org/documentation/doxygen/codeExamples.html **************/



bool**
MapLoader::loadVDBMap(char* filename)
{
    //Initialize the OpenVDB library.

    // Create a VDB file object.


    // Open the file. This reads the file header, but not any grids.


    //Loop over all grids in the file and retrieve a shared pointer to "SDF". Close file


    //cast the generic grid pointer to a FloatGrid pointer.

    //Get metadata into strings, see .meta file 


    //Convert strings to numbers


    // Create the map data - 2D map and SDF arrays. Allocate memory

    //Get an accessor for coordinate-based access to voxels.
    
    /* Iterate through SDF arrayusing the accessor to get SDF values. 
       Add true or false to map data according to the SDF value. */


    //Get values of Vectors posStart and posEnd

    //Change "loaded" flag

    //Return map data array
}

float**
MapLoader::getSDF(){

}

MapLoader::MapLoader() {

}

Vec2f
MapLoader::getStartPosition() {

}

Vec2f
MapLoader::getEndPosition() {

}

unsigned int
MapLoader::gety_boundary() {

}

unsigned int
MapLoader::getx_boundary() {

}

unsigned int
MapLoader::getStartRadius() {

}

unsigned int
MapLoader::getEndRadius() {

}