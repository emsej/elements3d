#pragma once

#include <libguile.h>

#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "player.h"

/* Forward declarations. */
typedef struct player_t player_t;

/* Struct holding information about a map. */
typedef struct map_t
{
  unsigned int size;
  char *walling;
  char *flooring;
  char *ceiling;
  unsigned int number_of_bitmaps;
  bitmap_t **bitmaps;
} map_t;

void map_8x8 (map_t *map);
void map_load_guile (map_t *map, player_t *player, const char *path);
void map_free (map_t *map);
