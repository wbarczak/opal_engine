#pragma once

#include <vector>
#include <unordered_set>
#include <tuple>
#include "SparseSet.hpp"
#include "Components.hpp"

//rewrite this to use a tuple so there is no need for if constexprs in every function when I just add something simple

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

		m_Transforms.popIfContains(id);
		m_Velocities.popIfContains(id);
		m_Coliders.popIfContains(id);

		m_Freelist.push_back(id);
	}

	template <typename C, typename ... Args>
	void add(size_t id, Args&&... args)
	{
		if constexpr (std::is_same_v<C, Comp::Transform>)
		{
			m_Transforms.emplace(id, std::forward<Args>(args)...);
		}
		else if constexpr (std::is_same_v<C, Comp::Velocity>)
		{
			m_Velocities.emplace(id, std::forward<Args>(args)...);
		}
		else if constexpr (std::is_same_v<C, Comp::Collider>)
		{
			m_Coliders.emplace(id, std::forward<Args>(args)...);
		}
		else
		{
			static_assert(false);
		}
	}

	template <typename C>
	void remove(size_t id)
	{
		if constexpr (std::is_same_v<C, Comp::Transform>)
		{
			m_Transforms.popIfContains(id);
		}
		else if constexpr (std::is_same_v<C, Comp::Velocity>)
		{
			m_Velocities.popIfContains(id);
		}
		else if constexpr (std::is_same_v<C, Comp::Collider>)
		{
			m_Coliders.popIfContains(id);
		}
		else
		{
			static_assert(false);
		}
	}

	template <typename C>
	bool has(size_t id)
	{
		if constexpr (std::is_same_v<C, Comp::Transform>)
		{
			return m_Transforms.contains(id);
		}
		else if constexpr (std::is_same_v<C, Comp::Velocity>)
		{
			return m_Velocities.contains(id);
		}
		else if constexpr (std::is_same_v<C, Comp::Collider>)
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
		if constexpr (std::is_same_v<C, Comp::Transform>)
		{
			return m_Transforms[id];
		}
		else if constexpr (std::is_same_v<C, Comp::Velocity>)
		{
			return m_Velocities[id];
		}
		else if constexpr (std::is_same_v<C, Comp::Collider>)
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
		if constexpr (std::is_same_v<C, Comp::Transform>)
		{
			return m_Transforms;
		}
		else if constexpr (std::is_same_v<C, Comp::Velocity>)
		{
			return m_Velocities;
		}
		else if constexpr (std::is_same_v<C, Comp::Collider>)
		{
			return m_Coliders;
		}
		else
		{
			static_assert(false);
		}
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
	SparseSet<Comp::Transform, k_MaxEntities> m_Transforms;
	SparseSet<Comp::Velocity, k_MaxEntities> m_Velocities;
	SparseSet<Comp::Collider, k_MaxEntities> m_Coliders;
	SparseSet<Comp::Controlable, k_MaxEntities> m_Controlables;
};