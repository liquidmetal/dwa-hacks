#pragma once

#include "Fish.h"
#include "Vector.h"
#include "Grid.h"

struct Cell
{
	bool passable;
};

class Scene
{
public:
	Scene(const Vec2d& startPosition, const Vec2d& endPosition, const Grid<bool>& grid)
		:mStartPosition(startPosition)
		,mEndPosition(endPosition)
		,mGrid(grid)
	{
	}

	Vec2d getStartPosition() const
	{
		return mStartPosition;
	}
	Vec2d getEndPosition() const
	{
		return mEndPosition;
	}

	const Grid<bool>& getGrid() const
	{
		return mGrid;
	}
	bool getCell(unsigned int x, unsigned int y)
	{
		unsigned int maxX = mGrid.getMaxX();
		unsigned int maxY = mGrid.getMaxY();
		if ((x > maxX) || (y > maxY) || (x < 0) || (y < 0)){
			return false;
		}
		return mGrid.getValue(x, y);
	}
private:
	Grid<bool> mGrid;
	Vec2d mStartPosition;
	Vec2d mEndPosition;
};