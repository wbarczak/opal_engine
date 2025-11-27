#pragma once

#include <string>
#include <cmath>
#include <cassert>
#include <iostream>
#include <cstdint>
#include <numbers>
#include <algorithm>
#include <optional>
#include <chrono>

struct Vec2i
{
	int32_t x{ 0 };
	int32_t y{ 0 };
};

struct Vec2
{
	float x;
	float y;

	Vec2(float x = 0.0f, float y = 0.0f) noexcept :
		x(x), y(y) {}
	Vec2(const Vec2i& v) noexcept :
		x(v.x), y(v.y) {}

	inline float dot(const Vec2& v) const noexcept;
	inline float length() const noexcept;
	inline static Vec2 direction(float a) noexcept;

	inline Vec2& normalize() noexcept;
	inline Vec2 normalized() const noexcept;

	inline operator Vec2i() const noexcept;
};

struct Cir;

struct Rect
{
	Vec2 pos;
	float w;
	float h;

	Rect(float x = 0.0f, float y = 0.0f, float width = 0.0f, float height = 0.0f) noexcept :
		pos(x, y), w(width), h(height) {}
	Rect(Vec2 position, float width, float height) noexcept :
		pos(position), w(width), h(height) {}

	float& x() { return pos.x; }
	float& y() { return pos.y; }

	inline bool colide(const Vec2& v) noexcept;
	inline bool colide(const Rect& r) noexcept;
	inline bool colide(const Cir& c) noexcept;

	inline Vec2 resolve(const Rect& r) noexcept;
	inline Vec2 resolve(const Cir& c) noexcept;
};

struct Cir
{
	Vec2 pos;
	float rad;

	Cir(float x = 0.0f, float y = 0.0f, float radius = 0.0f) noexcept :
		pos(x, y), rad(radius) {}
	Cir(Vec2 position, float radius) noexcept :
		pos(position), rad(radius) {}

	float& x() { return pos.x; }
	float& y() { return pos.y; }

	inline bool colide(const Vec2& v) noexcept;
	inline bool colide(const Rect& r) noexcept;
	inline bool colide(const Cir& c) noexcept;

	inline Vec2 resolve(const Rect& r) noexcept;
	inline Vec2 resolve(const Cir& c) noexcept;
};

struct Col
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;

	constexpr Col(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) :
		r(r), g(g), b(b), a(a) {}
};

namespace Colors
{
	constexpr Col Black(0,0,0,255);
	constexpr Col White(255,255,255,255);
	constexpr Col Gray(30,30,30,255);
	constexpr Col LightGray(60,60,60,255);
	constexpr Col DarkTint(0,0,0,50);
	constexpr Col Red(255,0,0,255);
	constexpr Col Green(0,255,0,255);
	constexpr Col Blue(0,0,255,255);
}

class SecClock
{
public:

	SecClock() : m_StartingTime(std::chrono::steady_clock::now()) {}

	float restart()
	{
		auto now = std::chrono::steady_clock::now();
		float difference = std::chrono::duration<float>(now - m_StartingTime).count();
		m_StartingTime = now;

		return difference;
	}

	float elapsed()
	{
		auto now = std::chrono::steady_clock::now();
		return std::chrono::duration<float>(now - m_StartingTime).count();
	}

private:

	std::chrono::steady_clock::time_point m_StartingTime;
};

inline std::ostream& operator<<(std::ostream& os, const Vec2& v) {
	return os << "(x: " << v.x << ", y: " << v.y << ")";
}

inline Vec2 operator-(const Vec2& a) noexcept
{
	return Vec2(
		-a.x,
		-a.y
	);
}

inline Vec2 operator+(const Vec2& a, const Vec2& b) noexcept
{
	return Vec2(
		a.x + b.x,
		a.y + b.y
	);
}

inline Vec2 operator-(const Vec2& a, const Vec2& b) noexcept
{
	return Vec2(
		a.x - b.x,
		a.y - b.y
	);
}

inline Vec2& operator+=(Vec2& a, const Vec2& b) noexcept
{
	a.x += b.x;
	a.y += b.y;
	return a;
}

inline Vec2& operator-=(Vec2& a, const Vec2& b) noexcept
{
	a.x -= b.x;
	a.y -= b.y;
	return a;
}

inline Vec2 operator*(const Vec2& v, float n) noexcept
{
	return Vec2(
		v.x * n,
		v.y * n
	);
}

inline Vec2 operator*(float n, const Vec2& v) noexcept
{
	return Vec2(
		v.x * n,
		v.y * n
	);
}

inline Vec2 operator/(const Vec2& v, float n) noexcept
{
	assert(n);

	return Vec2(
		v.x / n,
		v.y / n
	);
}

inline Vec2& operator*=(Vec2& v, float n) noexcept
{
	v.x *= n;
	v.y *= n;
	return v;
}

inline Vec2& operator/=(Vec2& v, float n) noexcept
{
	assert(n);
	v.x /= n;
	v.y /= n;
	return v;
}

float Vec2::dot(const Vec2& v) const noexcept { return x * v.x + y * v.y; }
float Vec2::length() const noexcept { return sqrtf(dot(*this)); }
Vec2 Vec2::direction(float a) noexcept { return Vec2(cosf(a), sinf(a)); }

Vec2& Vec2::normalize() noexcept
{
	if (x == 0 && y == 0) return *this;
	*this = 1.0f / length() * *this;
	return *this;
}

Vec2 Vec2::normalized() const noexcept
{
	if (x == 0 && y == 0) return Vec2(0.0f, 0.0f);
	return 1.0f / length() * *this;
}

Vec2::operator Vec2i() const noexcept
{
	return Vec2i(
		static_cast<int32_t>(x),
		static_cast<int32_t>(y)
	);
}

inline std::ostream& operator<<(std::ostream& os, const Rect& r) {
	return os << "(x: " << r.pos.x << ", y: " << r.pos.y << ", w: " << r.w << ", h: " << r.h << ")";
}

inline std::ostream& operator<<(std::ostream& os, const Cir& c) {
	return os << "(x: " << c.pos.x << ", y: " << c.pos.y << ", r: " << c.rad << ")";
}

bool Rect::colide(const Vec2& v) noexcept
{
	return v.x >= pos.x
		&& v.x <= pos.x + w
		&& v.y >= pos.y
		&& v.y <= pos.y + h;
}

bool Rect::colide(const Rect& r) noexcept
{
	return pos.x     <= r.pos.x + r.w
		&& pos.x + w >= r.pos.x
		&& pos.y     <= r.pos.y + r.h
		&& pos.y + h >= r.pos.y;
}

bool Rect::colide(const Cir& c) noexcept
{
	Vec2 testPoint(
		std::clamp(c.pos.x, pos.x, pos.x + w),
		std::clamp(c.pos.y, pos.y, pos.y + h)
	);
	Vec2 difference = testPoint - c.pos;
	return difference.dot(difference) <= c.rad * c.rad;
}

bool Cir::colide(const Vec2& v) noexcept
{
	return (Vec2(pos.x, pos.y) - v).length() <= rad;
}

bool Cir::colide(const Rect& r) noexcept
{
	Vec2 testPoint(
		std::clamp(pos.x, r.pos.x, r.pos.x + r.w),
		std::clamp(pos.y, r.pos.y, r.pos.y + r.h)
	);
	Vec2 difference = testPoint - pos;
	return difference.dot(difference) <= rad * rad;
}

bool Cir::colide(const Cir& c) noexcept
{
	return (pos - c.pos).length() <= rad + c.rad;
}

Vec2 Rect::resolve(const Rect& r) noexcept
{
	bool noOverlap =
		pos.x >= r.pos.x + r.w ||
		pos.x + w <= r.pos.x ||
		pos.y >= r.pos.y + r.h ||
		pos.y + h <= r.pos.y;

	if (noOverlap) return {};

	float left = (r.pos.x + r.w) - pos.x;
	float right = (pos.x + w) - r.pos.x;
	float top = (r.pos.y + r.h) - pos.y;
	float bottom = (pos.y + h) - r.pos.y;

	float minX = std::min(left, right);
	float minY = std::min(top, bottom);

	Vec2 resolution{};

	if (minX < minY)
	{
		if (left < right)
		{
			resolution.x = -left;
		}
		else
		{
			resolution.x = right;
		}
	}
	else
	{
		if (top < bottom)
		{
			resolution.y = -top;
		}
		else
		{
			resolution.y = bottom;
		}
	}

	return resolution;
}

Vec2 Rect::resolve(const Cir& c) noexcept
{
	Vec2 closest(
		std::clamp(c.pos.x, pos.x, pos.x + w),
		std::clamp(c.pos.y, pos.y, pos.y + h)
	);
	Vec2 difference = closest - c.pos;
	float distanceSquared = difference.dot(difference);

	if (distanceSquared >= c.rad * c.rad)
	{
		return {};
	}

	float distance = sqrtf(distanceSquared);

	if (distance == 0.0f)
	{
		float left = c.pos.x - pos.x;
		float right = pos.x + w - c.pos.x;
		float top = c.pos.y - pos.y;
		float bottom = pos.y + h - c.pos.y;

		float minX = std::min(left, right);
		float minY = std::min(top, bottom);

		if (minX < minY)
		{
			if (left < right)
			{
				return Vec2(c.rad - left, 0.0f);
			}

			return Vec2(-(c.rad - right), 0.0f);
		}

		if (top < bottom)
		{
			return Vec2(0.0f, -(c.rad - top));
		}

		return Vec2(0.0f, -(c.rad - bottom));
	}

	Vec2 normal = difference / distance;
	float penetration = c.rad - distance;

	return normal * penetration;
}

Vec2 Cir::resolve(const Rect& r) noexcept
{
	Vec2 closest(
		std::clamp(pos.x, r.pos.x, r.pos.x + r.w),
		std::clamp(pos.y, r.pos.y, r.pos.y + r.h)
	);
	Vec2 difference = closest - pos;
	float distanceSquared = difference.dot(difference);

	if (distanceSquared >= rad * rad)
	{
		return {};
	}

	float distance = sqrtf(distanceSquared);

	if (distance == 0.0f)
	{
		float left = pos.x - r.pos.x;
		float right = r.pos.x + r.w - pos.x;
		float top = pos.y - r.pos.y;
		float bottom = r.pos.y + r.h - pos.y;

		float minX = std::min(left, right);
		float minY = std::min(top, bottom);

		if (minX < minY)
		{
			if (left < right)
			{
				return Vec2(rad - left, 0.0f);
			}

			return Vec2(-(rad - right), 0.0f);
		}

		if (top < bottom)
		{
			return Vec2(0.0f, -(rad - top));
		}

		return Vec2(0.0f, -(rad - bottom));
	}

	Vec2 normal = difference / distance;
	float penetration = rad - distance;

	return normal * -penetration;
}

Vec2 Cir::resolve(const Cir& c) noexcept
{
	Vec2 difference = pos - c.pos;
	float distanceSquared = difference.dot(difference);
	float radiusSum = rad + c.rad;

	if (distanceSquared >= radiusSum * radiusSum)
	{
		return {};
	}

	float distance = sqrtf(distanceSquared);

	if (distance == 0.0f)
	{
		return (radiusSum, 0.0f);
	}

	Vec2 normal = difference / distance;
	float penetration = radiusSum - distance;

	return normal * penetration;
}