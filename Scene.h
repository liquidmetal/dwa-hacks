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
	const Grid<Cell>& getGrid() const
	{
		return mGrid;
	}
	Cell getCell(unsigned int x, unsigned int y)
	{
		return mGrid.getValue(x, y);
	}
private:
	Grid<Cell> mGrid;
};