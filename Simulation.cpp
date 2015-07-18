#include "Simulation.h"

using namespace math;

const bool T = true;
const bool F = false;
unsigned int X_MAX = 50;
unsigned int Y_MAX = 25;

void
Simulation::loadScene(char* mapFile)
{
    MapLoader ml;
    bool **data = ml.loadMap(mapFile);
	const Vec2d startPosition = ml.getStartPosition();
	const Vec2d endPosition = ml.getEndPosition();
    X_MAX = ml.getNumCols();
    Y_MAX = ml.getNumRows();

    printf("The grid is %d*%d\n", X_MAX, Y_MAX);

	bool* passData = new bool[X_MAX * Y_MAX];
	//memcpy(passData, data, sizeof(bool) * X_MAX * Y_MAX);
    for(int y=0;y<Y_MAX;y++) {
        for(int x=0;x<X_MAX;x++) {
            passData[y*X_MAX + x] = data[y][x];
            if(passData[y*X_MAX + x]) {
                printf(".");
            } else {
                printf("#");
            }
        }
        printf("\n");
    }

	Grid<bool> mapData(X_MAX, Y_MAX, passData);
	mScene = new Scene(startPosition, endPosition, mapData);
}

bool
Simulation::frame()
{

	Flock flock(math::Vec2i(0, 0), math::Vec2i(X_MAX, Y_MAX), true, false);
	math::Vec2d location(math::randomRange(0, X_MAX),
	        			 math::randomRange(0, Y_MAX));
	for(int i = 0; i < 3; ++i)
        {
            flock.addBoid(location + math::Vec2d(i, 0), 
            			  math::Vec2d(0, 0),
                          math::Vec2d(
                          	math::randomRange(-400, 400) / 100.0, 
                          	math::randomRange(-5, 5) / 100.0
                          ),                       
                          8.0,
                          0.1
         );
        }

    FishSim fish;
    for (Vec2d point : mPath) {
    	flock.run();
    	flock.seek(point);
    	const std::vector<Boid*> boids = flock.getBoids();
    	std::cout << "Path: " << "X :" << point.x << " Y : " << point.y << std::endl;
    	for(int i = 0; i < boids.size(); ++i){
    		math::Vec2d location = boids[i]->getLocation();
    		std::cout << "Boid :" << i << ": X :"<< location.x << " Y : " << location.y << std::endl;
    		fish.set_pos_x(location.x);
    		fish.set_pos_y(location.y);
    		fish.set_pos_z(0);
    		fish.set_orient_x(0);
    		fish.set_orient_y(0);
    		fish.set_orient_z(0);
    		fish.SerializeToOstream(&fd);
    	}
    }

	return false;
}

void
Simulation::onFrameStart()
{
}

void
Simulation::onFrameEnd()
{

}


void
Simulation::openPipe(char* pipeFile)
{
	/* create the FIFO (named pipe) */
	mkfifo(pipeFile, 0700);
	fd.open(pipeFile, std::fstream::out);
}


void
Simulation::closePipe()
{
	fd.close();
}


void
Simulation::init(char* pipeFile)
{
	PathFinder pathFinder;
	mPath = pathFinder.getPath(mScene);
	openPipe(pipeFile);
}



void
Simulation::run()
{
	bool continueRunning = true;
	while (continueRunning) {
		onFrameStart();
		continueRunning = frame();
		onFrameEnd();
	}
	closePipe();
}


double
Simulation::totalTime()
{
	return 0;
}
