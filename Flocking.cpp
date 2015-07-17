#include "Flocking.h"

using namespace math;


////////////////
// Boid
////////////////

double Boid::desiredSeparation = 50.0;
double Boid::neighborDist = 100.0;

Boid::Boid(int id, Vec2d location, Vec2i borderMin, Vec2i borderMax, bool borderWrapping, bool borderRepulsion,
           Vec2d acceleration, Vec2d velocity, double maxSpeed, double maxForce) :
    id(id),
    location(location),
    borderMin(borderMin),
    borderMax(borderMax),
    borderWrapping(borderWrapping),
    borderRepulsion(borderRepulsion),
    acceleration(acceleration),
    velocity(velocity),
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
    acceleration = acceleration + force;
}

void Boid::update()
{
    velocity = velocity + acceleration;
    velocity.limit(maxSpeed);
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
    acceleration += steer(target);
}

math::Vec2d Boid::separate(const std::vector<Boid*>& boids)
{
    Vec2d steer(0, 0);
    int count = 0;

    for(size_t i = 0; i < boids.size(); ++i)
    {
        Boid* boid = boids[i];
        double distance = location.distance(boid->getLocation());

        if(distance > 0 && distance < desiredSeparation)
        {
            Vec2d difference = location - boid->getLocation();
            difference.normalize();
            difference = difference / distance;
            steer = steer + difference;
            ++count;
        }
    }

    if(count > 0)
    {
        steer = steer / (double) count; // Average
    }

    if(steer.length() > 0)
    {
        steer.normalize();
        steer = steer * maxSpeed;
        steer = steer - velocity;
        steer.limit(maxForce);
    }

    return steer;
}

void Boid::flock(const std::vector<Boid*>& boids)
{
    Vec2d separationForce = separate(boids) * 1.5; // Emphasize separation over the other two forces
    Vec2d alignForce = align(boids);
    Vec2d cohesionForce = cohesion(boids);

    applyForce(separationForce);
    applyForce(alignForce);
    applyForce(cohesionForce);
}

math::Vec2d Boid::align(const std::vector<Boid*>& boids)
{
    Vec2d sum(0, 0);
    int count = 0;

    for(size_t i = 0; i < boids.size(); ++i)
    {
        Boid* boid = boids[i];
        double distance = location.distance(boid->getLocation());

        if(distance > 0 && distance < neighborDist)
        {
            sum = sum + boid->getVelocity();
            ++count;
        }
    }

    if(count > 0)
    {
        sum = sum / (double) count;
        //sum.normalize();
        //sum = sum * maxSpeed;
        //Vec2d steer = sum - velocity;
        //steer.limit(maxForce);
        return sum;
    }

    else
    {
        return Vec2d(0, 0);
    }
}

math::Vec2d Boid::cohesion(const std::vector<Boid*>& boids)
{
    Vec2d sum(0, 0);
    int count = 0;

    for(size_t i = 0; i < boids.size(); ++i)
    {
        Boid* boid = boids[i];
        double distance = location.distance(boid->getLocation());
        if(distance > 0 && distance < neighborDist)
        {
            sum = sum + boid->getLocation();
            ++count;
        }
    }

    if(count > 0)
    {
        sum = sum / (double) count;
        return steer(sum);
    }

    else
    {
        return Vec2d(0, 0);
    }
}

void Boid::drift()
{
    double radius = 30.0;
    double diameter = 60.0;
    int driftBounds = 45;
    driftAngle += randomRange(-driftBounds, driftBounds);

    Vec2d center = velocity;
    center.normalize();
    center *= diameter;
    center += location;

    Vec2d offset(radius * cos(driftAngle), radius * sin(driftAngle));
    Vec2d target = center + offset;
    acceleration += steer(target);
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

    // Wrap z coordinate
    //if(location.z >= borderMax.z)
    //    location.z = borderMin.z;

   // else if(location.z < borderMin.z)
   //     location.z = borderMax.z -1;
}

void Boid::borderRepulse()
{
    if(location.x < borderMin.x || location.x > borderMax.x
            || location.y < borderMin.y || location.y > borderMax.y)
    //        || location.z < borderMin.z || location.z > borderMax.z)
    {
        seek(borderCenter);
    }
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

double Boid::getMaxForce()
{
    return maxForce;
}

double Boid::getMaxSpeed()
{
    return maxSpeed;
}

////////////////
// Flock
////////////////

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

void Flock::drift()
{
    for(size_t i = 0; i < boids.size(); ++i)
    {
        boids[i]->drift();
    }
}

int Flock::addBoid(Vec2d location, Vec2d acceleration, Vec2d velocity, double maxSpeed, double maxForce)
{
    boids.push_back(new Boid(boidIDCounter, location, borderMin, borderMax, borderWrapping, borderRepulsion, acceleration, velocity,
                               maxSpeed, maxForce));
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




