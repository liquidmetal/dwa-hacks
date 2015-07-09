#ifndef FLOCKING_H
#define FLOCKING_H

// System includes
#include <vector>

// Local includes
#include "Math/Vector.h"



class Boid {

public:

    Boid(int id, geometry::Vec2d location, geometry::Vec2i borderMin, geometry::Vec2i borderMax, bool borderWrapping, bool borderRepulsion,
         geometry::Vec2d acceleration, geometry::Vec2d velocity, double maxSpeed, double maxForce);

    void run(const std::vector<Boid*>& boids);
    void applyForce(geometry::Vec2d force);
    void update();
    geometry::Vec2d steer(geometry::Vec2d target);
    void seek(geometry::Vec2d target);
    geometry::Vec2d separate(const std::vector<Boid*>& boids);
    void flock(const std::vector<Boid*>& boids);
    geometry::Vec2d align(const std::vector<Boid*>& boids);
    geometry::Vec2d cohesion(const std::vector<Boid*>& boids);
    void drift();
    geometry::Vec2d steer();

    void wrapToBorders();
    void borderRepulse();
    bool getBorderWrapping();
    void setBorderWrapping(bool borderWrapping);
    int getID();
    geometry::Vec2d getLocation();
    geometry::Vec2d getVelocity();
    geometry::Vec2d getAcceleration();
    double getMaxForce();
    double getMaxSpeed();

    static double desiredSeparation;
    static double neighborDist;

private:

    geometry::Vec2i borderMin, borderMax;
    bool borderWrapping, borderRepulsion;

    geometry::Vec2d location;
    geometry::Vec2d velocity;
    geometry::Vec2d acceleration;
    geometry::Vec2d target;
    geometry::Vec2d borderCenter;

    double driftAngle;
    double maxForce;    // Maximum steering force
    double maxSpeed;    // Maximum speed

    int id;
};

class Flock {

public:

    Flock(geometry::Vec2i borderMin = geometry::Vec2i(), geometry::Vec2i borderMax = geometry::Vec2i(),
          bool borderWrapping = false, bool borderRepulsion = false);
    ~Flock();

    void run();
    void seek(geometry::Vec2d target);
    void drift();

    // Returns the boid id for the created boid
    int addBoid(geometry::Vec2d location, geometry::Vec2d acceleration = geometry::Vec2d 0,0),
                 geometry::Vec2d velocity = geometry::Vec2d geometry::randomRange<int>(-100, 100) / 40.0,
                                                          geometry::randomRange<int>(-100, 100) / 40.0,
                                                          geometry::randomRange<int>(-100, 100) / 40.0 + 0.01),
                 double maxSpeed = 5.0, double maxForce = 0.75);

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
    geometry::Vec2i borderMin, borderMax;
    bool borderWrapping;
    bool borderRepulsion;
    int boidIDCounter;
};


#endif // FLOCKING_H

