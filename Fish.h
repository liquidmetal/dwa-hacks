#pragma once

#include <Math.h>

class Fish
{
public:
	Fish(const Vec3& position, const Vec3& minDistance, const Vec3& maxDistance, float maxAcceleration, float maxRotation, float maxSpeed)
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
	Vec3 mPosition;
	Vec3 mOrientation;
	Vec3 mMinDistance;
	Vec3 mMaxDistance;
	float mMaxAcceleration;
	float mMaxRotation;
	float mMaxSpeed;
};