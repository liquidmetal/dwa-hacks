#pragma once
#include "Scene.h"
#include "Vector.h"
#include "PathFinder.h"
#include "Flocking.h"
#include <vector>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include "fish-sim.pb.h"
#include "MapLoader.h"
#include <chrono>
using namespace math;

class Simulation
{
public:
	void loadScene(char* mapFile);
	void init(char* pipeFile);
	void onFrameStart();
	void onFrameEnd();
	bool frame();
	void run();
	long long totalTime();
private:
	void openPipe(char* pipeFile);
	void closePipe();
    bool bWriteToPipe = false;

	Scene* mScene;
	std::vector<Vec2d> mPath;
	std::fstream fd;
	std::chrono::steady_clock::time_point mStartTime;
	std::chrono::steady_clock::time_point mEndTime;
};
