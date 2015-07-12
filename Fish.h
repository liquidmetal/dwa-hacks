#pragma once

#include "Vector.h"

using namespace math;

class Fish
{
public:
	Fish(const Vec3d& position, const Vec3d& minDistance, const Vec3d& maxDistance, float maxAcceleration, float maxRotation, float maxSpeed)
		: mPosition(position)
		, mMinDistance(minDistance)
		, mMaxDistance(maxDistance)
		, mMaxAcceleration(maxAcceleration)
		, mMaxRotation(maxRotation)
		, mMaxSpeed(maxSpeed)
	{
	}

	Vec3d& position() {
		return mPosition;
	}
	const Vec3d& position() const {
		return mPosition;
	}
	const Vec3d& minDistance() const {
		return mMinDistance;
	}
	const Vec3d& maxDistance() const {
		return mMaxDistance;
	}
	float maxAcceleration() const {
		return mMaxAcceleration;
	}
	float maxRotation() const {
		return mMaxRotation;
	}
	
	float maxSpeed() const {
		return mMaxSpeed;
	}
private:
	Vec3d mPosition;
	Vec3d mOrientation;
	Vec3d mMinDistance;
	Vec3d mMaxDistance;
	float mMaxAcceleration;
	float mMaxRotation;
	float mMaxSpeed;
};