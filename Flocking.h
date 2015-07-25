#pragma once
#include "Boid.h"

class Flocking {
public:
	void update();
	void addBoid();
	void addBoid(int x, int y);
    void removeBoid(int x, int y, int radius);
    void setBounds(int xbound=100, int ybound=100);
    int flockSize();
    int x_bound,y_bound;
	vector<Boid> boids;
};
