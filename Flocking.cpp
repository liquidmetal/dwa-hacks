#include "Flocking.h"

using namespace math;


/************************ Class that represents a flock *************************/

Flock::Flock(Vec2i borderMin, Vec2i borderMax, bool borderWrapping, bool borderRepulsion):
    borderMin(borderMin),
    borderMax(borderMax),
    borderWrapping(borderWrapping),
    borderRepulsion(borderRepulsion),
    boidIDCounter(0)
{

}

Flock::~Flock()
{
    clear();
}

void Flock::run()
{
    for(size_t i = 0; i < boids.size(); ++i)
    {
        boids[i]->run(boids);
    }
}

void Flock::seek(Vec2d target)
{
    for(size_t i = 0; i < boids.size(); ++i)
    {
        boids[i]->seek(target);
    }
}

void Flock::avoidObstacle(Scene* mScene)
{
    for(size_t i = 0; i < boids.size(); ++i)
    {
        boids[i]->avoidObstacle(mScene);
    }
}

void Flock::update(){
    for(size_t i = 0; i < boids.size(); ++i)
    {
        boids[i]->update();
    }    
}


int Flock::addBoid(Vec2d location, Vec2d acceleration, Vec2d velocity, double orientation, double maxSpeed, double maxForce)
{
    boids.push_back(new Boid(boidIDCounter, location, borderMin, borderMax, borderWrapping, borderRepulsion, acceleration, velocity,
                               orientation, maxSpeed, maxForce));
    ++boidIDCounter;
    return boidIDCounter;
}

int Flock::addBoid(Boid *boid)
{
    boids.push_back(boid);
    ++boidIDCounter;
    return boidIDCounter;
}

void Flock::removeBoid(int id)
{
    for(size_t i = 0; i < boids.size(); ++i)
    {
        if(id == boids[i]->getID())
        {
            delete boids[i];
            boids.erase(boids.begin() + i);
        }
    }
}

void Flock::clear()
{
    for(size_t i = 0; i < boids.size(); ++i)
    {
        delete boids[i];
    }

    boids.clear();
}

const std::vector<Boid*>& Flock::getBoids()
{
    return boids;
}

bool Flock::getBorderWrapping()
{
    return borderWrapping;
}

void Flock::setBorderWrapping(bool borderWrapping)
{
    this->borderWrapping = borderWrapping;

    for(size_t i = 0; i < boids.size(); ++i)
    {
        boids[i]->setBorderWrapping(borderWrapping);
    }
}

int Flock::getNumberOfBoids()
{
    return boids.size();
}

int Flock::size()
{
    return boids.size();
}



/***************************** Class that represents a member of a flock, called "boid" ********************/

double Boid::desiredSeparation = 1;
double Boid::neighborDist = 1;

Boid::Boid(int id, Vec2d location, Vec2i borderMin, Vec2i borderMax, bool borderWrapping, bool borderRepulsion,
           Vec2d acceleration, Vec2d velocity, double orientation, double maxSpeed, double maxForce) :
    id(id),
    location(location),
    borderMin(borderMin),
    borderMax(borderMax),
    borderWrapping(borderWrapping),
    borderRepulsion(borderRepulsion),
    acceleration(acceleration),
    velocity(velocity),
    orientation(orientation),
    maxSpeed(maxSpeed),
    maxForce(maxForce)
{
    Vec2i center = borderMin + ((borderMax - borderMin) / 2);
    borderCenter = Vec2d(center.x, center.y);
}

void Boid::run(const std::vector<Boid*>& boids)
{
    flock(boids);
    update();

    if(borderWrapping)
       wrapToBorders();

    if(borderRepulsion)
        borderRepulse();
}

void Boid::applyForce(Vec2d force)
{

}

void Boid::update()
{
    velocity = velocity + acceleration;
    velocity.limit(maxSpeed);
    orientation = atan(velocity.y / velocity.x) * 180/PI;
    location = location + velocity;
    acceleration = acceleration * 0;
}

Vec2d Boid::steer(Vec2d target)
{
    Vec2d desired = target - location;
    double distance = desired.length();

    if(distance > 0)
    {
        desired.normalize();
        desired = desired * maxSpeed;
        Vec2d steer = desired - velocity;
        steer.limit(maxForce);
        return steer;
    }
    else
    {
        return Vec2d(0, 0);
    }
}


void Boid::seek(Vec2d target)
{
    acceleration += steer(target) * 4;
}


void Boid::avoidObstacle(Scene *mScene)
{
    
    unsigned int x = (unsigned int)location.x;
    unsigned int y = (unsigned int)location.y;

    if (!mScene->getCell(x + 2, y + 2)) {
        acceleration += steer(Vec2d(x -1, y - 1));
    }

    if (!mScene->getCell(x + 1, y + 1)) {
        acceleration += steer(Vec2d(x -1, y - 1));
    }

    if (!mScene->getCell(x + 2, y - 2)) {
        acceleration += steer(Vec2d(x -1, y + 1));
    }

    if (!mScene->getCell(x + 1, y - 1)) {
        acceleration += steer(Vec2d(x -1, y + 1));
    }

    if (!mScene->getCell(x - 2, y + 2)) {
        acceleration += steer(Vec2d(x +1, y - 1));
    }

    if (!mScene->getCell(x - 1, y + 1)) {
        acceleration += steer(Vec2d(x +1, y - 1));
    }

    if (!mScene->getCell(x - 2, y - 2)) {
        acceleration += steer(Vec2d(x + 1, y + 1));
    }

    if (!mScene->getCell(x - 1, y - 1)) {
        acceleration += steer(Vec2d(x + 1, y + 1));
    }

    if (!mScene->getCell(x + 2, y)) {
        acceleration += steer(Vec2d(x - 1, y));
    }

    if (!mScene->getCell(x + 1, y)) {
        acceleration += steer(Vec2d(x - 1, y));
    }

    if (!mScene->getCell(x - 2, y)) {
        acceleration += steer(Vec2d(x + 1, y));
    }

    if (!mScene->getCell(x - 1, y)) {
        acceleration += steer(Vec2d(x + 1, y));
    }

    if (!mScene->getCell(x, y + 2)) {
        acceleration += steer(Vec2d(x, y - 1));
    }

    if (!mScene->getCell(x, y + 1)) {
        acceleration += steer(Vec2d(x, y - 1));
    }

    if (!mScene->getCell(x, y - 2)) {
        acceleration += steer(Vec2d(x, y + 1));
    }

    if (!mScene->getCell(x, y - 1)) {
        acceleration += steer(Vec2d(x, y + 1));
    }

}

math::Vec2d Boid::separate(const std::vector<Boid*>& boids)
{

}

void Boid::flock(const std::vector<Boid*>& boids)
{
    Vec2d separationForce = separate(boids) * 4.5; // Emphasize separation over the other two forces
    Vec2d alignForce = align(boids);
    Vec2d cohesionForce = cohesion(boids);

    applyForce(separationForce);
    applyForce(alignForce);
    applyForce(cohesionForce);
}


math::Vec2d Boid::align(const std::vector<Boid*>& boids)
{

}

math::Vec2d Boid::cohesion(const std::vector<Boid*>& boids)
{

}



void Boid::wrapToBorders()
{
    // Wrap x coordinate
    if(location.x >= borderMax.x)
        location.x = borderMin.x;

    else if(location.x < borderMin.x)
        location.x = borderMax.x -1;

    // Wrap y coordinate
    if(location.y >= borderMax.y)
        location.y = borderMin.y;

    else if(location.y < borderMin.y)
        location.y = borderMax.y -1;

}

void Boid::borderRepulse()
{
    if(location.x <= borderMin.x)
        location.x = borderMin.x + 1; 
    
    if (location.x >= borderMax.x)
        location.x = borderMax.x - 1;
    
    if (location.y <= borderMin.y)
        location.y = borderMin.y + 1;
    
    if (location.y >= borderMax.y)
        location.y = borderMax.y - 1;
}

bool Boid::getBorderWrapping()
{
    return borderWrapping;
}

void Boid::setBorderWrapping(bool borderWrapping)
{
    this->borderWrapping = borderWrapping;
}

int Boid::getID()
{
    return id;
}

Vec2d Boid::getLocation()
{
    return location;
}

math::Vec2d Boid::getVelocity()
{
    return velocity;
}

math::Vec2d Boid::getAcceleration()
{
    return acceleration;
}

double Boid::getOrientation()
{
    return orientation;
}

double Boid::getMaxForce()
{
    return maxForce;
}

double Boid::getMaxSpeed()
{
    return maxSpeed;
}
