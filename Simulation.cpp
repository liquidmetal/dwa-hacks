#include "Simulation.h"
#include "display.h"
#include <iostream>
#include <thread>

using namespace math;

const bool T = true;
const bool F = false;
unsigned int X_MAX = 50;
unsigned int Y_MAX = 25;
const unsigned int NUM_FISH = 3;

void
Simulation::loadScene(char* mapFile)
{
	auto startTime = std::chrono::steady_clock::now();
	mStartTime = std::chrono::steady_clock::now();
    MapLoader ml;

	bool **data = ml.loadMap(mapFile);
    //bool **data = ml.loadVDBMap(mapFile);   //Loading VDB files now

	const Vec2d startPosition = ml.getStartPosition();
	const Vec2d endPosition = ml.getEndPosition();
    X_MAX = ml.getNumCols();
    Y_MAX = ml.getNumRows();
    mFishPositions = ml.getFishesFromMap();
    //std:cout << FishPositions << 
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
Simulation::frame(long long simTimeInMS)
{
    const double THRESHOLD =  1;
    // find the closest point and approach to that
    const std::vector<Boid*> boids = flock.getBoids();
    math::Vec2d location(0,0);
    for(int i = 0; i < boids.size(); ++i){
        location += boids[i]->getLocation();
    }

    location /= boids.size();

    int currentPathPoint =  pointFlockSeeking;
    // XXX - currently approching to first point in first frame 
    // not correct, should improve it further
    math::Vec2d pointToSeek = mPath[pointFlockSeeking];

    if (mPath.size() != currentPathPoint) {
        double distanceFromCurr = (mPath[currentPathPoint]-location).length();
        double distanceFromNext = (mPath[currentPathPoint + 1]-location).length();

        if (distanceFromCurr < THRESHOLD) {
            pointToSeek = mPath[currentPathPoint + 1];
            pointFlockSeeking = currentPathPoint + 1;
        }
    } else {
        return false;
    }
        

    flock.seek(pointToSeek);
    //flock.update();
    flock.run();
    flock.avoidObstacle(mScene);
    flock.update();

    FishSim fish;
    // Fish 0 is the path information
    fish.set_fish_id(0);
    fish.set_pos_x(pointToSeek.x);
    fish.set_pos_y(pointToSeek.y);
    char sz = fish.ByteSize();
    if(bWriteToPipe) {
        fd.write(&sz, sizeof(char));
        fish.SerializeToOstream(&fd);
    }
    
    // now send the positions the boids
    const std::vector<Boid*> boidsCurrent = flock.getBoids();
    for(int i = 0; i < boidsCurrent.size(); ++i){
        math::Vec2d location = boidsCurrent[i]->getLocation();
        math::Vec2d velocity = boidsCurrent[i]->getVelocity();
        math::Vec2d acceleration = boidsCurrent[i]->getAcceleration();
        //std::cout << "Boid :" << i << ": X :"<< location.x << " Y : " << location.y << std::endl;
        // std::cout << "Boid velocity:" << i << ": X :"<< velocity.x << " Y : " << velocity.y << std::endl;
        // std::cout << "Boid acceleration:" << i << ":  X :"<< acceleration.x << " Y : " << acceleration.y << std::endl;
            
        // Fish i + 1 is the actual fish data
        fish.set_fish_id(i + 1);
        fish.set_pos_x(location.x);
        fish.set_pos_y(location.y);
        char sz = fish.ByteSize();

        // Write to the pipe only if it was passed originally
        if(bWriteToPipe) {
            fd.write(&sz, sizeof(char));
            fish.SerializeToOstream(&fd);
        }
    }

    return true;

#if 0

    FishSim fish;
    for (Vec2d point : mPath) {
        std::cout << "Path: " << "X :" << point.x << " Y : " << point.y << std::endl;
        if ((point.x != 0) && (point.y != 0)){
            flock.seek(point);
            flock.update();
            flock.run();
        }

        // Fish 0 is the path information
        fish.set_fish_id(0);
        fish.set_pos_x(point.x);
        fish.set_pos_y(point.y);
        char sz = fish.ByteSize();
        if(bWriteToPipe) {
            fd.write(&sz, sizeof(char));
            fish.SerializeToOstream(&fd);
        }

    	const std::vector<Boid*> boids = flock.getBoids();
    	for(int i = 0; i < boids.size(); ++i){
            math::Vec2d location = boids[i]->getLocation();
            math::Vec2d velocity = boids[i]->getVelocity();
            math::Vec2d acceleration = boids[i]->getAcceleration();
            float orientation = boids[i]->getOrientation();
    		std::cout << "Boid :" << i << ": X :"<< location.x << " Y : " << location.y << std::endl;
            // std::cout << "Boid velocity:" << i << ": X :"<< velocity.x << " Y : " << velocity.y << std::endl;
            // std::cout << "Boid acceleration:" << i << ":  X :"<< acceleration.x << " Y : " << acceleration.y << std::endl;
            
            // Fish i + 1 is the actual fish data
            fish.set_fish_id(i + 1);
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
#endif

	// return false;
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
    pointFlockSeeking = 0;
    flock.setBorderMin(math::Vec2i(0, 0));
    flock.setBorderMax(math::Vec2i(X_MAX, Y_MAX));
    flock.setBorderWrapping(false);
    flock.setBorderRepulsion(true);
    math::Vec2d location(math::randomRange(0, X_MAX),
                         math::randomRange(0, Y_MAX));

    std::cout << "no of fishes : " <<  mFishPositions.size() << std::endl;
    for(int i = 0; i < mFishPositions.size(); ++i)
    {
        std::cout << "Fishes added at " << mFishPositions[i].x << "," << mFishPositions[i].y << std::endl;
        flock.addBoid(mFishPositions[i], 
                        math::Vec2d(0, 0),
                        math::Vec2d(
                        math::randomRange(-4, 4) / 100.0, 
                        math::randomRange(-5, 5) / 100.0
                        ),
                        0,
                        1/60.0,
                        0.1/60.0
        );
    }
}



void
Simulation::run()
{
	bool continueRunning = true;
	static long long simTime = 0;
	while (continueRunning) {
		auto startTime = std::chrono::steady_clock::now();
		onFrameStart();
		continueRunning = frame(simTime);
		onFrameEnd();
		auto endTime = std::chrono::steady_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
#if 0
		/*if (duration > 33) {
			std::cout << "Exceeded maximum allocated time." << std::endl;
			exit(1);
		}
		else*/
#endif
        if (duration > 16) {
			continue;
			simTime += duration;
		}
		else {
			std::this_thread::sleep_for(endTime - startTime);
			simTime += 16;
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
