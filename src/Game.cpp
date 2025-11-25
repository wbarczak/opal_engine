#include <cstdint>
#include <fstream>
#include <chrono>
#include "nlohmann/json.hpp"
#include "Game.hpp"
#include "Renderer.hpp"
#include "Core.hpp"
#include "Window.hpp"
#include "World.hpp"
#include "Entities.hpp"

#include "raylib.h"

constexpr float k_Fov = std::numbers::pi / 180.0f * 90.0f;

static Entities::Player s_Player;

static World s_Level;

void Game::init()
{
	using json = nlohmann::json;
	{
		std::ifstream file("assets/textures.json");
		json mapping;
		file >> mapping;
		Renderer::loadImages(mapping);
	}

	Window::init(1280, 720, "Opal Engine");

	while (!Renderer::loadTexturesFromImages());
	using json = nlohmann::json;
	{
		std::ifstream file("data/tiles.json");
		json mapping;
		file >> mapping;
		World::loadTiles(mapping);
		file.close();

		file.clear();
		file.open("data/test_map.json");
		file >> mapping;
		s_Level.load(mapping);
		s_Player.setPosition(Vec2(mapping["spawnpoint"][0],mapping["spawnpoint"][1]));
	}
	DisableCursor();
}

void Game::cleanup()
{
	Renderer::unload();
	Window::close();
	World::unloadTiles();
}

static void tick(float dt);
static void draw();

void Game::loop()
{
	SecClock clock;

	while (!Window::shouldClose())
	{
		tick(clock.restart());

		Renderer::beginDrawing();
		Renderer::clearBackground();

		Renderer::drawCeiling(Colors::Gray);
		Renderer::drawFloor(Colors::LightGray);

		draw();

		Renderer::endDrawing();
	}
}

void tick(float dt)
{
	s_Player.tick(dt);
}

void draw()
{
	const int32_t width = Window::getWidth();
	const int32_t height = Window::getHeight();

	auto position = s_Player.getPosition();
	auto angle = s_Player.getAngle();
	const float angleIncrement = k_Fov / (float)width;
	float angleStart = angle - (k_Fov * 0.5f);

	Renderer::drawCeiling(Colors::Gray);
	Renderer::drawFloor(Colors::LightGray);

	for (size_t column = 0; column < width; ++column)
	{
		float rayAngle = angleStart + column * angleIncrement;
		Vec2 direction = Vec2::direction(rayAngle);

		auto hit = s_Level.raycast(position, direction);

		if (!hit) continue;

		float perpendicularDistance = hit.value().distance * cosf(rayAngle - angle);
		int32_t lineHeight = (int)(height / perpendicularDistance);

		Renderer::drawCollumn(
			column,
			lineHeight,
			hit.value().textureId,
			hit.value().point,
			hit.value().sideways
		);
	}
}