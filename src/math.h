#ifndef MATH_H
#define MATH_H

#include <math.h>

//- rhjr: types 
#define PI 3.14159265358979

//- rhjr: helpers 
#define radians_to_degrees(rads) ((rads) * (180 / PI))
#define degrees_to_radians(deg)  ((deg) * (PI / 180))

//- rhjr: vectors 
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

// rhjr: vector constructors
Vec2 vec2 (float x, float y);
Vec3 vec3 (float x, float y, float z);

#define vec2_lit(x, y) (Vec2) { x, y }
#define vec3_lit(v)    (Vec3) { v.x, v.y, 0 }

// rhjr: vector operations
Vec2  vec2_addition    (Vec2 u, Vec2 v); 
Vec2  vec2_subtract    (Vec2 u, Vec2 v); 
float vec2_dot_product (Vec2 u, Vec2 v);

float vec2_magnitude  (Vec2 v);
Vec2  vec2_normalize  (Vec2 v);
float vec2_distance   (Vec2 u, Vec2 v);

Vec3  vec3_cross_product (Vec3 u, Vec3 v);

// rhjr: vector helpers
Vec3 vec2_to_vec3 (Vec2 v);

#endif 
