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
	const Grid<Cell>& getGrid() const;
	Cell getCell(int x, int y);
private:
	Grid<Cell> mGrid;
};