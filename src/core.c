#include "core.h"
#if RAYLIB
  #include "..\inc\raylib.h"
#endif

#include <stdio.h>
#include <string.h>

//- rhjr
int draw(LimbedList *list)
{
#if RAYLIB
  const uint32_t ball_size = 10;
  const uint32_t limb_width = 20.0f;

  // rhjr: Flip the coordinates, that it becomes a graph.
  Vector2 limb_tail_pos = {
    (gfx_center_x + list->first->limb.tail_position.x),
    (gfx_center_y + list->first->limb.tail_position.y)
  };

  Vector2 limb_head_pos = {
    (gfx_center_x + list->first->limb.head_position.x),
    (gfx_center_y + list->first->limb.head_position.y)
  };

  DrawLineEx(limb_tail_pos, limb_head_pos, limb_width, LIGHTGRAY);
  DrawCircleV(limb_tail_pos, ball_size, BLACK);

  for(LimbNode *current = list->first; current != NULL; current = current->next)
  {
    Vector2 limb_tail_pos = {
      (gfx_center_x + current->limb.head_position.x),
      (gfx_center_y + current->limb.head_position.y)
    };

    Vector2 limb_head_pos = {
      (gfx_center_x + current->limb.tail_position.x),
      (gfx_center_y + current->limb.tail_position.y)
    };

    DrawLineEx(limb_tail_pos, limb_head_pos, limb_width, LIGHTGRAY);
    DrawCircleV(limb_tail_pos, ball_size, GRAY);
  }
#endif
  return 0;
}

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
  uint32_t iterations = 0;

  // rhjr: loop throught the list in reverse. 
  LimbNode *current = list->last;

  do
  { 
    Limb *limb = &current->limb;
    Vec2 destination =
      vec2_normalize(vec2_subtract(target, limb->tail_position));
    Vec2 current_position_norm = 
      vec2_normalize(vec2_subtract(current_position, limb->tail_position));

    // rhjr: target angle
    float target_angle = vec2_dot_product(destination, current_position_norm);

    if (target_angle < 0.99999) // rhjr: when angle is 1, no rotation is needed.
    {
      // rhjr: determine direction.
      Vec3 direction =
        vec3_cross_product(
          vec3_lit(destination), vec3_lit(current_position_norm));

      float turn_angle = radians_to_degrees(acosf(target_angle));

      // rhjr: damping -> restricting the the amount a limb can change.
      if (turn_angle > CCD_DAMPING_WIDTH)
      turn_angle = CCD_DAMPING_WIDTH;

      if (direction.z > 0.0f) // rhjr: turn clockwise
      {
        limb->angle -= CCD_CONSTRAINT(-135, turn_angle, 135);
      }
      else if (direction.z < 0.0f) //rhjr: turn counter-clockwise
      {
        limb->angle += CCD_CONSTRAINT(-135, turn_angle, 135);;
      }
    }

    current_position = kinematics_calculate_end_position(list);

    // rhjr: next limb.
    if (current->prev != NULL)
    current = current->prev;
    else 
      current = list->last;

    distance = vec2_distance(target, current_position);
  }
  while(distance > CCD_POS_THRESHOLD && ++iterations < CCD_MAX_ITERATION);

  printf("End effector: { x: %.2f, y: %.2f }\n",
         list->last->limb.head_position.x, list->last->limb.head_position.y);
  printf("Target: { x: %.2f, y: %.2f } after %d iterations\n",
         target.x, target.y, iterations);
}


//- rhjr
int
main(void)
{
  uint64_t buffer[ARENA_COMMIT_SIZE] = {0};
  
  Arena arena = {0};
  arena_init(&arena, &buffer, ARENA_COMMIT_SIZE);
  
  // rhjr: initialize robot arm.
  LimbedList list = {0};
  limb_list_push(&arena, &list, limb_lit(0, 100, 90));
  limb_list_push(&arena, &list, limb_lit(1, 100, 45)); 
  limb_list_push(&arena, &list, limb_lit(2, 100,  0)); 
  
  // rhjr: forward kinematics 
  Vec2 current_position_temp = kinematics_calculate_end_position(&list);

  #if RAYLIB
  InitWindow(GFX_SCREEN_WIDTH, GFX_SCREEN_HEIGHT, GFX_WINDOW_TITLE);
  SetTargetFPS(GFX_TARGET_FPS);              
   
  Vector2 target = { -100.0f, -100.0f };
  Vector2 current_position = {
    current_position_temp.x,
    current_position_temp.y
  };

  while (!WindowShouldClose())    
  {
    target = GetMousePosition();

    BeginDrawing();

    ClearBackground(RAYWHITE);

    draw(&list);
    DrawCircleV(target, 10, RED);

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
      Vector2 new_target = {
        ((target.x - (GFX_SCREEN_WIDTH / 2))),
        ((target.y - (GFX_SCREEN_HEIGHT / 2)))
      };

      kinematics_cyclic_coordinate_descent(
        &list, vec2_lit(new_target.x, new_target.y));
      
      current_position_temp = kinematics_calculate_end_position(&list);
    }

    EndDrawing();
  }

  CloseWindow();       
  #else
  
  // rhjr: inverse kinematics
    kinematics_cyclic_coordinate_descent(
      &list, vec2_lit(230, 150));

#endif
  
  return 0;
}
