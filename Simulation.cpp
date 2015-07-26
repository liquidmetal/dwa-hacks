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
    printf("The grid is %d*%d\n", X_MAX, Y_MAX);

    bool* passData = new bool[X_MAX * Y_MAX];
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
    if (mPath.empty()) {
        std::cout << "No paths found, fish cannot reach destination. Nothing to do here..." << std::endl;
        exit(1);
    }
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
