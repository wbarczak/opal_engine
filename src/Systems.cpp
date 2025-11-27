#include "Systems.hpp"
#include "EntityManager.hpp",
#include "World.hpp"

void Systems::applyVelocity(EntityManager& entities)
{
	auto velocities = entities.getSet<Components::Velocity>();

	for (const auto& [id, component] : velocities)
	{
		auto smth = entities.get<Components::Transform>(id).position += component.current;
	}
}