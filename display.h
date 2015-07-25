#include <GL/gl.h>	// Header File For The OpenGL32 Library
#include <GL/glu.h>	// Header File For The GLu32 Library
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <unistd.h>     // Header File for sleeping.
#include <thread>
#include <vector>

class dispFish
{
public:
    dispFish(int fid,float fx,float fy,float fz,float forientation)
    :id(fid) ,
     x(fx) ,
     y(fy) ,
     z(fz) ,
     orientation(forientation)
     {}


    int id;
    float x,y,z;
    float orientation;

};


extern std::vector<dispFish> display_fishes;
