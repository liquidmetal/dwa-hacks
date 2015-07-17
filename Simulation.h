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
using namespace math;

class Simulation
{
public:
	void loadScene();
	void init();
	void onFrameStart();
	void onFrameEnd();
	bool frame();
	void run();
	void openPipe();
	void closePipe();
	double totalTime();
private:
	Scene* mScene;
	std::vector<Vec2d> mPath;
	std::fstream fd;
};