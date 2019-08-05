#pragma once

#include <SDL2/SDL.h>

#include "player.h"
#include "render.h"
#include "util.h"

/* Enum holding the state of the game. */

typedef enum game_state_t
  {
   GAME_STATE_QUIT = -1,
   GAME_STATE_INITIALIZING = 0,
   GAME_STATE_MAIN_MENU,
   GAME_STATE_INGAME
  } game_state_t;

/* Struct holding all configuration information of the game. */

typedef struct game_config_t
{
  unsigned int window_width, window_height;
  unsigned int projection_width, projection_height;
} game_config_t;

/* Struct holding all information about the current state of the
   game. */

typedef struct game_t
{
  game_state_t state;
  game_config_t config;

  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *projection;

  unsigned int latest_tick, delta_tick;

  Uint16 *pixel_buffer;

  /* Actual game data. */
  player_t player;
  map_t map;
} game_t;

void game_initialize (game_t *game);
void game_process_event (game_t *game, SDL_Event *event);
void game_update (game_t *game);
void game_render (game_t *game);
void game_quit (game_t *game);
