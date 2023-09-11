#ifndef CORE_H
#define CORE_H

#include <stdint.h>
#include "math.h"

//------------------------------------------------------------------------------
//- rhjr: Arena Allocator

typedef struct Arena Arena;
struct Arena
{
	uint64_t *memory;
	uint32_t position;
	uint32_t size;
};

void   arena_init  (Arena *arena, void *back_buffer, uint64_t size);

void * arena_alloc (Arena *arena, uint64_t size);
void   arena_free  (Arena *arena);

//------------------------------------------------------------------------------
//- rhjr: LimbedList 

typedef struct Limb Limb;
struct Limb
{
	uint32_t length;
	float angle;

	Vec2 position;
};

#define limb_lit(l, a, v) (Limb) {l, a, v}

typedef struct LimbNode LimbNode;
struct LimbNode
{
	Limb limb; 
	LimbNode *next;
};

typedef struct LimbedList LimbedList; 
struct LimbedList
{
	LimbNode *root;
	LimbNode *last; //end effector

	uint32_t limb_amount;
};

void limb_list_push (Arena *arena, LimbedList *list, Limb limb);

//------------------------------------------------------------------------------
//- rhjr: Kinematica 

// - rhjr: Cyclic Coordinate Descent
void cyclic_coordinate_descent (LimbedList *list, Vec2 endpoint);

#endif
