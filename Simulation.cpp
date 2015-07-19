#include "Simulation.h"
#include <thread>

using namespace math;

const bool T = true;
const bool F = false;
unsigned int X_MAX = 50;
unsigned int Y_MAX = 25;


void
Simulation::loadScene(char* mapFile)
{
	auto startTime = std::chrono::steady_clock::now();
	mStartTime = std::chrono::steady_clock::now();
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
	auto endTime = std::chrono::steady_clock::now();
	std::cout << "Map Loading Time (ms) : " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << std::endl;
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
            fish.set_fish_id(i);
    		fish.set_pos_x(location.x);
    		fish.set_pos_y(location.y);
            char sz = fish.ByteSize();

            // Write to the pipe only if it was passed originally
            if(bWriteToPipe) {
                fd.write(&sz, sizeof(char));
                fish.SerializeToOstream(&fd);
            }
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
	std::cout << "Init Time (ms) : " << std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count() << std::endl;
}



void
Simulation::run()
{
	bool continueRunning = true;
	while (continueRunning) {
		auto startTime = std::chrono::steady_clock::now();
		onFrameStart();
		continueRunning = frame();
		onFrameEnd();
		auto endTime = std::chrono::steady_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
		if (duration > 33) {
			std::cout << "Exceeded maximum allocated time." << std::endl;
			exit(1);
		}
		else if (duration > 16) {
			continue;
		}
		else {
			std::this_thread::sleep_for(endTime - startTime);
		}

	}
	closePipe();
	mEndTime = std::chrono::steady_clock::now();
}


long long
Simulation::totalTime()
{
	return std::chrono::duration_cast<std::chrono::seconds>(mEndTime - mStartTime).count();
}
