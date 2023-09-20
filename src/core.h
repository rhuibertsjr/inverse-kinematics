#ifndef CORE_H
#define CORE_H

#include <stdint.h>
#include "math.h"

//- rhjr: environment 
#if !defined(_WIN64) || !defined(_MSC_VER)
#warning "Only tested on Windows 11, with the MSVC compiler."
#endif

#define ARENA_COMMIT_SIZE 1024

//- rhjr: arena allocator 
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

//- rhjr: limbed list 
typedef struct Limb Limb;
struct Limb
{
  uint8_t id;
	float length;
	float angle;

  Vec2 tail_position;
	Vec2 head_position;
};

#define limb_lit(id, length, angle) (Limb) {id, length, angle, {0}, {0}}

typedef struct LimbNode LimbNode;
struct LimbNode
{
  Limb limb; 
  LimbNode *next;
  LimbNode *prev;
};

typedef struct LimbedList LimbedList; 
struct LimbedList
{
  LimbNode *first;
  LimbNode *last; //end effector

  uint32_t limb_amount;
};

void limb_list_push    (Arena *arena, LimbedList *list, Limb limb);

//- rhjr: kinematics 
#define CCD_MAX_TRIES     10
#define CCD_POS_THRESHOLD 1.0f

// rhjr: forward kinematics 
Vec2 kinematics_calculate_end_position (LimbedList *list);

// rhjr: inverse kinematics
void kinematics_cyclic_coordinate_descent (LimbedList *list, Vec2 target);

#endif
