#pragma once
#include "Boid.h"
#include "Scene.h"
#include "PathFinder.h"

class Flocking {
public:
	int update();
	void addBoid();
	void addBoid(int x, int y);
    void removeBoid(int x, int y, int radius);
    void setBounds(int xbound=100, int ybound=100);
    int flockSize();
    void setDestination(int x, int y,float area);
    void setDestination(Vec2f dest,float area);
    void setSceneMap(Scene* scene);

    int x_bound,y_bound;
    Vec2f destination;
    float destinationArea;
    int destinationMinX;
    int destinationMaxX;
    int destinationMinY;
    int destinationMaxY;
    Scene* sceneMap;

    PathFinder pathFinder;

	vector<Boid> boids;

};
