#include "Systems.hpp"
#include "EntityManager.hpp",
#include "World.hpp"
#include "Core.hpp"

#include "raylib.h"

static Vec2 calculateVelocity(Vec2 );

void Systems::resolveWorldColisions(GameContext& context)
{
	auto& transforms = context.entities.getSet<Comp::Transform>();

	const int32_t worldWidth = context.level.width();
	const int32_t worldHeight = context.level.height();

	for (const auto& [id, transform] : transforms)
	{
		if (!context.entities.has<Comp::Collider>(id)) continue;

		Cir bounds(
			transform.position,
			context.entities.get<Comp::Collider>(id).radius
		);

		Vec2i starting{
			bounds.pos.x - bounds.rad,
			bounds.pos.y - bounds.rad
		};

		Vec2i ending{
			bounds.pos.x + bounds.rad,
			bounds.pos.y + bounds.rad
		};

		Vec2 fullResolution;

		for (int32_t y = starting.y; y <= ending.y; ++y)
		{
			if (y < 0 || y >= worldHeight) continue;

			for (int32_t x = starting.x; x <= ending.x; ++x)
			{
				if (x < 0 || x >= worldWidth ||
					!context.level.tile(y, x).isSolid())
				{
					continue;
				}
				
				fullResolution += bounds.resolve(Rect(x, y, 1.0f, 1.0f));
			}
		}

		transform.position += fullResolution;
	}
}

void Systems::applyVelocity(GameContext& context, float dt)
{
	auto& velocities = context.entities.getSet<Comp::Velocity>();

	for (const auto& [id, velocity] : velocities)
	{
		if (!context.entities.has<Comp::Transform>(id)) continue;

		context.entities.get<Comp::Transform>(id).position += velocity.current * dt;
	}
}

Vec2 calculateVelocity(float dt, Vec2 direction, GameContext& context, size_t id)
{
	if (!context.entities.has<Comp::Velocity>(id)) return {};

	auto& velocity = context.entities.get<Comp::Velocity>(id);

	if (direction.x == 0.0f && direction.y == 0.0f)
	{
		float calculatedSpeed = velocity.current.length() - velocity.deceleration * dt;

		if (calculatedSpeed <= 0.0f)
		{
			velocity.current = {};
			return velocity.current;
		}

		velocity.current = velocity.current.normalized() * calculatedSpeed;
		return velocity.current;
	}

	velocity.current += direction * (velocity.acceleration * dt);

	if (velocity.current.length() > velocity.max)
	{
		velocity.current = velocity.current.normalized() * velocity.max;
	}

	return velocity.current;
}

static constexpr float k_MouseSpeed = 0.08f;

void Systems::moveControlable(GameContext& context, float dt)
{
	auto& controlables = context.entities.getSet<Comp::Controlable>();

	for (const auto& [id, _] : controlables)
	{
		auto& transform = context.entities.get<Comp::Transform>(id);
		transform.angle += GetMouseDelta().x * k_MouseSpeed * dt;
		Vec2 direction;

		if (IsKeyDown(KEY_W)) direction += Vec2::direction(transform.angle);
		if (IsKeyDown(KEY_S)) direction += Vec2::direction(transform.angle + std::numbers::pi);
		if (IsKeyDown(KEY_D)) direction += Vec2::direction(transform.angle + std::numbers::pi / 2.0f);
		if (IsKeyDown(KEY_A)) direction += Vec2::direction(transform.angle - std::numbers::pi / 2.0f);

		if (direction.dot(direction) > 1e-6f) direction.normalize();
		else direction = {};

		Vec2 deltaDistance = calculateVelocity(dt, direction, context, id) * dt;
	}
}