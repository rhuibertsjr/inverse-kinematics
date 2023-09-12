#ifndef MATH_H
#define MATH_H

#include <math.h>

//------------------------------------------------------------------------------
//- rhjr: Types + helpers

#define PI 3.14159265358979

#define radians_to_degrees(rads) ((rads) * (180 / PI))
#define degrees_to_radians(deg)  ((deg) * (PI / 180))

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

Vec2 vec2_addition (Vec2 u, Vec2 v); 
Vec2 vec2_subtract (Vec2 u, Vec2 v); 

float vec2_dot_product (Vec2 u, Vec2 v);

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

Vec3 vec3 (float x, float y, float z);

Vec3 vec3_cross_product (Vec3 u, Vec3 v);

//------------------------------------------------------------------------------
//- rhjr: Vector helpers 
Vec3 vec2_to_vec3 (Vec2 v);

#endif 
