#include "Flocking.h"

int Flocking::update()
{
    int i;
    for(i = 0; i < boids.size(); i++)
    {

        if(boids[i].isHit(destination.x,destination.y,destinationArea))
        {
            boids[i].reachedDestination = true;
        }


        if(sceneMap->getCell(boids[i].loc.x,boids[i].loc.y))
        {
            vector<Vec2f> path;
            path = pathFinder.getPath(sceneMap,boids[i].loc);
            if(path.size()>1)
            {
                Vec2f dest = path[min((int)path.size()-1,1)]; //!@#
                boids[i].seek(dest,1.5); //seek the Goal !@#
            }
        }
        else
        {
            boids[i].hitObstacle = true;
        }


        boids[i].update(boids);

        if(boids[i].reachedDestination)
        removeBoid(boids[i].loc.x,boids[i].loc.y,1);  //ineffcient way to remove boids

    }

    if(flockSize()==0)
        return 0;
    else
        return 1;
}

void Flocking::addBoid()
{
    boids.push_back(Boid());
}

void Flocking::addBoid(int x, int y)
{
    boids.push_back(Boid(x, y, x_bound, y_bound));
}

void Flocking::removeBoid(int x, int y, int radius)
{
    int i;
    for (i=0; i<boids.size(); i++)
    {
        if(boids[i].isHit(x, y, radius))
        {
            boids.erase(boids.begin()+i);
        }
    }
}

int Flocking::flockSize()
{
    return boids.size();
}


void Flocking::setBounds(int x, int y)
{
    x_bound = x;
    y_bound = y;
}


void Flocking::setDestination(int x, int y,float area)
{
    destination.setval((float)x,(float)y);
    destinationArea = area;

    destinationMinX = min((int)(destination.x-destinationArea),0);
    destinationMaxX = min((int)(destination.x+destinationArea),(int)x_bound);
    destinationMinY = min((int)(destination.y-destinationArea),0);
    destinationMaxY = min((int)(destination.y+destinationArea),(int)y_bound);

}


void Flocking::setDestination(Vec2f dest,float area)
{
    destination=dest;
    destinationArea=area;

    destinationMinX = min((int)(destination.x-destinationArea),0);
    destinationMaxX = min((int)(destination.x+destinationArea),(int)x_bound);
    destinationMinY = min((int)(destination.y-destinationArea),0);
    destinationMaxY = min((int)(destination.y+destinationArea),(int)y_bound);

}


void Flocking::setSceneMap(Scene* scene)
{
    sceneMap = scene;
}

