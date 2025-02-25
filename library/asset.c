#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <assert.h>

#include "asset.h"

#include "asset_cache.h"
#include "color.h"
#include "sdl_wrapper.h"

typedef struct asset {
  asset_type_t type;
  SDL_Rect bounding_box;
} asset_t;

typedef struct text_asset {
  asset_t base;
  TTF_Font *font;
  const char *text;
  rgb_color_t color;
} text_asset_t;

typedef struct image_asset {
  asset_t base;
  SDL_Texture *texture;
  body_t *body;
} image_asset_t;

typedef struct button_asset {
  asset_t base;
  image_asset_t *image_asset;
  text_asset_t *text_asset;
  button_handler_t handler;
  bool is_rendered;
} button_asset_t;

typedef struct background_asset {
  asset_t base;
  image_asset_t *image_asset;
  ssize_t scrolling_speed;
} background_asset_t;

/**
 * Allocates memory for an asset with the given parameters.
 *
 * @param ty the type of the asset
 * @param bounding_box the bounding box containing the location and dimensions
 * of the asset when it is rendered
 * @return a pointer to the newly allocated asset
 */
static asset_t *asset_init(asset_type_t ty, SDL_Rect bounding_box) {
  asset_t *new;
  switch (ty) {
  case ASSET_IMAGE: {
    new = malloc(sizeof(image_asset_t));
    break;
  }
  case ASSET_FONT: {
    new = malloc(sizeof(text_asset_t));
    break;
  }
  case ASSET_BUTTON: {
    new = malloc(sizeof(button_asset_t));
    break;
  }
  case ASSET_BACKGROUND: {
    new = malloc(sizeof(background_asset_t));
    break;
  }
  default: {
    assert(false && "Unknown asset type");
  }
  }
  assert(new);
  new->type = ty;
  new->bounding_box = bounding_box;
  return new;
}

asset_type_t asset_get_type(asset_t *asset) { return asset->type; }

asset_t *asset_make_image(const char *filepath, SDL_Rect bounding_box) {
  image_asset_t *image = malloc(sizeof(image_asset_t));
  assert(image);
  image->base = *asset_init(ASSET_IMAGE, bounding_box);
  image->texture =
      (SDL_Texture *) asset_cache_obj_get_or_create(ASSET_IMAGE, filepath);
  image->body = NULL;
  return (asset_t *) image;
}

asset_t *asset_make_background(SDL_Rect bounding_box, asset_t *image_asset, 
                            size_t speed) {
  background_asset_t *background = malloc(sizeof(background_asset_t));
  assert(background);
  background->base = *asset_init(ASSET_BACKGROUND, bounding_box);
  background->image_asset = (image_asset_t *) image_asset;
  background->scrolling_speed = -speed;
  return (asset_t *) background;
}


asset_t *asset_make_image_with_body(const char *filepath, body_t *body) {
  image_asset_t *image = malloc(sizeof(image_asset_t));
  assert(image);
  image->base = *asset_init(ASSET_IMAGE, bounding_box(body));
  image->texture =
      (SDL_Texture *) asset_cache_obj_get_or_create(ASSET_IMAGE, filepath);
  image->body = body;
  return (asset_t *) image;
}

asset_t *asset_make_text(const char *filepath, SDL_Rect bounding_box,
                         const char *text, rgb_color_t color) {
  text_asset_t *words = malloc(sizeof(text_asset_t));
  assert(words);
  words->base = *asset_init(ASSET_FONT, bounding_box);
  words->font = asset_cache_obj_get_or_create(ASSET_FONT, filepath);
  words->text = text;
  words->color = color;
  return (asset_t *) words;
}

asset_t *asset_make_button(SDL_Rect bounding_box, asset_t *image_asset,
                           asset_t *text_asset, button_handler_t handler) {
  button_asset_t *button = malloc(sizeof(button_asset_t));
  assert(button);
  button->base = *asset_init(ASSET_BUTTON, bounding_box);
  button->image_asset = (image_asset_t *) image_asset;
  button->handler = handler;
  button->text_asset = (text_asset_t *) text_asset;
  button->is_rendered = false;
  return (asset_t *) button;
}

void asset_on_button_click(asset_t *button, state_t *state, double x, double y) {
  button_asset_t *button_asset = (button_asset_t *) button;
  if (!button_asset->is_rendered) {
    return;
  }
  SDL_Rect bounding_box = button_asset->base.bounding_box;
  if (x >= bounding_box.x && x <= bounding_box.x + bounding_box.w &&
      y >= bounding_box.y && y <= bounding_box.y + bounding_box.h) {
    button_asset->handler(state);
  }
  button_asset->is_rendered = false;
}

ssize_t asset_background_tick(asset_t *background){
  background_asset_t *background_asset = (background_asset_t *) background;
  background_asset->base.bounding_box.x += background_asset->scrolling_speed;
  return (ssize_t) background_asset->base.bounding_box.x;
}

ssize_t asset_background_xpos(asset_t *background){
  background_asset_t *background_asset = (background_asset_t *) background;
  return (ssize_t) background_asset->base.bounding_box.x;
}

void asset_render(asset_t *asset) {
  switch (asset->type) {
  case ASSET_IMAGE: {
    image_asset_t *image_asset = (image_asset_t *)asset;
    if (image_asset->body) {
      sdl_image_render(image_asset->texture, bounding_box(image_asset->body));
    } else {
      sdl_image_render(image_asset->texture, asset->bounding_box);
    }
    break;
  }
  case ASSET_FONT: {
    text_asset_t *text_asset = (text_asset_t *) asset;
    asset_text_render(text_asset->font, text_asset->text, asset->bounding_box,
                    text_asset->color);
    break;
  }
  case ASSET_BUTTON: {
    button_asset_t *button = (button_asset_t *) asset;
    if (button->image_asset) {
      image_asset_t *image_asset = (image_asset_t *) button->image_asset;
      sdl_image_render(image_asset->texture, button->base.bounding_box);
    }
    if (button->text_asset) {
      text_asset_t *text_asset = (text_asset_t *) button->text_asset;
      if (text_asset) {
        asset_text_render(text_asset->font, text_asset->text,
                        text_asset->base.bounding_box, text_asset->color);
      }
    }
    button->is_rendered = true;
    break;
  }
  case ASSET_BACKGROUND: {
    background_asset_t *background = (background_asset_t *) asset;
    if (background->image_asset) {
      image_asset_t *image_asset = (image_asset_t *) background->image_asset;
      sdl_image_render(image_asset->texture, background->base.bounding_box);
    }
    break;
  }
  default: {
    fprintf(stderr, "Asset is not a compatible type.");
    break;
  }
  }
}

void asset_destroy(asset_t *asset) { free(asset); }
