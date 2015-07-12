#pragma once

#include <vector>
#include "Scene.h"
#include <set> 

using namespace math;

struct SearchNode
{
	int x;
	int y;
	int id;
	SearchNode* parent;
	float movementCost;
	float hueristicCost;

	SearchNode(int x, int y, int id, SearchNode* parent = nullptr) 
		: parent(parent), x(x), y(y), id(id), movementCost(0), hueristicCost(0) {}

	float getExpectedCost() const
	{
		return movementCost + hueristicCost;
	}

	void calculateHeuristicCost(SearchNode* endNode)
	{
		hueristicCost = std::abs(x - endNode->x) + std::abs(y - endNode->y);
	}

};

class PathFinder
{
public:
	PathFinder(SearchNode* startCell, SearchNode* endCell);
	const std::vector<Vec2f>& getPath(const Scene& scene);
	bool foundPath() const
	{
		return mFoundPath;
	}
private:
	SearchNode* getNextNode();
	void searchPath();
	void searchNode(int x, int y, float cost, SearchNode* parent);

	std::vector<SearchNode*> mOpenList;
	std::vector<SearchNode*> mClosedList;
	std::vector<Vec2d> mPath;
	SearchNode* mStartNode;
	SearchNode* mEndNode;
	bool mFoundPath;
	Scene* mScene;
};