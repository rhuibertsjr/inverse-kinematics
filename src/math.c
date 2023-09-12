#include "math.h"

//- rhjr: Vector 2D
Vec2 vec2(float x, float y)
{
	Vec2 result = {{x, y}};
	return result;
}

Vec2 vec2_addition(Vec2 u, Vec2 v)
{
    Vec2 result;
    result.x = u.x + v.x;
    result.y = u.y + v.y;
    return result;
}

Vec2 vec2_subtract(Vec2 u, Vec2 v)
{
    Vec2 result;
    result.x = u.x - v.x;
    result.y = u.y - v.y;
    return result;
}

float vec2_dot_product(Vec2 u, Vec2 v)
{
    //- rhjr: The dot-product of can be geometrically interpreted as 
    // u * v = |u||v| * cos(\theta).
    return (u.x * v.x) + (u.y * v.y);
}

// - rhjr: Vector 3D
Vec3 vec3(float x, float y, float z)
{
	Vec3 result = {{x, y, z}};
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

// - rhjr: Vector helpers
Vec3 vec2_to_vec3(Vec2 v)
{
	Vec3 result = vec3(v.x, v.y, 0);
	return result;
}
