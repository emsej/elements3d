/* map.c --- map loading. */

#include "map.h"

/* Load map from path using GNU Guile. */

void
map_load_guile (map_t *map, player_t *player, const char *path)
{
  scm_init_guile ();
  scm_c_primitive_load (path);

  /* Load the map dimensions. */
  
  SCM scm_size = scm_variable_ref (scm_c_lookup ("size"));
  if (scm_is_number (scm_size))
    map->size = scm_to_int (scm_size);
  else
    FATAL ("size undefined or not a number.");
  
  /* Load the tilemaps. */
  
  SCM scm_walling = scm_variable_ref (scm_c_lookup ("walling"));
  if (scm_is_bytevector (scm_walling))
    {
      size_t len = scm_c_bytevector_length (scm_walling);

      map->walling = malloc (len); /* TODO: check for NULL. */

      for (size_t i = 0; i < len; ++i)
	map->walling[i] = scm_c_bytevector_ref (scm_walling, i);
    }
  else
    FATAL ("walling undefined or not a byte vector.");

  SCM scm_flooring = scm_variable_ref (scm_c_lookup ("flooring"));
  if (scm_is_bytevector (scm_flooring))
    {
      size_t len = scm_c_bytevector_length (scm_flooring);

      map->flooring = malloc (len); /* TODO: check for NULL. */

      for (size_t i = 0; i < len; ++i)
	map->flooring[i] = scm_c_bytevector_ref (scm_flooring, i);
    }
  else
    FATAL ("flooring undefined or not a byte vector.");

  SCM scm_ceiling = scm_variable_ref (scm_c_lookup ("ceiling"));
  if (scm_is_bytevector (scm_ceiling))
    {
      size_t len = scm_c_bytevector_length (scm_ceiling);

      map->ceiling = malloc (len); /* TODO: check for NULL. */

      for (size_t i = 0; i < len; ++i)
	map->ceiling[i] = scm_c_bytevector_ref (scm_ceiling, i);
    }
  else
    FATAL ("ceiling undefined or not a byte vector.");

  /* Load the bitmaps. TODO: implement tilesets. */

  SCM scm_bitmaps = scm_variable_ref (scm_c_lookup ("bitmaps"));
  if (scm_is_simple_vector (scm_bitmaps))
    {
      size_t len = SCM_SIMPLE_VECTOR_LENGTH (scm_bitmaps);

      map->number_of_bitmaps = len;
      map->bitmaps = malloc (map->number_of_bitmaps * sizeof (bitmap_t));

      for (size_t i = 0; i < len; ++i)
	map->bitmaps[i] = bitmap_load (scm_to_latin1_stringn (SCM_SIMPLE_VECTOR_REF (scm_bitmaps, i), NULL));
    }
  else
    FATAL ("bitmaps undefined or not a vector.");

  SCM scm_player_x = scm_variable_ref (scm_c_lookup ("player-x"));
  if (scm_is_number (scm_player_x))
    player->x = scm_to_int (scm_player_x);
  else
    FATAL ("player-x undefined or not a number.");
  
  SCM scm_player_y = scm_variable_ref (scm_c_lookup ("player-y"));
  if (scm_is_number (scm_player_y))
    player->y = scm_to_int (scm_player_y);
  else
    FATAL ("player-y undefined or not a number.");

  SCM scm_player_direction = scm_variable_ref (scm_c_lookup ("player-direction"));
  if (scm_is_number (scm_player_direction))
    (void) 0;
  else
    FATAL ("player-angle undefined or not a number.");
}

/* Free all resources allocated in map. */

void
map_free (map_t *map)
{
  free (map->walling);
}
