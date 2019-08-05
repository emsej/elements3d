#pragma once

#include <SDL2/SDL.h>

#include "game.h"
#include "player.h"
#include "map.h"
#include "util.h"

/* Forward declarations. */
typedef struct game_config_t game_config_t;

void render_minimap (const map_t map, const player_t player, SDL_Renderer *renderer);
void render_projection (const map_t map, const player_t player, const game_config_t config, SDL_Renderer *renderer, SDL_Texture *projection, Uint16 *pixel_buffer);
