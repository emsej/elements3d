#include "player.h"

/* Move player forward. */

void
player_move_forward (player_t *player, const map_t map)
{
  switch (player->direction)
    {
    case PLAYER_DIRECTION_UP:
      {
	if (map.walling[XY (player->x, player->y - 1, map.size)]
	    == 0)
	  player->y -= 1;
      }
      break;
    case PLAYER_DIRECTION_DOWN:
      {
	if (map.walling[XY (player->x, player->y + 1, map.size)]
	    == 0)
	player->y += 1;
      }
      break;
    case PLAYER_DIRECTION_LEFT:
      {
	if (map.walling[XY (player->x - 1, player->y, map.size)]
	    == 0)
	player->x -= 1;
      }
      break;
    case PLAYER_DIRECTION_RIGHT:
      {
	if (map.walling[XY (player->x + 1, player->y, map.size)]
	    == 0)
	player->x += 1;
      }
      break;
    }
}

/* Move player backward. */

void
player_move_backward (player_t *player, const map_t map)
{
  switch (player->direction)
    {
    case PLAYER_DIRECTION_UP:
      {
	if (map.walling[XY (player->x, player->y + 1, map.size)]
	    == 0)
	player->y += 1;
      }
      break;
    case PLAYER_DIRECTION_DOWN:
      {
	if (map.walling[XY (player->x, player->y - 1, map.size)]
	    == 0)
	player->y -= 1;
      }
      break;
    case PLAYER_DIRECTION_LEFT:
      {
	if (map.walling[XY (player->x + 1, player->y, map.size)]
	    == 0)
	player->x += 1;
      }
      break;
    case PLAYER_DIRECTION_RIGHT:
      {
	if (map.walling[XY (player->x - 1, player->y, map.size)]
	    == 0)
	player->x -= 1;
      }
      break;
    }  
}

/* Turn left. */

void
player_turn_left (player_t *player)
{
  switch (player->direction)
    {
    case PLAYER_DIRECTION_UP:
      {
	player->direction = PLAYER_DIRECTION_LEFT;
      }
      break;
    case PLAYER_DIRECTION_DOWN:
      {
	player->direction = PLAYER_DIRECTION_RIGHT;
      }
      break;
    case PLAYER_DIRECTION_LEFT:
      {
	player->direction = PLAYER_DIRECTION_DOWN;
      }
      break;
    case PLAYER_DIRECTION_RIGHT:
      {
	player->direction = PLAYER_DIRECTION_UP;
      }
      break;
    }    
}

/* Turn right. */

void
player_turn_right (player_t *player)
{
  switch (player->direction)
    {
    case PLAYER_DIRECTION_UP:
      {
	player->direction = PLAYER_DIRECTION_RIGHT;
      }
      break;
    case PLAYER_DIRECTION_DOWN:
      {
	player->direction = PLAYER_DIRECTION_LEFT;
      }
      break;
    case PLAYER_DIRECTION_LEFT:
      {
	player->direction = PLAYER_DIRECTION_UP;
      }
      break;
    case PLAYER_DIRECTION_RIGHT:
      {
	player->direction = PLAYER_DIRECTION_DOWN;
      }
      break;
    }  
}

/* Process key. */

void
player_process_key (player_t *player, const map_t map, SDL_Keycode key)
{
  switch (key)
    {
    case SDLK_w:
      {
	player_move_forward (player, map);
      }
      break;
    case SDLK_s:
      {
	player_move_backward (player, map);
      }
      break;
    case SDLK_a:
      {
	player_turn_left (player);
      }
      break;
    case SDLK_d:
      {
	player_turn_right (player);
      }
      break;
    }
}
