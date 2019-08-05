#pragma once

#include <SDL2/SDL.h>

#define XY(x, y, width) (((y) * (width)) + (x))
#define CLAMP(n, low, high) (((n) > (high)) ? (high) : (((n) < (low)) ? (low) : (n)))

#define FLNO __FILE__, __LINE__
#define INFO(message) (fprintf (stderr, "\x1B[34m%s:%d: %s\x1B[0m", FLNO, message))
#define FATAL(message) (fprintf (stderr, "\x1B[31m%s:%d: %s\x1B[0m", FLNO, message), exit (-1))

/* 2D integer vector. */

typedef struct vec2i_t
{
  int x, y;
} vec2i_t;

/* 2D float vector. */

typedef struct vec2f_t
{
  float x, y;
} vec2f_t;

/* Bitmap. */

typedef struct bitmap_t
{
  int width, height;
  Uint16 *pixels;
} bitmap_t;

bitmap_t *bitmap_load (const char *path);
void bitmap_free (bitmap_t *bitmap);
Uint16 get_pixel_color (SDL_Surface *surface, int x, int y);
void shade_color (Uint16 *color);
