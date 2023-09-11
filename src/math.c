#include "math.h"

Vec2 vec2(float x, float y)
{
	Vec2 result = {{x, y}};
	return result;
}

float vec2_dot_product(Vec2 u, Vec2 v)
{
    return (u.x * v.x) + (u.y * v.y);
}

float vec2_distance(Vec2 u, Vec2 v)
{
	float dx = u.x - v.x;
	float dy = u.y - v.y;

    return sqrtf(dx * dx + dy * dy);
}

Vec3 vec3(float x, float y, float z)
{
	Vec3 result = {{x, y, z}};
	return result;
}

Vec3 vec2_to_vec3(Vec2 v)
{
	Vec3 result = vec3(v.x, v.y, 0);
	return result;
}

Vec3 vec3_cross_product(Vec3 u, Vec3 v)
{
    Vec3 result;
    result.x = (u.y * v.z) - (u.z * v.y);
    result.y = (u.z * v.x) - (u.x * v.z);
    result.z = (u.x * v.y) - (u.y * v.x);
    return result;
}
