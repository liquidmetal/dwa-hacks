#include "Simulation.h"
#include "Vector.h"
#include "Scene.h"
#include "PathFinder.h"
#include "Flocking.h"

using namespace math;

const bool T = true;
const bool F = false;
const unsigned int X_MAX = 50;
const unsigned int Y_MAX = 25;

void
Simulation::loadScene()
{

	bool data[Y_MAX][X_MAX] = {
			{ T, T, T, T, T, T, T, T, T, T, T, T, F, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T },
			{ T, T, T, T, T, T, T, T, T, T, T, T, F, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T },
			{ T, T, T, T, T, T, T, T, T, T, T, T, F, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T },
			{ T, T, T, T, T, T, T, T, T, T, T, T, F, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T },
			{ T, T, T, T, T, T, T, T, T, T, T, T, F, T, T, T, T, T, T, T, T, T, T, T, F, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T },
			{ T, T, T, T, T, T, T, T, T, T, T, T, F, T, T, T, T, T, T, T, T, T, T, T, F, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T },
			{ T, T, T, T, T, T, T, T, T, T, T, T, F, T, T, T, T, T, T, T, T, T, T, T, F, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T },
			{ T, T, T, T, T, T, T, T, T, T, T, T, F, T, T, T, T, T, T, T, T, T, T, T, F, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T },
			{ T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, F, T, T, T, T, T, T, T, T, T, T, T, T, T, F, F, F, F, F, F, T, T, T, T, T, T },
			{ T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, F, T, T, T, T, T, T, T, T, T, T, T, T, T, F, T, T, T, T, T, T, T, T, T, T, T },
			{ T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, F, T, T, T, T, T, T, T, T, T, T, T, T, T, F, T, T, T, T, T, T, T, T, T, T, T },
			{ T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, F, T, T, T, T, T, T, T, T, T, T, T, T, T, F, T, T, T, T, T, T, T, T, T, T, T },
			{ T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, F, T, T, T, T, T, T, T, T, T, T, T, T, T, F, T, T, T, T, T, T, T, T, T, T, T },
			{ T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, F, T, T, T, T, T, T, T, T, T, T, T, T, T, F, T, T, T, T, T, T, T, T, T, T, T },
			{ T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, F, T, T, T, T, T, T, T, T, T, T, T, T, T, F, T, T, T, T, T, T, T, T, T, T, T },
			{ T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, F, T, T, T, T, T, T, T, T, T, T, T, T, T, F, F, F, F, F, F, T, T, T, T, T, T },
			{ T, T, T, T, T, T, T, T, T, T, T, T, F, T, T, T, T, T, T, T, T, T, T, T, F, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T },
			{ T, T, T, T, T, T, T, T, T, T, T, T, F, T, T, T, T, T, T, T, T, T, T, T, F, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T },
			{ T, T, T, T, T, T, T, T, T, T, T, T, F, T, T, T, T, T, T, T, T, T, T, T, F, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T },
			{ T, T, T, T, T, T, T, T, T, T, T, T, F, T, T, T, T, T, T, T, T, T, T, T, F, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T },
			{ T, T, T, T, T, T, T, T, T, T, T, T, F, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T },
			{ T, T, T, T, T, T, T, T, T, T, T, T, F, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T },
			{ T, T, T, T, T, T, T, T, T, T, T, T, F, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T },
			{ T, T, T, T, T, T, T, T, T, T, T, T, F, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T },
			{ T, T, T, T, T, T, T, T, T, T, T, T, F, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T },
	};

	const Vec2d startPosition(5, 5);
	const Vec2d endPosition(41, 12);

	bool* passData = new bool[X_MAX * Y_MAX];
	memcpy(passData, data, sizeof(data));

	Grid<bool> mapData(X_MAX, Y_MAX, passData);
	mScene = new Scene(startPosition, endPosition, mapData);

}

bool
Simulation::frame()
{
	Flock flock(math::Vec2i(0, 0), math::Vec2i(X_MAX, Y_MAX), true, false);
	math::Vec2d location(math::randomRange(0, X_MAX),
	        			 math::randomRange(0, Y_MAX));
	for(int i = 0; i < 3; ++i)
        {
            flock.addBoid(location + math::Vec2d(i, 0), 
            			  math::Vec2d(0, 0),
                          math::Vec2d(
                          	math::randomRange(-400, 400) / 100.0, 
                          	math::randomRange(-5, 5) / 100.0
                          ),                       
                          8.0,
                          0.1
         );
        }

    for (Vec2d point : mPath) {
    	flock.run();
    	flock.seek(point);
    	const std::vector<Boid*> boids = flock.getBoids();
    	std::cout << "Path: " << "X :" << point.x << " Y : " << point.y << std::endl;
    	for(int i = 0; i < boids.size(); ++i){
    		math::Vec2d location = boids[i]->getLocation();
    		std::cout << "Boid :" << i << ": X :"<< location.x << " Y : " << location.y << std::endl;

    	}
    }

	return false;
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
Simulation::init()
{
	PathFinder pathFinder;
	mPath = pathFinder.getPath(mScene);

}

void
Simulation::run()
{
	bool continueRunning = true;
	while (continueRunning) {
		onFrameStart();
		continueRunning = frame();
		onFrameEnd();
	}
}

double
Simulation::totalTime()
{
	return 0;
}