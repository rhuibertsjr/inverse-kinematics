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

//- rhjr: LimbedList
void limb_list_push (Arena *arena, LimbedList *list, Limb limb)
{
	LimbNode node = { limb, 0 };
	LimbNode *ptr =
		(LimbNode*) arena_alloc(arena, sizeof(LimbNode));

	if (list->root == NULL) 
		list->root = list->last = ptr;
	else {
		list->last->next = ptr;
		list->last = ptr;
	}

	memcpy(ptr, &node, sizeof(LimbNode));

	list->limb_amount += 1;
}

// - rhjr: Cyclic Coordinate Descent
void cyclic_coordinate_descent (LimbedList *list, Vec2 endpoint)
{
	Vec2 end_pos = list->root->limb.position;

	//- rhjr: Forward inverse kinematics
	//  Ex = (l_1 * cos(a_1)) + (l_2 * cos(a_1 + a_2)) + ... (list->last).
	//  Ey = (l_1 * sin(a_1)) + (l_2 * sin(a_1 + a_2)) + ... (list->last).
	//
	for(LimbNode *current = list->root; current != NULL;
		current = current->next)
	{
		current->limb.position.x =
			end_pos.x + current->limb.length * cosf(current->limb.angle);

		current->limb.position.y =
			end_pos.y + current->limb.length * sinf(current->limb.angle);

		end_pos    = current->limb.position;
		end_pos.x += current->limb.angle;

		printf("Position limb: { %f, %f }\n", end_pos.x, end_pos.y);
	}
	printf("\n");
	printf("Position end effector: { %f, %f }\n", end_pos.x, end_pos.y);
}

//- rhjr: Entry point
int main(void)
{

#define ARENA_COMMIT_SIZE 1024

	uint64_t buffer[ARENA_COMMIT_SIZE] = {0};

	Arena arena = {0};
	arena_init(&arena, &buffer, ARENA_COMMIT_SIZE);

	//- rhjr: Initialize arm.
	LimbedList list = {0};
	limb_list_push(&arena, &list, limb_lit(4,  1,   {0}));
	limb_list_push(&arena, &list, limb_lit(4, -0.2, {0}));
	limb_list_push(&arena, &list, limb_lit(4,  0.5, {0}));

	//- rhjr: Forward kinematics 
	cyclic_coordinate_descent(&list, vec2(3, 3));

	return 0;
}
