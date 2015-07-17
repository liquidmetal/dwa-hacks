#include "PathFinder.h"

std::vector<Vec2d>
PathFinder::getPath(Scene* scene)
{
	cleanup();
	mScene = scene;
	int startX = (int)mScene->getStartPosition().x;
	int startY = (int)mScene->getStartPosition().y;
	int startId = startY * mScene->getGrid().getMaxX() + startX;
	mStartNode = new SearchNode(startX, startY, startId);

	int endX = (int)mScene->getEndPosition().x;
	int endY = (int)mScene->getEndPosition().y;
	int endId = endX * mScene->getGrid().getMaxX() + endX;
	mEndNode = new SearchNode(endX, endY, endId);

	mOpenList.push_back(mStartNode);

	searchPath();

	if (mFoundPath) {
		return mPath;
	}
	else {
		return std::vector<Vec2d>();
	}
}

SearchNode*
PathFinder::getNextNode()
{
	double bestCost = std::numeric_limits<float>::max();
	int nodeIndex = -1;
	size_t i = 0;
	SearchNode* nextNode = nullptr;

	for (SearchNode* node : mOpenList)
	{
		if (node->getExpectedCost() < bestCost)
		{
			bestCost = node->getExpectedCost();
			nodeIndex = i;
		}
		i++;

		if (nodeIndex >= 0)
		{
			nextNode = mOpenList[nodeIndex];
			mClosedList.push_back(nextNode);
			mOpenList.erase(mOpenList.begin() + nodeIndex);
		}
	}

	return nextNode;
}

void
PathFinder::searchNode(unsigned int x,unsigned int y, double movementCost, SearchNode* parent)
{
	if (!mScene->getCell(x, y))
	{
		return;
	}

	int id = y * mScene->getGrid().getMaxX() + x;
	for (SearchNode* node : mClosedList)
	{
		if (id == node->id)
		{
			return;
		}
	}

	SearchNode* newNode = new SearchNode(x, y, id, parent);
	newNode->calculateHeuristicCost(mEndNode);
	newNode->movementCost = movementCost;

	for (SearchNode* node : mOpenList)
	{
		if (id == node->id) 
		{
			if (node->getExpectedCost() > newNode->getExpectedCost())
			{
				node->movementCost = newNode->movementCost;
				node->parent = newNode;
			}
			else
			{
				delete newNode;
				return;
			}
		}
	}

	mOpenList.push_back(newNode);
}

void
PathFinder::searchPath()
{
	if (mOpenList.empty())
	{
		return;
	}

	SearchNode* node = getNextNode();
	if (node->id == mEndNode->id)
	{
		std::vector<Vec2d> inversePath;
		while (node != nullptr) 
		{
			inversePath.push_back(Vec2d(node->x, node->y));
		}

		mPath.assign(inversePath.rbegin(), inversePath.rend());
	}
	else {
		searchNode(node->x + 1, node->y, node->movementCost + 1, node);
		searchNode(node->x - 1, node->y, node->movementCost + 1, node);
		searchNode(node->x, node->y + 1, node->movementCost + 1, node);
		searchNode(node->x, node->y - 1, node->movementCost + 1, node);
		searchNode(node->x + 1, node->y + 1, node->movementCost + 1.414, node);
		searchNode(node->x - 1, node->y + 1, node->movementCost + 1.414, node);
		searchNode(node->x + 1, node->y - 1, node->movementCost + 1.414, node);
		searchNode(node->x - 1, node->y - 1, node->movementCost + 1.414, node);
	}
}

void
PathFinder::cleanup()
{
	delete mEndNode;
	delete mStartNode;

	for (SearchNode* node : mOpenList)
	{
		delete node;
	}

	for (SearchNode* node : mClosedList)
	{
		delete node;
	}

	mFoundPath = false;
	mPath.clear();
	mOpenList.clear();
	mClosedList.clear();
}