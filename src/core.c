#include "core.h"
#include <stdio.h>
#include <string.h>

//- rhjr: Arena allocator
void arena_init(Arena *arena, void *back_buffer, uint64_t size)
{
	arena->memory = (uint64_t*) back_buffer;
	arena->position = 0;
	arena->size = size;
}

void * arena_alloc(Arena *arena, uint64_t size)
{
	if (arena->position + size <= arena->size)
	{
		//- rhjr: cast because, void* not defined in MSVC.
		void *ptr = (uint64_t*) arena->memory + arena->position;
		arena->position += size;

		memset(ptr, 0, size);
		return ptr;
	}

	return NULL;
}

void arena_free(Arena *arena)
{
	arena->position = 0;
}

//- rhjr: Limb
void limb_print(Limb *limb)
{
	printf(
		"Limb at {x: %f, y: %f }:\n  length: %f;\n  angle: %f; \n\n",
		limb->position.x, limb->position.y, limb->length, limb->angle);
}

//- rhjr: LimbedList
void limb_list_push (Arena *arena, LimbedList *list, Limb limb)
{
	LimbNode node = { limb, 0 };
	LimbNode *ptr =
		(LimbNode*) arena_alloc(arena, sizeof(LimbNode));

	if (list->root == NULL)
	{
		list->root = list->last = ptr;
	}	
	else
	{
		list->last->next = ptr;
		list->last = ptr;
	}

	memcpy(ptr, &node, sizeof(LimbNode));

	list->limb_amount += 1;
}

Vec2 limb_list_calculate_end_effector(LimbedList *list)
{
	Vec2 end_effector_position = list->root->limb.position;

	Vec2 previous_position = {0};
	float previous_angle = 0;

	//- rhjr: forward kinematics
	//  Ex = (l_1 * sin(a_1)) + (l_2 * cos(a_1 + a_2)) + ... (list->last).
	//  Ey = (l_1 * cos(a_1)) + (l_2 * sin(a_1 + a_2)) + ... (list->last).
	//
	for(LimbNode *current = list->root; current != NULL;
		current = current->next)
	{
		Limb *limb = &current->limb;

		//- rhjr: if multiple limbs are defined, then use the last one as root.
		limb->position =
			vec2_addition(limb->position, previous_position);
		limb->angle += previous_angle;

		//- rhjr: calculate the position of limb.
		limb->position.x =
			limb->position.x + limb->length * sinf(
				degrees_to_radians(limb->angle));

		limb->position.y =
			limb->position.y + limb->length * cosf(
				degrees_to_radians(limb->angle));

		printf(
			"Limb at {x: %.2f, y: %.2f }:\n  length: %2.f;\n  angle: %2.f;\n",
			limb->position.x, limb->position.y, limb->length, limb->angle);

		previous_position = limb->position;
		previous_angle = limb->angle;
	}

	return end_effector_position;
}

// - rhjr: Cyclic Coordinate Descent
void cyclic_coordinate_descent (LimbedList *list, Vec2 target)
{
	//- rhjr: inverse kinematics
}

#define ARENA_COMMIT_SIZE 1024

//- rhjr: Entry point
int main(void)
{
	uint64_t buffer[ARENA_COMMIT_SIZE] = {0};

	Arena arena = {0};
	arena_init(&arena, &buffer, ARENA_COMMIT_SIZE);

	//- rhjr: Initialize arm.
	LimbedList list = {0};
	limb_list_push(&arena, &list, limb_lit(1.414,  45));
	limb_list_push(&arena, &list, limb_lit(1,      45)); 
	limb_list_push(&arena, &list, limb_lit(1.414,  45)); 

	//- rhjr: Forward kinematics 
	limb_list_calculate_end_effector(&list);

	return 0;
}
