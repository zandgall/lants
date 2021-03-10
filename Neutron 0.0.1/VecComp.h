#pragma once
#include <glm/glm.hpp>
struct vec_comp
{
	bool operator()(const glm::u32vec2 &lhs, const glm::u32vec2 &rhs) const
	{
		if (lhs.x == rhs.x)
			return lhs.y > rhs.y;

		return lhs.x < rhs.x;
	}
};

struct vec3_comp
{
	bool operator()(const glm::ivec3 &lhs, const glm::ivec3 &rhs) const
	{
		if (lhs.x == rhs.x)
			if (lhs.y == rhs.y)
				return lhs.z > rhs.z;
			else return lhs.y > rhs.y;

		return lhs.x < rhs.x;
	}
};