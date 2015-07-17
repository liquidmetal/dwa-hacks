#include "Simulation.h"
#include <iostream>

int main()
{
	Simulation simulation;
	simulation.loadScene();
	simulation.init();
	simulation.run();
	std::cout << "Total Simulation Time : " << simulation.totalTime() << std::endl;
	return 0;
}