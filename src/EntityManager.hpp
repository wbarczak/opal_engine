#pragma once

#include <vector>
#include <unordered_set>
#include <tuple>
#include "SparseSet.hpp"
#include "Components.hpp"

static constexpr size_t k_MaxEntities = 2000;

template <typename T>
using CompSet = SparseSet<T, k_MaxEntities>;

using CompSets = std::tuple<
	CompSet<Comp::Collider>,
	CompSet<Comp::Controlable>,
	CompSet<Comp::Transform>,
	CompSet<Comp::Velocity>
>;

class EntityManager
{
public:

	static constexpr size_t k_MaxEntities = 2000;

	template <typename T>
	using CompSet = SparseSet<T, k_MaxEntities>;

	using CompSets = std::tuple<
		CompSet<Comp::Collider>,
		CompSet<Comp::Controlable>,
		CompSet<Comp::Transform>,
		CompSet<Comp::Velocity>
	>;

	EntityManager()
	{
		m_Freelist.reserve(k_MaxEntities);
		for (int32_t id = k_MaxEntities - 1; id >= 0; --id)
		{
			m_Freelist.push_back(id);
		}
	}

	size_t spawn()
	{
		size_t id = m_Freelist.back();
		m_Freelist.pop_back();
		m_Entities.insert(id);
		return id;
	}

	void despawn(size_t id)
	{
		if (!m_Entities.count(id)) return;

		std::get<CompSet<Comp::Collider>>(m_Components).popIfContains(id);
		std::get<CompSet<Comp::Controlable>>(m_Components).popIfContains(id);
		std::get<CompSet<Comp::Transform>>(m_Components).popIfContains(id);
		std::get<CompSet<Comp::Velocity>>(m_Components).popIfContains(id);

		m_Freelist.push_back(id);
	}

	template <typename C, typename ... Args>
	void add(size_t id, Args&&... args)
	{
		std::get<CompSet<C>>(m_Components).emplace(id, std::forward<Args>(args)...);
	}

	template <typename C>
	void remove(size_t id)
	{
		std::get<CompSet<C>>(m_Components).popIfContains(id);
	}

	template <typename C>
	bool has(size_t id)
	{
		return std::get<CompSet<C>>(m_Components).contains(id);
	}

	template <typename C>
	C& get(size_t id)
	{
		return std::get<CompSet<C>>(m_Components)[id];
	}

	template<typename C>
	auto& getSet()
	{
		return std::get<CompSet<C>>(m_Components);
	}

	bool contains(size_t id)
	{
		assert(id < k_MaxEntities, "You're trying to check if an entity of impossible id exists");

		return m_Entities.count(id);
	}

private:

	std::vector<size_t> m_Freelist;
	std::unordered_set<size_t> m_Entities;
	CompSets m_Components;
};