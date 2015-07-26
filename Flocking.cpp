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

        if(useCollisionFromSDF)
        {
            float sdfval = collisionSDF[(int)boids[i].loc.x][(int)boids[i].loc.y];
            float val,usesdfval;
            Vec2f dir;

            if(sdfval!=-999)
            {
                if(boids[i].prevSDF!=-999)
                {
                    val = sdfval-boids[i].prevSDF;

                }
                else
                {
                    val = 1;
                }


                if(sdfval==0)
                {
                    if(boids[i].prevSDF<0)
                        usesdfval = 0.1;
                    else
                        usesdfval = -0.1;
                }


            }
            else
            {
                val = 1;
                usesdfval = 999;
            }
            dir = boids[i].loc-boids[i].prevloc;



            if(val>0)
                dir/=usesdfval;
            else if(val<0)
                dir/=-usesdfval;


            //boids[i].seek(dir+boids[i].loc,dir.length()*0.1);

            boids[i].prevSDF=sdfval;
            boids[i].prevloc=boids[i].loc;

        }

        if(sceneMap->getCell(boids[i].loc.x,boids[i].loc.y))
        {

            vector<Vec2f> path;
            path = pathFinder.getPath(sceneMap,boids[i].loc);
            if(path.size()>1)
            {
                Vec2f dest = path[min((int)path.size()-1,1)]; //!@#
                boids[i].seek(dest,1); //seek the Goal !@#
            }


            //boids[i].seek(destination,0.5); //seek the Goal !@#
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
}


void Flocking::setDestination(Vec2f dest,float area)
{
    destination=dest;
    destinationArea=area;
}


void Flocking::setSceneMap(Scene* scene)
{
    sceneMap = scene;
}

void Flocking::useCollisionSDF(bool val)
{
    useCollisionFromSDF = val;

    if(useCollisionFromSDF)
    {
        collisionSDF = sceneMap->getSDFhandle();
    }
}

vector<Boid> Flocking::getBoids()
{
    return boids;
}
