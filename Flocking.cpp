#include "Flocking.h"

void Flocking::update() {
    int i;
    for(i = 0; i < boids.size(); i++) {
        boids[i].update(boids);
    }
}

void Flocking::addBoid() {
    boids.push_back(Boid());
}

void Flocking::addBoid(int x, int y) {
    boids.push_back(Boid(x, y, x_bound, y_bound));
}

void Flocking::removeBoid(int x, int y, int radius) {
    int i;
    for (i=0; i<boids.size(); i++) {
        if(boids[i].isHit(x, y, radius)) {
            boids.erase(boids.begin()+i);
        }
    }
}

int Flocking::flockSize() {
    return boids.size();
}


void Flocking::setBounds(int x, int y)
{
    x_bound = x;
    y_bound = y;
}
