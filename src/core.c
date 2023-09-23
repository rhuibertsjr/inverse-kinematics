#include "core.h"

#include <stdio.h>
#include <string.h>

//- rhjr: arena allocator 
void
arena_init (Arena *arena, void *back_buffer, uint64_t size)
{
  arena->memory = (uint64_t*) back_buffer;
  arena->position = 0;
  arena->size = size;
}

void
*arena_alloc (Arena *arena, uint64_t size)
{
  if (arena->position + size <= arena->size)
  {
    // rhjr: cast because, void* size not defined in MSVC.
    void *ptr = (uint64_t*) arena->memory + arena->position;
    arena->position += size;
      
    memset(ptr, 0, size);
    return ptr;
  }

  return NULL;
}

void
arena_free (Arena *arena)
{
  arena->position = 0;
}

//- rhjr: limbed list 
void
limb_list_push (Arena *arena, LimbedList *list, Limb limb)
{
  LimbNode node = { limb, 0, list->last };
  LimbNode *ptr =
    (LimbNode*) arena_alloc(arena, sizeof(LimbNode));

  if (list->first == NULL)
  {
    list->first = list->last = ptr;
  }	
  else
  {
    list->last->next = ptr;
    list->last = ptr;
  }

  memcpy(ptr, &node, sizeof(LimbNode));
  list->limb_amount += 1;
}

//- rhjr: kinematics  
Vec2
kinematics_calculate_end_position (LimbedList *list)
{
  Vec2 previous_position = {0};
  float previous_angle = 0;

  for(LimbNode *current = list->first; current != NULL; current = current->next)
  {
    Limb *limb = &current->limb;

    // rhjr: tail position
    limb->tail_position = previous_position;

    // rhjr: head position
    limb->head_position.x = previous_position.x + limb->length * cosf(
      degrees_to_radians(limb->angle));

    limb->head_position.y = previous_position.y + limb->length * sinf(
      degrees_to_radians(limb->angle));

    // rhjr: update for next limb
    previous_position = limb->head_position;
    previous_angle = limb->angle;
  }

  return list->last->limb.head_position;

}

void
kinematics_cyclic_coordinate_descent (LimbedList *list, Vec2 target)
{
  Vec2 current_position = kinematics_calculate_end_position(list);
  float distance = {0};
  uint32_t tries = 0;

  // rhjr: loop throught the list in reverse. 
  LimbNode *current = list->last;

  do
  { 
    Limb *limb = &current->limb;
    Vec2 destination =
      vec2_normalize(vec2_subtract(target, current_position));
    Vec2 current_position_norm = vec2_normalize(limb->head_position);

    // rhjr: target angle
    float target_angle = vec2_dot_product(destination, current_position_norm);
    if (target_angle < 0.99999) // rhjr: when angle is 1, no rotation is needed.
    {
      // rhjr: determine direction.
      Vec3 direction =
        vec3_cross_product(
          vec3_lit(destination), vec3_lit(limb->head_position));
      float turn_angle = radians_to_degrees(acosf(target_angle));

      if (direction.z > 0.0f) // rhjr: turn clockwise
        limb->angle -= turn_angle;
      else if (direction.z < 0.0f) //rhjr: turn counter-clockwise
        limb->angle += turn_angle;
    }

    current_position = kinematics_calculate_end_position(list);

    // rhjr: next limb.
    if (current->prev != NULL)
      current = current->prev;
    else 
      current = list->last;

    distance = vec2_distance(target, current_position);

    printf("Limb %u: { x: %.2f, y: %.2f }\n",
      limb->id, limb->head_position.x, limb->head_position.y);
  }
  while(distance > CCD_POS_THRESHOLD && tries++ < CCD_MAX_TRIES);

  printf("End effector: { x: %.2f, y: %.2f }\n",
    list->last->limb.head_position.x, list->last->limb.head_position.y);

}

//- rhjr: 
int
main(void)
{
  uint64_t buffer[ARENA_COMMIT_SIZE] = {0};

  Arena arena = {0};
  arena_init(&arena, &buffer, ARENA_COMMIT_SIZE);

  // rhjr: initialize robot arm.
  LimbedList list = {0};
  limb_list_push(&arena, &list, limb_lit(0, 1, 90));
  limb_list_push(&arena, &list, limb_lit(1, 1, 45)); 
  limb_list_push(&arena, &list, limb_lit(2, 1,  0)); 

  // rhjr: forward kinematics 
  kinematics_calculate_end_position(&list);

  // rhjr: inverse kinematics
  kinematics_cyclic_coordinate_descent(&list, vec2_lit(2.5, 2.5));

  return 0;
}
