/* render.c --- rendering code. */

#include "render.h"

/* Render a top down projection of the map. */

void
render_minimap (const map_t map, const player_t player, SDL_Renderer *renderer)
{
  SDL_SetRenderDrawColor (renderer, 255, 255, 255, 255);
  for (unsigned int x = 0; x < map.size; ++x)
    {
      for (unsigned int y = 0; y < map.size; ++y)
	{
	  if (map.walling[XY (x, y, map.size)] == 1)
	    {
	      SDL_Rect rect;
	      rect.x = x * 16;
	      rect.y = y * 16;
	      rect.w = 16;
	      rect.h = 16;
      
	      SDL_RenderFillRect (renderer, &rect);
	    }
	}
    }

  /* And draw the player position. */

  SDL_SetRenderDrawColor (renderer, 255, 0, 0, 255);
  
  SDL_Rect rect;
  rect.x = player.x * 16;
  rect.y = player.y * 16;
  rect.w = 16;
  rect.h = 16;

  SDL_RenderFillRect (renderer, &rect);

  /* Also draw the direction the player is facing. */
  rect.w = 8;
  rect.h = 8;
  
  switch (player.direction)
    {
    case PLAYER_DIRECTION_UP:
      {
	rect.y -= 8;
	rect.x += 4;
      }
      break;
    case PLAYER_DIRECTION_DOWN:
      {
	rect.y += 16;
	rect.x += 4;
      }
      break;
    case PLAYER_DIRECTION_LEFT:
      {
	rect.x -= 8;
	rect.y += 4;
      }
      break;
    case PLAYER_DIRECTION_RIGHT:
      {
	rect.x += 16;
	rect.y += 4;
      }
      break;
    }

  SDL_SetRenderDrawColor (renderer, 0, 0, 255, 255);
  SDL_RenderFillRect (renderer, &rect);
}

/* Render a raycasted projection of map. */

void
render_projection (const map_t map, const player_t player, const game_config_t config,
		   SDL_Renderer *renderer, SDL_Texture *projection, Uint16 *pixel_buffer)
{
  vec2f_t direction = { 0 };
  vec2f_t plane = { 0 };
  
  switch (player.direction)
    {
    case PLAYER_DIRECTION_UP:
      {
	direction =
	  (vec2f_t)
	  {
	   .x = 0.f,
	   .y = -1.f
	  };

	plane =
	  (vec2f_t)
	  {
	   .x = .66f,
	   .y = 0.f
	  };
      }
      break;
    case PLAYER_DIRECTION_DOWN:
      {
	direction =
	  (vec2f_t)
	  {
	   .x = 0.f,
	   .y = 1.f
	  };

	plane =
	  (vec2f_t)
	  {
	   .x = -.66f,
	   .y = 0.f
	  };
      }
      break;
    case PLAYER_DIRECTION_LEFT:
      {
	direction =
	  (vec2f_t)
	  {
	   .x = -1.f,
	   .y = 0.f
	  };

	plane =
	  (vec2f_t)
	  {
	   .x = 0.f,
	   .y = -.66f
	  };
      }
      break;
    case PLAYER_DIRECTION_RIGHT:
      {
	direction =
	  (vec2f_t)
	  {
	   .x = 1.f,
	   .y = 0.f
	  };

	plane =
	  (vec2f_t)
	  {
	   .x = 0.f,
	   .y = .66f
	  };
      }
      break;
    }

  vec2f_t player_position_adjusted =
    (vec2f_t)
    {
     .x = (float) player.x + .5f,
     .y = (float) player.y + .5f
    };
  
  for (unsigned int x = 0; x < config.projection_width; ++x)
    {
      /* Clear the pixel buffer. */
      for (unsigned int y = 0; y < config.projection_height; ++y)
	  pixel_buffer[XY (x, y, config.projection_width)] = 0;

      /* X coordinate in camera space. */
      float camera_x = 2.f * x / config.projection_width - 1;
      /* Direction of the ray, aka where are we casting? */
      vec2f_t ray_direction =
	(vec2f_t)
	{
	 .x = direction.x + plane.x * camera_x,
	 .y = direction.y + plane.y * camera_x
	};
      /* Position on the map grid. */
      vec2i_t map_position =
	(vec2i_t)
	{
	 .x = player.x,
	 .y = player.y
	};
      /* Distance from current position to next side of the grid. */
      vec2f_t side_distance = { 0 };
      vec2f_t delta_distance =
	(vec2f_t)
	{
	 .x = fabsf (1.f / ray_direction.x),
	 .y = fabsf (1.f / ray_direction.y)
	};
      float perp_wall_distance = 0; /* ??? */
      /* What direction to step in (either +1 or -1) */
      vec2i_t step_direction = { 0 };
      /* Have we hit a wall? */
      int hit = 0;
      /* Was a NS or EW wall hit? */
      int side = 0;

      /* Calculate the step direction and the initial side
	 distance. */
      if (ray_direction.x < 0)
	{
	  step_direction.x = -1;
	  side_distance.x = (player_position_adjusted.x - map_position.x)
	    * delta_distance.x;
	}
      else
	{
	  step_direction.x = 1;
	  side_distance.x = (map_position.x + 1.0
			     - player_position_adjusted.x)
	    * delta_distance.x;
	}

      if (ray_direction.y < 0)
	{
	  step_direction.y = -1;
	  side_distance.y = (player_position_adjusted.y - map_position.y)
	    * delta_distance.y;
	}
      else
	{
	  step_direction.y = 1;
	  side_distance.y = (map_position.y + 1.0
			     - player_position_adjusted.y)
	    * delta_distance.y;
	}

      /* Perform DDA. */
      while (hit == 0)
	{
	  /* Jump to next map square. */
	  if (side_distance.x < side_distance.y)
	    {
	      side_distance.x += delta_distance.x;
	      map_position.x += step_direction.x;
	      side = 0;
	    }
	  else
	    {
	      side_distance.y += delta_distance.y;
	      map_position.y += step_direction.y;
	      side = 1;
	    }

	  /* Check if we hit the edge of the map, if not, check if we
	     hit a wall. */
	  if (map.walling[XY (map_position.x, map_position.y,
			      map.size)] > 0)
	    hit = 1;
	}

      /* Calculate distance projected on camera. Whatever that is. */
      perp_wall_distance =
	side == 0
	? (map_position.x - player_position_adjusted.x
	   + (1 - step_direction.x) / 2) / ray_direction.x
	: (map_position.y - player_position_adjusted.y
	   + (1 - step_direction.y) / 2) / ray_direction.y;

      /* Calculate the height of the line to blit on the renderer. */
      int line_height = (int) (config.projection_height / perp_wall_distance);
      
      /* Calculate lowest and highest pixel of the vertical line. */
      int draw_start = -line_height / 2 + config.projection_height / 2;
      if (draw_start < 0)
	draw_start = 0;
      int draw_end = line_height / 2 + config.projection_height / 2;
      if ((unsigned int) draw_end >= config.projection_height)
	draw_end = config.projection_height;

      bitmap_t *wall_bitmap = map.bitmaps[(int) map.walling
					  [XY (map_position.x,
					       map_position.y,
					       map.size)] - 1];
      
      float wall_x;
      if (side == 0)
      	wall_x = player_position_adjusted.y + perp_wall_distance
      	  * ray_direction.y;
      else
      	wall_x = player_position_adjusted.x + perp_wall_distance
      	  * ray_direction.x;
      wall_x -= floorf (wall_x);

      vec2i_t wall_bitmap_coordinates =
	(vec2i_t)
	{
	 .x = (int) (wall_x * (float) wall_bitmap->width), .y = 0
	};

      if (side == 0 && ray_direction.x > 0)
	wall_bitmap_coordinates.x = wall_bitmap->width
	  - wall_bitmap_coordinates.x - 1;
      if (side == 1 && ray_direction.y < 0)
	wall_bitmap_coordinates.x = wall_bitmap->width
	  - wall_bitmap_coordinates.x - 1;

      for (int y = draw_start; y < draw_end; ++y)
	{
	  /* The fuck is this magic? */
	  wall_bitmap_coordinates.y = (((y * 256 - config.projection_height
					 * 128 + line_height * 128)
					* wall_bitmap->height)
				       / line_height) / 256;

	  Uint16 wall_pixel =
	    wall_bitmap->pixels[XY (wall_bitmap_coordinates.x,
				    wall_bitmap_coordinates.y,
				    wall_bitmap->width)];
	  /* And thats it. Draw the pixel. */
	  if (side == 1)
	    shade_color (&wall_pixel);

	  pixel_buffer[XY (x, y, config.projection_width)] = wall_pixel;
	}

      /* Floor and ceiling. */
      vec2f_t floor_wall_position = { 0 };

      /* 4 different floor positions possible. */
      if (side == 0 && ray_direction.x > 0)
	{
	  floor_wall_position.x = map_position.x;
	  floor_wall_position.y = map_position.y + wall_x;
	}
      else if (side == 0 && ray_direction.x < 0)
	{
	  floor_wall_position.x = map_position.x + 1;
	  floor_wall_position.y = map_position.y + wall_x;
	}
      else if (side == 1 && ray_direction.y > 0)
	{
	  floor_wall_position.x = map_position.x + wall_x;
	  floor_wall_position.y = map_position.y;
	}
      else
	{
	  floor_wall_position.x = map_position.x + wall_x;
	  floor_wall_position.y = map_position.y + 1;
	}

      if (draw_end < 0)
	draw_end = config.projection_height;

      /* Draw the floor and ceiling. */
      for (int y = draw_end; (unsigned int) y < config.projection_height; y++)
	{
	  float current_distance = config.projection_height
	    / (2.f * y - config.projection_height);
	  float weight = current_distance / perp_wall_distance;
	  vec2f_t current_position =
	    (vec2f_t)
	    {
	     .x = weight * floor_wall_position.x + (1 - weight)
	     * player_position_adjusted.x,
	     .y = weight * floor_wall_position.y + (1 - weight)
	     * player_position_adjusted.y
	    };

	  /* Draw the floor. */
	  bitmap_t *floor_bitmap =
	    map.bitmaps[(int) map.flooring[XY ((int) current_position.x,
					       (int) current_position.y,
					       map.size)] - 1];

	  vec2i_t floor_bitmap_coordinates =
	    (vec2i_t)
	    {
	     .x = (int) (current_position.x
			 * floor_bitmap->width) % floor_bitmap->width,
	     .y = (int) (current_position.y
			 * floor_bitmap->height) % floor_bitmap->height
	    };
	  
	  pixel_buffer[XY (x, y, config.projection_width)] =
	    floor_bitmap->pixels[XY (floor_bitmap_coordinates.x,
				     floor_bitmap_coordinates.y,
				     floor_bitmap->width)];

	  /* Draw the ceiling. */
	  bitmap_t *ceiling_bitmap =
	    map.bitmaps[(int) map.ceiling[(int) XY ((int) current_position.x,
						    (int) current_position.y,
						    map.size)] - 1];

	  vec2i_t ceiling_bitmap_coordinates =
	    (vec2i_t)
	    {
	     .x = (int) (current_position.x
			 * ceiling_bitmap->width) % ceiling_bitmap->width,
	     .y = (int) (current_position.y
			 * ceiling_bitmap->height) % ceiling_bitmap->height
	    };

	  pixel_buffer[XY (x, config.projection_height - y - 1,
			   config.projection_width)] =
	    ceiling_bitmap->pixels[XY (ceiling_bitmap_coordinates.x,
				       ceiling_bitmap_coordinates.y,
				       ceiling_bitmap->width)];
	}
    }
  /* Copy the pixel buffer to the renderer. */
  SDL_UpdateTexture (projection, NULL, pixel_buffer,
		     config.projection_width * sizeof (Uint16));
  SDL_RenderCopy (renderer, projection, NULL, NULL);
}
