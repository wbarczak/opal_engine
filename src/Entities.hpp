#pragma once

#include "nlohmann/json.hpp"
#include "Components.hpp"

namespace Entities
{
	class Entity
	{
	public:

		Entity() = default;
		virtual ~Entity() = default;
		virtual void tick(float dt) = 0;
	};

	class Player : public Entity
	{
	public:

		Player(Vec2 position = {}, float angle = 0.0f) :
			m_Transform(position, angle),
			m_Velocity(3.0f, 20.0f, 20.0f),
			m_Colider(0.3f) {}
		void setPosition(Vec2 position) { m_Transform.position = position; }
		void setAngle(float angle) { m_Transform.angle = angle; }
		Vec2 getPosition() { return m_Transform.position; }
		float getAngle() { return m_Transform.angle; }
		void tick(float dt);

	private:

		Comp::Transform m_Transform;
		Comp::Velocity m_Velocity;
		Comp::Collider m_Colider;
	};
}