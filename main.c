/* main.c --- the main game file. */

#include <stdlib.h>

#include "game.h"

int
main ()
{
  SDL_Init (SDL_INIT_VIDEO);

  game_t game = { 0 };
  game_initialize (&game);

  while (game.state != GAME_STATE_QUIT)
    {
      SDL_Event event;
      while (SDL_PollEvent (&event))
	game_process_event (&game, &event);
      game_update (&game);
    }

  game_quit (&game);
  SDL_Quit ();
  
  return EXIT_SUCCESS;
}
