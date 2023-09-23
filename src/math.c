#include "math.h"

Vec2
vec2 (float x, float y)
{
	Vec2 result = {{x, y}};
	return result;
}

Vec3
vec3 (float x, float y, float z)
{
	Vec3 result = {{x, y, z}};
	return result;
}

Vec2
vec2_addition (Vec2 u, Vec2 v)
{
  Vec2 result;
  result.x = u.x + v.x;
  result.y = u.y + v.y;
  return result;
}

Vec2
vec2_subtract (Vec2 u, Vec2 v)
{
  Vec2 result;
  result.x = u.x - v.x;
  result.y = u.y - v.y;
  return result;
}

float
vec2_dot_product (Vec2 u, Vec2 v)
{
  // rhjr: The dot-product of can be geometrically interpreted as 
  //         u * v = |u||v| * cos(\theta).
  return (u.x * v.x) + (u.y * v.y);
}

float
vec2_magnitude (Vec2 v)
{
  return sqrtf(v.x * v.x + v.y * v.y);
}

Vec2
vec2_normalize (Vec2 v)
{
  float magnitude = vec2_magnitude(v);
  if (magnitude == 0.0) return v;

  v.x /= magnitude;
  v.y /= magnitude;

  return v;
}

float
vec2_distance (Vec2 u, Vec2 v)
{
  // rhjr: Euclidean distance
  float distance_x = u.x - v.x;
  float distance_y = u.y - v.y;
  return sqrtf(distance_x * distance_x + distance_y * distance_y);
}

Vec3
vec3_cross_product (Vec3 u, Vec3 v)
{
  Vec3 result;
  result.x = (u.y * v.z) - (u.z * v.y);
  result.y = (u.z * v.x) - (u.x * v.z);
  result.z = (u.x * v.y) - (u.y * v.x);
  return result;
}

Vec3
vec2_to_vec3 (Vec2 v)
{
	Vec3 result = vec3(v.x, v.y, 0);
	return result;
}
