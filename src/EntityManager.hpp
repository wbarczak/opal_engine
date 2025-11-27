#pragma once

#include <vector>
#include <unordered_set>
#include "SparseSet.hpp"
#include "Components.hpp"

class EntityManager
{
public:

	EntityManager()
	{
		m_Freelist.reserve(k_MaxEntities);
		for (int32_t id = k_MaxEntities - 1; id >= 0; --id)
		{
			m_Freelist.push_back(id);
		}
	}

	size_t make()
	{
		size_t id = m_Freelist.back();
		m_Freelist.pop_back();
		m_Entities.insert(id);
		return id;
	}

	template <typename C, typename ... Args>
	void add(size_t id, Args&&... args)
	{
		if constexpr (std::is_same_v<C, Components::Transform>)
		{
			m_Transforms.emplace(id, std::forward<Args>(args)...);
		}
		else if constexpr (std::is_same_v<C, Components::Velocity>)
		{
			m_Velocities.emplace(id, std::forward<Args>(args)...);
		}
		else if constexpr (std::is_same_v<C, Components::Collider>)
		{
			m_Coliders.emplace(id, std::forward<Args>(args)...);
		}
		else
		{
			static_assert(false);
		}
	}

	template <typename C>
	bool has(size_t id)
	{
		if constexpr (std::is_same_v<C, Components::Transform>)
		{
			return m_Transforms.contains(id);
		}
		else if constexpr (std::is_same_v<C, Components::Velocity>)
		{
			return m_Velocities.contains(id);
		}
		else if constexpr (std::is_same_v<C, Components::Collider>)
		{
			return m_Coliders.contains(id);
		}
		else
		{
			static_assert(false);
		}
	}

	template <typename C>
	C& get(size_t id)
	{
		if constexpr (std::is_same_v<C, Components::Transform>)
		{
			return m_Transforms[id];
		}
		else if constexpr (std::is_same_v<C, Components::Velocity>)
		{
			return m_Velocities[id];
		}
		else if constexpr (std::is_same_v<C, Components::Collider>)
		{
			return m_Coliders[id];
		}
		else
		{
			static_assert(false);
		}
	}

	template<typename C>
	auto& getSet()
	{
		if constexpr (std::is_same_v<C, Components::Transform>)
		{
			return m_Transforms;
		}
		else if constexpr (std::is_same_v<C, Components::Velocity>)
		{
			return m_Velocities;
		}
		else if constexpr (std::is_same_v<C, Components::Collider>)
		{
			return m_Coliders;
		}
		else
		{
			static_assert(false);
		}
	}

	void remove(size_t id)
	{
		if (!m_Entities.count(id)) return;

		m_Transforms.popIfContains(id);
		m_Velocities.popIfContains(id);
		m_Coliders.popIfContains(id);

		m_Freelist.push_back(id);
	}

	bool contains(size_t id)
	{
		assert(id < k_MaxEntities, "You're trying to check if an entity of impossible id exists");

		return m_Entities.count(id);
	}

private:

	static constexpr size_t k_MaxEntities = 2000;
	std::vector<size_t> m_Freelist;
	std::unordered_set<size_t> m_Entities;
	SparseSet<Components::Transform, k_MaxEntities> m_Transforms;
	SparseSet<Components::Velocity, k_MaxEntities> m_Velocities;
	SparseSet<Components::Collider, k_MaxEntities> m_Coliders;
};