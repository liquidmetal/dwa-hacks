#ifndef FLOCKING_H
#define FLOCKING_H

// System includes
#include <vector>

// Local includes
#include "Vector.h"



class Boid {

public:

    Boid(int id, math::Vec2d location, math::Vec2i borderMin, math::Vec2i borderMax, bool borderWrapping, bool borderRepulsion,
         math::Vec2d acceleration, math::Vec2d velocity, double maxSpeed, double maxForce);

    void run(const std::vector<Boid*>& boids);
    void applyForce(math::Vec2d force);
    void update();
    math::Vec2d steer(math::Vec2d target);
    void seek(math::Vec2d target);
    math::Vec2d separate(const std::vector<Boid*>& boids);
    void flock(const std::vector<Boid*>& boids);
    math::Vec2d align(const std::vector<Boid*>& boids);
    math::Vec2d cohesion(const std::vector<Boid*>& boids);
    void drift();
    math::Vec2d steer();

    void wrapToBorders();
    void borderRepulse();
    bool getBorderWrapping();
    void setBorderWrapping(bool borderWrapping);
    int getID();
    math::Vec2d getLocation();
    math::Vec2d getVelocity();
    math::Vec2d getAcceleration();
    double getMaxForce();
    double getMaxSpeed();

    static double desiredSeparation;
    static double neighborDist;

private:

    math::Vec2i borderMin, borderMax;
    bool borderWrapping, borderRepulsion;

    math::Vec2d location;
    math::Vec2d velocity;
    math::Vec2d acceleration;
    math::Vec2d target;
    math::Vec2d borderCenter;

    double driftAngle;
    double maxForce;    // Maximum steering force
    double maxSpeed;    // Maximum speed

    int id;
};

class Flock {

public:

    Flock(math::Vec2i borderMin = math::Vec2i(), math::Vec2i borderMax = math::Vec2i(),
          bool borderWrapping = false, bool borderRepulsion = false);
    ~Flock();

    void run();
    void seek(math::Vec2d target);
    void drift();

    // Returns the boid id for the created boid
    int addBoid(math::Vec2d location, math::Vec2d acceleration,
                math::Vec2d velocity,
                double maxSpeed, 
		        double maxForce);

    // Flock takes ownership of the the item and will free the memory upon removal or on the Flocks deconstructor
    int addBoid(Boid* boid);
    void removeBoid(int id);
    void clear();
    const std::vector<Boid*>& getBoids();
    bool getBorderWrapping();
    void setBorderWrapping(bool borderWrapping);
    int getNumberOfBoids();
    int size();

private:

    std::vector<Boid*> boids;
    math::Vec2i borderMin, borderMax;
    bool borderWrapping;
    bool borderRepulsion;
    int boidIDCounter;
};


#endif // FLOCKING_H

