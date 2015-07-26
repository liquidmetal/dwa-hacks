#pragma once

#include "Vector.h"
#include <vector>

using namespace math;
using namespace std;

class Boid {
public:

	Boid(int x=0, int y=0, int xbound =100, int ybound=100);


    float dist(Vec2f v1,Vec2f v2);
    float clamp(float val, float minval, float maxval);


	void update(vector<Boid> &boids);

    void seek(Vec2f target,float weight=1);
    void avoid(Vec2f target,float weight=1);
    void boundCheck(int padding);

    void flock(vector<Boid> &boids);
    bool isHit(int x,int y, int radius);

	Vec2f steer(Vec2f target);

	Vec2f separate(vector<Boid> &boids);
	Vec2f align(vector<Boid> &boids);
	Vec2f cohesion(vector<Boid> &boids);

	Vec2f loc,vel,acc;
	Vec2i endCorner;

    //Storing previous location and SDF for collision detection
    Vec2f prevloc;
    float prevSDF;

	float r;
	float maxforce;
	float maxspeed;
	float orient;

	bool reachedDestination;
	bool hitObstacle;
};

