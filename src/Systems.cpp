#include "Systems.hpp"
#include "EntityManager.hpp",
#include "World.hpp"

void Systems::resolveWorldColisions(World& world, EntityManager& entities)
{
	auto transforms = entities.getSet<Comp::Transform>();

	for (const auto& [id, transform] : transforms)
	{

	}
}

void Systems::applyVelocity(EntityManager& entities)
{
	auto& velocities = entities.getSet<Comp::Velocity>();

	for (const auto& [id, velocity] : velocities)
	{
		if (entities.has<Comp::Transform>(id))
		{
			entities.get<Comp::Transform>(id).position += velocity.current;
		}
	}
}

void Systems::displayView(EntityManager& entities, size_t currentEntity)
{

}