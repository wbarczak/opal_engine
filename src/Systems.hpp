#pragma once

class World;
class EntityManager;

namespace Systems
{
	void resolveWorldColisions(World& world, EntityManager& entities);
	void applyVelocity(EntityManager& entities);
	void displayView(EntityManager& entities, size_t currentEntity);
}