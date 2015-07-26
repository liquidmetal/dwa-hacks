#pragma once

#include <vector>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <chrono>
#include <thread>

#include "Scene.h"
#include "Vector.h"
#include "PathFinder.h"
#include "Flocking.h"
#include "fish-sim.pb.h"
#include "MapLoader.h"

using namespace math;

class Simulation
{
public:
	void loadScene(char* mapFile);
	void init(char* pipeFile);

	bool frame();
	void run();
	long long totalTime();

	Flocking* getFlockHandle();
	Scene* getSceneHandle();

	Simulation()
	{
	    x_bound = 100;
	    y_bound = 100;
    }

private:

	void openPipe(char* pipeFile);
	void closePipe();
    bool bWriteToPipe = false;

    Flocking flock;
	Scene* mScene;
	MapLoader ml;
	std::vector<Vec2f> mPath;

	std::fstream fd;
	std::chrono::steady_clock::time_point mStartTime;
	std::chrono::steady_clock::time_point mEndTime;

	unsigned int x_bound;
    unsigned int y_bound;
    Vec2f startPosition;
	Vec2f endPosition;
	float startPositionRadius;
	float endPositionRadius;

};
