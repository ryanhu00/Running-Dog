#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <assert.h>

#include "asset.h"
#include "asset_cache.h"
#include "list.h"
#include "sdl_wrapper.h"

static list_t *ASSET_CACHE;

const size_t FONT_SIZE = 18;
const size_t INITIAL_CAPACITY = 5;

typedef struct {
  asset_type_t type;
  const char *filepath;
  void *obj;
} entry_t;

static void asset_cache_free_entry(entry_t *entry) {
  assert(entry != NULL);
  switch (entry->type) {
  case ASSET_IMAGE: {
    SDL_DestroyTexture(entry->obj);
    break;
  }
  case ASSET_FONT: {
    TTF_CloseFont(entry->obj);
    break;
  }
  case ASSET_BUTTON: {
    asset_t *button = (asset_t *) entry->obj;
    asset_destroy((asset_t *) button);
    break;
  }
  case ASSET_BACKGROUND: {
    asset_t *background = (asset_t *) entry->obj;
    asset_destroy((asset_t *) background);
    break;
  }
  default: {
    fprintf(stderr, "Asset is not a compatible type.");
    break;
  }
  }
  free(entry);
}

void asset_cache_init() {
  ASSET_CACHE = list_init(INITIAL_CAPACITY, (free_func_t) asset_cache_free_entry);
}

void asset_cache_destroy() { list_free(ASSET_CACHE); }

void *check_existence(const char *filepath) {
  for (size_t i = 0; i < list_size(ASSET_CACHE); i++) {
    entry_t *entry = list_get(ASSET_CACHE, i);
    if (entry->filepath && strcmp(entry->filepath, filepath) == 0) {
      return entry->obj;
    }
  }
  return NULL;
}

void *asset_cache_obj_get_or_create(asset_type_t ty, const char *filepath) {
  void *exists = check_existence(filepath);
  if (exists) {
    return exists;
  }

  entry_t *entry = malloc(sizeof(entry_t));
  assert(entry);
  entry->filepath = filepath;
  switch (ty) {
  case ASSET_FONT: {
    entry->type = ASSET_FONT;
    entry->obj = ttf_load_text(filepath, FONT_SIZE);
    break;
  }
  case ASSET_IMAGE: {
    entry->type = ASSET_IMAGE;
    entry->obj = sdl_load_image(filepath);
    break;
  }
  default: {
    fprintf(stderr, "Asset is not a compatible type.");
    break;
  }
  }
  list_add(ASSET_CACHE, entry);
  return entry->obj;
}

void asset_cache_register_button(asset_t *button) {
  assert(asset_get_type(button) == ASSET_BUTTON);
  entry_t *entry = malloc(sizeof(entry_t));
  assert(entry);
  entry->type = ASSET_BUTTON;
  entry->obj = button;
  entry->filepath = NULL;
  list_add(ASSET_CACHE, entry);
}

void asset_cache_handle_buttons(state_t *state, double x, double y) {
  for (size_t i = 0; i < list_size(ASSET_CACHE); i++) {
    entry_t *entry = list_get(ASSET_CACHE, i);
    asset_type_t type = entry->type;
    if (type == ASSET_BUTTON) {
      asset_t *button = (asset_t *) entry->obj;
      asset_on_button_click(button, state, x, y);
    }
  }
}
