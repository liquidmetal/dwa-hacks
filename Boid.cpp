#include "Boid.h"



Boid::Boid(int x, int y, int xbound, int ybound){

    loc.setval(x,y);
	vel.setval(0,0);
	acc.setval(0,0);
    r = 3.0;
    maxspeed = .5;
    maxforce = 0.1;
    orient = 0;
    endCorner.setval(xbound,ybound);
}

// Method to update location
void Boid::update(vector<Boid> &boids) {

	flock(boids);

    vel += acc;   // Update velocity
    vel.x = clamp(vel.x, -maxspeed, maxspeed);  // Limit speed
	vel.y = clamp(vel.y, -maxspeed, maxspeed);  // Limit speed
    loc += vel;
    acc.setval(0,0);  // Resetval accelertion to 0 each cycle
    orient = (float)atan2(vel.y,vel.x) * 180/PI;

    boundCheck();


}

void Boid::seek(Vec2f target) {
    acc += steer(target);
}

void Boid::avoid(Vec2f target) {
    acc -= steer(target);
}

void Boid::boundCheck() {

    if(loc.x>endCorner.x)
    {
        loc.x=endCorner.x;
        vel.x=-vel.x;
    }

    else if(loc.x<0)
     {
         loc.x=0;
         vel.x=-vel.x;
     }

    if(loc.y>endCorner.y)
    {
        loc.y=endCorner.y;
        vel.y=-vel.y;
    }


    else if(loc.y<0)
    {
        loc.y=0;
        vel.y=-vel.y;
    }


}


// A method that calculates a steering vector towards a target
Vec2f Boid::steer(Vec2f target) {
    Vec2f steer;  // The steering vector
    Vec2f desired = target - loc;  // A vector pointing from the location to the target

	float d = target.distance(loc); // Distance from the target is the magnitude of the vector


	// If the distance is greater than 0, calc steering (otherwise return zero vector)
    if (d > 0) {

		desired /= d; // Normalize desired

		desired *= maxspeed;

		// Steering = Desired minus Velocity
		steer = desired - vel;
		steer.x = clamp(steer.x, -maxforce, maxforce); // Limit to maximum steering force
		steer.y = clamp(steer.y, -maxforce, maxforce);

    }
    return steer;
}


void Boid::flock(vector<Boid> &boids) {
	Vec2f sep = separate(boids);
	Vec2f ali = align(boids);
	Vec2f coh = cohesion(boids);

	// Arbitrarily weight these forces
	sep *= 1.5;
	ali *= 1.0;
	coh *= 1.0;

	acc += sep + ali + coh;
}

/*
 postscript
 */
bool Boid::isHit(int x, int y, int radius) {
    int r = 1;
    int range = 50;//calculation error range
    int dist =r + radius - range;
    if(pow((x-loc.x),2)+pow((y-loc.y),2) < dist * dist) {
        return true;
    }
    return false;
}

// Separation
// Method checks for nearby boids and steers away
Vec2f Boid::separate(vector<Boid> &boids) {
    float desiredseparation = 10.0f;
    Vec2f steer;
    int count = 0;

    // For every boid in the system, check if it's too close
    for (int i = 0 ; i < boids.size(); i++) {
		Boid &other = boids[i];

		float d = loc.distance(other.loc);

		// If the distance is greater than 0 and less than an arbitrary amount (0 when you are yourself)
		if ((d > 0) && (d < desiredseparation)) {
			// Calculate vector pointing away from neighbor
			Vec2f diff = loc - other.loc;
			diff /= d;			// normalize
			diff /= d;        // Weight by distance
			steer += diff;
			count++;            // Keep track of how many
		}
    }
    // Average -- divide by how many
    if (count > 0) {
		steer /= (float)count;
    }


    // As long as the vector is greater than 0
	//float mag = sqrt(steer.x*steer.x + steer.y*steer.y);

	float mag = steer.length();
    if (mag > 0) {
		// Implement Reynolds: Steering = Desired - Velocity
		steer /= mag;
		steer *= maxspeed;
		steer -= vel;
		steer.x = clamp(steer.x, -maxforce, maxforce);
		steer.y = clamp(steer.y, -maxforce, maxforce);
    }
    return steer;
}

// Alignment
// For every nearby boid in the system, calculate the average velocity
Vec2f Boid::align(vector<Boid> &boids) {
    float neighbordist = 15.0;
    Vec2f steer;
    int count = 0;
    for (int i = 0 ; i < boids.size(); i++) {
		Boid &other = boids[i];

		float d = loc.distance(other.loc);
		if ((d > 0) && (d < neighbordist)) {
			steer += (other.vel);
			count++;
		}
    }
    if (count > 0) {
		steer /= (float)count;
    }

    // As long as the vector is greater than 0
	float mag = steer.length();
    if (mag > 0) {
		// Implement Reynolds: Steering = Desired - Velocity
		steer /= mag;
		steer *= maxspeed;
		steer -= vel;
		steer.x = clamp(steer.x, -maxforce, maxforce);
		steer.y = clamp(steer.y, -maxforce, maxforce);
    }
    return steer;
}

// Cohesion
// For the average location (i.e. center) of all nearby boids, calculate steering vector towards that location
Vec2f Boid::cohesion(vector<Boid> &boids) {
    float neighbordist = 20.0;
    Vec2f sum;   // Start with empty vector to accumulate all locations
    int count = 0;
    for (int i = 0 ; i < boids.size(); i++) {
		Boid &other = boids[i];
		float d = loc.distance(other.loc);
		if ((d > 0) && (d < neighbordist)) {
			sum += other.loc; // Add location
			count++;
		}
    }
    if (count > 0) {
		sum /= (float)count;
		return steer(sum);  // Steer towards the location
    }
    return sum;
}


float Boid::dist(Vec2f v1,Vec2f v2)
{
    return v1.distance(v2);
}

float Boid::clamp(float val, float minval, float maxval)
{
    if(val<minval)
        return minval;
    else if (val>maxval)
        return maxval;
    else
        return val;
}
