#pragma once
#include "Scene.h"
#include <vector>
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
	double totalTime();
private:
	Scene* mScene;
	std::vector<Vec2d> mPath;
};