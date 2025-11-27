#include <cstdint>
#include <fstream>
#include <chrono>
#include "nlohmann/json.hpp"
#include "Game.hpp"
#include "Renderer.hpp"
#include "Core.hpp"
#include "Window.hpp"
#include "World.hpp"
#include "EntityManager.hpp"
#include "Systems.hpp"

#include "raylib.h"

static GameContext s_Context;

static size_t s_PlayerId;
static void spawnPlayer(Vec2 position);
static void displayPlayerAttributes(GameContext& context);

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
		s_Context.level.load(mapping);
		spawnPlayer(Vec2(mapping["spawnpoint"][0],mapping["spawnpoint"][1]));
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

void Game::loop()
{
	SecClock clock;

	while (!Window::shouldClose())
	{
		tick(clock.restart());

		Renderer::beginDrawing();
		Renderer::clearBackground();

		Systems::displayView(s_Context, s_PlayerId);

		Renderer::endDrawing();
	}
}

void tick(float dt)
{
	Systems::moveControlable(s_Context, dt);
	Systems::applyVelocity(s_Context, dt);
	Systems::resolveWorldColisions(s_Context);
}

void spawnPlayer(Vec2 position)
{
	auto& entities = s_Context.entities;
	s_PlayerId = entities.spawn();

	entities.add<Comp::Controlable>(s_PlayerId);
	entities.add<Comp::Transform>(s_PlayerId, position);
	entities.add<Comp::Velocity>(s_PlayerId, 3.0f, 20.0f, 20.f);
	entities.add<Comp::Collider>(s_PlayerId, 0.3f);
}

static void displayPlayerAttributes(GameContext& context)
{
	auto& entities = s_Context.entities;
	auto id = s_PlayerId;

	auto transform = entities.get<Comp::Transform>(id);
	auto velocity = entities.get<Comp::Velocity>(id);

	std::cout << transform.position << " " << transform.angle << " " << velocity.current << std::endl;
}