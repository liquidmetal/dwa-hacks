#pragma once
#include "Boid.h"
#include "Scene.h"
#include "PathFinder.h"

class Flocking {
public:
	void update();
	void addBoid();
	void addBoid(int x, int y);
    void removeBoid(int x, int y, int radius);
    void setBounds(int xbound=100, int ybound=100);
    int flockSize();
    void setDestination(int x, int y);
    void setDestination(Vec2f dest);
    void setSceneMap(Scene* scene);

    int x_bound,y_bound;
    Vec2f destination;
    float destinationArea;
    Scene* sceneMap;

    PathFinder pathFinder;

	vector<Boid> boids;

};
