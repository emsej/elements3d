#include "util.h"

/* Load bitmap from PATH. */

bitmap_t *
bitmap_load (const char *path)
{
  bitmap_t *bitmap = malloc (sizeof (bitmap_t));
  SDL_Surface *surface = SDL_LoadBMP (path);

  if (!surface)
    FATAL ("Couldn't load bitmap.");
    
  bitmap->width = surface->w;
  bitmap->height = surface->h;
  bitmap->pixels = malloc (bitmap->width * bitmap->height
			   * sizeof (Uint16));

  for (int x = 0; x < bitmap->width; ++x)
    for (int y = 0; y < bitmap->height; ++y)
      bitmap->pixels[XY (x, y, bitmap->width)] =
	get_pixel_color (surface, x, y);

  SDL_FreeSurface (surface);

  return bitmap;
}

/* Free BITMAP. */

void
bitmap_free (bitmap_t *bitmap)
{
  free (bitmap->pixels);
}

/* Get color of surface pixel at (X, Y). */

Uint16
get_pixel_color (SDL_Surface *surface, int x, int y)
{
  if (SDL_MUSTLOCK (surface))
    SDL_LockSurface (surface);

  int bpp = surface->format->BytesPerPixel;
  Uint8 *pixel_address = (Uint8 *) surface->pixels
    + XY (x, y, surface->w) * bpp;

  if (SDL_MUSTLOCK (surface))
    SDL_UnlockSurface (surface);

  return *(Uint16 *) pixel_address;
}

/* Shade color by some random magic number. */

void
shade_color (Uint16 *color)
{
  *color = (*color >> 1) & 8355;
}
