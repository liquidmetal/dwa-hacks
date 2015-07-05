#pragma once
typedef unsigned int uint;

class Vec3
{
public:
	float& x() {
		return mX;
	}
	float& y() {
		return mY;
	}
	float& z() {
		return mZ;
	}
	
	float distance(const Vec3& destination) const;
	
private:
	float mX;
	float mY;
	float mZ;
}