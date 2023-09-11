#ifndef MATH_H
#define MATH_H

#include <math.h>

//------------------------------------------------------------------------------
//- rhjr: Vector 2D

typedef union Vec2 Vec2;
union Vec2
{
	struct
	{
		float x;
		float y;
	};

	float v[2];
};

Vec2 vec2(float x, float y);

//- rhjr: The dot-product of two vectors can be geometrically interpreted as 
//            u * v = |u||v| * cos(\theta).
float vec2_dot_product(Vec2 u, Vec2 v);

//- rhjr Euclidean distance of two vectors.
float vec2_distance(Vec2 u, Vec2 v);

//------------------------------------------------------------------------------
//- rhjr: Vector 3D 

typedef union Vec3 Vec3;
union Vec3
{
	struct
	{
		float x;
		float y;
		float z;
	};

	float v[3];
};

Vec3 vec3(float x, float y, float z);
Vec3 vec3_cross_product(Vec3 u, Vec3 v);

Vec3 vec2_to_vec3(Vec2 v);

#endif 
