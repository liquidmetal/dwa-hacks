#pragma once

template <class T>
class Grid
{
public:
	Grid(unsigned int x, unsigned int y)
	{
		mData = new T[x * y];
		mMaxX = x;
		mMaxY = y;
	}

	unsigned int getMaxX() const
	{
		return mMaxX;
	}

	unsigned int getMaxY() const
	{
		return mMaxY;
	}

	T& getValue(unsigned int x, unsigned int y)
	{
		return mData[y*mMaxX + x];
	}
private:
	T* mData;
	unsigned int mMaxX;
	unsigned int mMaxY;
};