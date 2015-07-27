#include <openvdb/openvdb.h>
#include <stdlib.h>
#include <fstream>
using namespace std;


int main(int argc,char** argv)
{
    // If the argument isn't give, display the helpful message
    if(argc == 1) {
        printf("Usage: %s <vdb>\n", argv[0]);
        return -1;
    }
    char* filename = argv[1];

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


    string boundx = grid->metaValue<string>("boundx");
    string boundy = grid->metaValue<string>("boundy");
    string startx = grid->metaValue<string>("startx");
    string starty = grid->metaValue<string>("starty");
    string start_radius = grid->metaValue<string>("start_radius");
    string endx = grid->metaValue<string>("endx");
    string endy = grid->metaValue<string>("endy");
    string end_radius = grid->metaValue<string>("end_radius");

    int x_boundary = stoi(boundx);
    int y_boundary = stoi(boundy);

    float startRadius = stof(start_radius);
    float endRadius = stof(end_radius);

    int x_bound = x_boundary;
    int y_bound = y_boundary;
    int z_bound = 0; 


    float grid_array[x_bound][y_bound];

   
    openvdb::FloatGrid::Accessor accessor = grid->getAccessor();
    
    for(int x=0;x<x_bound;x++) 
    for(int y=0;y<y_bound;y++) 
    {
            openvdb::Coord pos(x,y,z_bound);
            grid_array[x][y] = accessor.getValue(pos);
     
        //std::cout<<"Grid array "<<grid_array[x][y]<<"\n";
    }


    ofstream outMapFile;
    std::string outFileName = std::string(filename)+".map";
        outMapFile.open (outFileName);
        
        for(int x =x_bound-1;x>=0;x-=1) {
            for(int y =0;y<y_bound;y+=1) {	
                if(grid_array[x][y]<=0&&grid_array[x][y]!=-999)
                    outMapFile<<"#";
                else
                    outMapFile<<".";
                
                //printf("%0.1f",grid_array[x][z]);
                
            }
            outMapFile<<"\n";
                //printf("\n");
        }
        
        outMapFile.close();
}
