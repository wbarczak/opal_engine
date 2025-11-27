#include "Entities.hpp"

#include "raylib.h"

constexpr float k_MouseSpeed = 0.08f;

void Entities::Player::tick(float dt)
{
	m_Transform.angle += GetMouseDelta().x * k_MouseSpeed * dt;
	Vec2 direction;

	if (IsKeyDown(KEY_W)) direction += Vec2::direction(m_Transform.angle);
	if (IsKeyDown(KEY_S)) direction += Vec2::direction(m_Transform.angle + std::numbers::pi);
	if (IsKeyDown(KEY_D)) direction += Vec2::direction(m_Transform.angle + std::numbers::pi / 2.0f);
	if (IsKeyDown(KEY_A)) direction += Vec2::direction(m_Transform.angle - std::numbers::pi / 2.0f);

	if (direction.dot(direction) > 1e-6f) direction.normalize();
	else direction = {};

	//Vec2 deltaDistance = m_Velocity.update(dt, direction) * dt;
	//deltaDistance = m_Colider.colideAgainstWorld(m_Transform.position, deltaDistance);

	//m_Transform.position += deltaDistance;
}