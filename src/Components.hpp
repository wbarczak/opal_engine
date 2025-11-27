#pragma once

#include <algorithm>
#include <numeric>
#include "Core.hpp"

namespace Components
{
	struct Transform
	{
		float angle;
		Vec2 position;

		Transform(Vec2 position = {}, float angle = 0) :
			angle(angle),
			position(position) {}
	};

	struct Velocity
	{
		float max;
		float acceleration;
		float deceleration;
		Vec2 current;

		Velocity(float max, float acceleration, float deceleration, Vec2 starting = {}) :
			max(max),
			acceleration(acceleration),
			deceleration(deceleration),
			current(starting) {}
	};

	struct Collider
	{
		float radius;

		Collider(float radius) :
			radius(radius) {}
	};
}