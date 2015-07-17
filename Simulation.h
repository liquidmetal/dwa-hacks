#pragma once
#include "Scene.h"

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
};