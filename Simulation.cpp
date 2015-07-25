#include "Simulation.h"
#include <unistd.h>     // Header File for sleeping.


using namespace math;



void
Simulation::loadScene(char* mapFile)
{
	auto startTime = std::chrono::steady_clock::now();
	mStartTime = std::chrono::steady_clock::now();

    bool **data = ml.loadVDBMap(mapFile);   //Loading VDB files now

	startPosition = ml.getStartPosition();
	endPosition = ml.getEndPosition();

    x_bound = ml.getNumCols();
    y_bound = ml.getNumRows();

    printf("The grid is %d*%d\n", x_bound, y_bound);

	bool* passData = new bool[x_bound * y_bound];

	for(int y=0;y<y_bound;y++) {
        for(int x=0;x<x_bound;x++) {
            passData[y*x_bound + x] = data[y][x];
            if(passData[y*x_bound + x]) {
                //printf(".");
            } else {
                //printf("#");
            }
        }
        //printf("\n");
    }

	Grid<bool> mapData(x_bound, y_bound, passData);
	mScene = new Scene(startPosition, endPosition, mapData);

	auto endTime = std::chrono::steady_clock::now();
	std::cout << "Map Loading Time (ms) : " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << std::endl;
}

bool
Simulation::frame()
{
    flock.update();

    usleep(10000); //!@#

	return true;
}


void
Simulation::openPipe(char* pipeFile)
{
    if(pipeFile == nullptr) {
        // Don't open the pipe if the pipe file arg wasn't passed
        // Useful when debugging
        bWriteToPipe = false;
        return;
    }

	/* create the FIFO (named pipe) */
	mkfifo(pipeFile, 0700);
	fd.open(pipeFile, std::fstream::out);
    bWriteToPipe = true;
}


void
Simulation::closePipe()
{
    if(bWriteToPipe) {
        fd.close();
    }
}


void
Simulation::init(char* pipeFile)
{
	auto startTime = std::chrono::steady_clock::now();
	PathFinder pathFinder;
	mPath = pathFinder.getPath(mScene);
	openPipe(pipeFile);
	auto endTime = std::chrono::steady_clock::now();


	startPositionArea=10; //!@#
	endPositionArea=10;

    int startPosMinX = min((int)(startPosition.x-startPositionArea),0);
    int startPosMaxX = min((int)(startPosition.x+startPositionArea),(int)x_bound);
    int startPosMinY = min((int)(startPosition.y-startPositionArea),0);
    int startPosMaxY = min((int)(startPosition.y+startPositionArea),(int)y_bound);

    //endPosition-endPositionArea;
    //endPosition+endPositionArea;


    flock.setBounds(x_bound,y_bound);

    int seed=123;
	for(int i = 0; i < 10; ++i) //!@#
    {
        flock.addBoid((float)randomRange(startPosMinX,startPosMaxX,seed+i),(float)randomRange(startPosMinY,startPosMaxY,seed+i+1));//Arbritary 1. just to change y positions

    }


    std::cout << "Init Time (ms) : " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << std::endl;
}



void
Simulation::run()
{
	bool continueRunning = true;
	static long long simTime = 0;



	while (continueRunning) {
		auto startTime = std::chrono::steady_clock::now();



		continueRunning = frame();


		auto endTime = std::chrono::steady_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

		simTime+=duration;

	}
	closePipe();

	std::cout << "Flocking simulation time (ms) : "<<simTime;
	mEndTime = std::chrono::steady_clock::now();
}


long long
Simulation::totalTime()
{
	return std::chrono::duration_cast<std::chrono::seconds>(mEndTime - mStartTime).count();
}


Flocking* Simulation::getFlockHandle()
{
        return &flock;
}

Scene* Simulation::getSceneHandle()
{
        return mScene;
}
