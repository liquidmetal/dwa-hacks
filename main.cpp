#include "Simulation.h"
#include <iostream>

int main(int argc, char* argv[])
{
    if(argc<2) {
        printf("Please use the following syntax to invoke this command\n");
        printf("%s <map-path> [pipe-file]\n", argv[0]);
        return 1;
    }

    // The command line parameters for this would look like this:
    // <execname> <mapname>
    char* mapFile = argv[1];
    char* pipeFile = nullptr;
    if(argc==3) {
        pipeFile = argv[2];
    }

	Simulation simulation;
	simulation.loadScene(mapFile);
	simulation.init(pipeFile);
	simulation.run();
	std::cout << "Total Simulation Time : " << simulation.totalTime() << std::endl;
	return 0;
}
