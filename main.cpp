#include "Simulation.h"
#include <iostream>

int main(int argc, char* argv[])
{
    if(argc==1) {
        printf("Please use the following syntax to invoke this command\n");
        printf("%s <map-path>\n", argv[0]);
        return 1;
    }

    // The command line parameters for this would look like this:
    // <execname> <mapname>
    char* mapFile = argv[1];

	Simulation simulation;
	simulation.loadScene(mapFile);
	simulation.init();
	simulation.run();
	std::cout << "Total Simulation Time : " << simulation.totalTime() << std::endl;
	return 0;
}
