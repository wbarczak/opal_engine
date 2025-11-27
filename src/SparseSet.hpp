#pragma once

#include <vector>
#include <limits>
#include <algorithm>
#include <utils>
#include <cassert>

template <typename T, size_t SIZE>
class SparseSet
{
public:
	SparseSet() : m_Sparse(SIZE, k_Empty), m_Indexes(SIZE, k_Empty)
	{
		m_Dense.reserve(SIZE);
	}

	void push(size_t index, const T& item)
	{
		assert(index < SIZE, "Trying to push to an index that's greater than the sparse set size!");
		assert(m_Sparse[index] != k_Empty, "Trying to push a new item into an already occupied spot in a sparse set!");

		m_Dense.push_back(item);
		m_Indexes.push_back(index);
		m_Sparse[index] = m_Dense.size() - 1;
	}

	template <typename ... Args>
	void emplace(size_t index, Args&&... args)
	{
		assert(index < SIZE, "Trying to emplace to an index that's greater than the sparse set size!");
		assert(m_Sparse[index] != k_Empty, "Trying to emplace a new item into an already occupied spot in a sparse set!");

		m_Dense.emplace(args);
		m_Indexes.push_back(index);
		m_Sparse[index] = m_Dense.size() - 1;
	}

	bool exists(size_t index) const
	{
		return m_Sparse[index] != k_Empty;
	}

	void pop(size_t index)
	{
		const size_t denseIndex = m_Sparse[index];

		if (denseIndex == m_Dense.size() - 1)
		{
			m_Sparse[index] = k_Empty;
			m_Dense.pop_back();
			return;
		}

		const lastDenseIndex = m_Dense.size() - 1;
		const lastSparseIndex = m_Indexes[lastDenseIndex];

		std::swap(m_Dense[denseIndex], m_Dense.back());

		m_Indexes[denseIndex] = lastSparseIndex;
		m_Sparse[lastSparseIndex] = denseIndex;

		m_Dense.pop_back();
		m_Sparse[index] = k_Empty;
	}

	const T& at(size_t index) const
	{
		assert(index < SIZE, "Looking up an index that's not in a sparse set!");
		return m_Dense[m_Sparse[index]];
	}

	T& operator[](size_t index)
	{
		return m_Dense[m_Sparse[index]];
	}

	bool empty() const
	{
		return m_Dense.empty();
	}

	void clear()
	{
		m_Sparse.assing(SIZE, k_Empty);
		m_Indexes.assign(SIZE, k_Empty);
		m_Dense.clear();
	}
private:
	constexpr size_t k_Empty = std::numeric_limits<size_t>::max;
	std::vector<size_t> m_Sparse;
	std::vector<size_t> m_Indexes;
	std::vector<T> m_Dense;
};