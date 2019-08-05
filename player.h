#pragma once

#include <SDL2/SDL.h>

#include "map.h"
#include "util.h"

/* Forward declarations. */
typedef struct map_t map_t;

/* An enum storing direction of the player. */

typedef enum player_direction_t
{
 PLAYER_DIRECTION_UP,
 PLAYER_DIRECTION_DOWN,
 PLAYER_DIRECTION_LEFT,
 PLAYER_DIRECTION_RIGHT,
} player_direction_t;

/* Struct holding information about the player. */

typedef struct player_t
{
  unsigned int x, y;
  player_direction_t direction;
} player_t;

void player_move_forward (player_t *player, const map_t map);
void player_move_backward (player_t *player, const map_t map);
void player_turn_left (player_t *player);
void player_turn_right (player_t *player);
void player_process_key (player_t *player, const map_t map, SDL_Keycode key);
