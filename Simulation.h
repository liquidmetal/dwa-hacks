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
	double totalTime();
private:
	void openPipe(char* pipeFile);
	void closePipe();

	Scene* mScene;
	std::vector<Vec2d> mPath;
	std::fstream fd;
};
