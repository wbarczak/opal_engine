#include "Components.hpp"

//Vec2 Components::Velocity::update(float dt, Vec2 direction)
//{
//	if (direction.x == 0.0f && direction.y == 0.0f)
//	{
//		float calculatedSpeed = current.length() - deceleration * dt;
//
//		if (calculatedSpeed <= 0.0f)
//		{
//			current = {};
//			return current;
//		}
//
//		current = current.normalized() * calculatedSpeed;
//		return current;
//	}
//
//	current += direction * (acceleration * dt);
//
//	if (current.length() > max)
//	{
//		current = current.normalized() * max;
//	}
//
//	return current;
//}
//
//Vec2 Components::Collider::colideAgainstWorld(Vec2 position, Vec2 deltaDistance)
//{
//	return deltaDistance;
//}