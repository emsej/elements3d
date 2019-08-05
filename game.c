/* game.c --- game state, input handling, loading/saving, etc. */

#include "game.h"

/* Initialize the game---initialize SDL, create the window, load the
   main menu. */

void
game_initialize (game_t *game)
{
  game->config =
    (game_config_t)
    {
     .window_width = 640,
     .window_height = 480,
     .projection_width = 320,
     .projection_height = 240,
    };
  
  game->window = SDL_CreateWindow ("Veto3D", SDL_WINDOWPOS_UNDEFINED,
				   SDL_WINDOWPOS_UNDEFINED,
				   game->config.window_width,
				   game->config.window_height,
				   SDL_WINDOW_OPENGL);

  if (!game->window)
    FATAL ("Failed to create SDL window.");

  game->renderer = SDL_CreateRenderer (game->window, -1,
				       SDL_RENDERER_SOFTWARE);

  if (!game->renderer)
    FATAL ("Failed to create SDL renderer.");

  SDL_RenderSetLogicalSize (game->renderer,
			    game->config.projection_width,
			    game->config.projection_height);
  
  game->projection = SDL_CreateTexture (game->renderer,
					SDL_PIXELFORMAT_ARGB1555,
					SDL_TEXTUREACCESS_STREAMING,
					game->config.projection_width,
					game->config.projection_height);

  if (!game->projection)
    FATAL ("Failed to create projection texture.");

  game->pixel_buffer = malloc (game->config.projection_width
			       * game->config.projection_height
			       * sizeof (Uint16));
  
  game->latest_tick = SDL_GetTicks ();

  game->player =
    (player_t)
    {
     .x = 1,
     .y = 1,
     .direction = PLAYER_DIRECTION_UP
    };
  
  map_load_guile (&game->map, &game->player, "data/levels/example.ss");

  /* Because we render on input only, this is needed... */
  game_render (game);
  
  game->state = GAME_STATE_INGAME;
}

/* Process SDL events. */

void
game_process_event (game_t *game, SDL_Event *event)
{
  switch (event->type)
    {
    case SDL_QUIT:
      {
	game->state = GAME_STATE_QUIT;
      }
      break;
    }

  if (game->state == GAME_STATE_INGAME)
    {
      switch (event->type)
	{
	case SDL_KEYDOWN:
	  {
	    player_process_key (&game->player, game->map, event->key.keysym.sym);
	    game_render (game);
	  }
	  break;
	}
    }
}

/* Update the game, called each tick. */

void
game_update (game_t *game)
{
  unsigned int new_time = SDL_GetTicks ();
  game->delta_tick = new_time - game->latest_tick;
  game->latest_tick = new_time;

  /* Put game_render () after everything inside of here if the game
     state requires that the screen is updated each frame. */
}

/* Render the map, GUI, et cetera. */

void
game_render (game_t *game)
{
  SDL_SetRenderDrawColor (game->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear (game->renderer);

  render_projection (game->map, game->player, game->config, game->renderer, game->projection, game->pixel_buffer);
  render_minimap (game->map, game->player, game->renderer);
  
  SDL_RenderPresent (game->renderer);
}

/* Quit the game, free all allocated resources. */

void
game_quit (game_t *game)
{
  if (game->window)
    SDL_HideWindow (game->window);

  free (game->pixel_buffer);
  
  if (game->projection)
    SDL_DestroyTexture (game->projection);

  if (game->renderer)
    SDL_DestroyRenderer (game->renderer);

  if (game->window)
    SDL_DestroyWindow (game->window);

  game->state = GAME_STATE_QUIT;
}
