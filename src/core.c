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
	// TODO
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
	limb_list_push(&arena, &list, limb_lit(vec2(1, 1), 4, 1));
	limb_list_push(&arena, &list, limb_lit(vec2(2, 2), 4, -0.2));
	limb_list_push(&arena, &list, limb_lit(vec2(3, 1), 4, 0.5));

	//- rhjr: Forward kinematics 
	cyclic_coordinate_descent(&list, vec2(3, 3));

	return 0;
}
