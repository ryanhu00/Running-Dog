#include "sdl_wrapper.h"
#include "asset_cache.h"
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

const char WINDOW_TITLE[] = "CS 3";
const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 500;
const double MS_PER_S = 1e3;
SDL_Color COLOR_BLACK = {0, 0, 0, 255};

/**
 * The coordinate at the center of the screen.
 */
vector_t center;
/**
 * The coordinate difference from the center to the top right corner.
 */
vector_t max_diff;
/**
 * The SDL window where the scene is rendered.
 */
SDL_Window *window;
/**
 * The renderer used to draw the scene.
 */
SDL_Renderer *renderer;
/**
 * The keypress handler, or NULL if none has been configured.
 */
key_handler_t key_handler = NULL;
/**
 * SDL's timestamp when a key was last pressed or released.
 * Used to mesasure how long a key has been held.
 */
uint32_t key_start_timestamp;
/**
 * The value of clock() when time_since_last_tick() was last called.
 * Initially 0.
 */
clock_t last_clock = 0;

/** Computes the center of the window in pixel coordinates */
vector_t get_window_center(void) {
  int *width = malloc(sizeof(*width)), *height = malloc(sizeof(*height));
  assert(width != NULL);
  assert(height != NULL);
  SDL_GetWindowSize(window, width, height);
  vector_t dimensions = {.x = *width, .y = *height};
  free(width);
  free(height);
  return vec_multiply(0.5, dimensions);
}

/**
 * Computes the scaling factor between scene coordinates and pixel coordinates.
 * The scene is scaled by the same factor in the x and y dimensions,
 * chosen to maximize the size of the scene while keeping it in the window.
 */
double get_scene_scale(vector_t window_center) {
  // Scale scene so it fits entirely in the window
  double x_scale = window_center.x / max_diff.x,
         y_scale = window_center.y / max_diff.y;
  return x_scale < y_scale ? x_scale : y_scale;
}

/** Maps a scene coordinate to a window coordinate */
vector_t get_window_position(vector_t scene_pos, vector_t window_center) {
  // Scale scene coordinates by the scaling factor
  // and map the center of the scene to the center of the window
  vector_t scene_center_offset = vec_subtract(scene_pos, center);
  double scale = get_scene_scale(window_center);
  vector_t pixel_center_offset = vec_multiply(scale, scene_center_offset);
  vector_t pixel = {.x = round(window_center.x + pixel_center_offset.x),
                    // Flip y axis since positive y is down on the screen
                    .y = round(window_center.y - pixel_center_offset.y)};
  return pixel;
}

/**
 * Converts an SDL key code to a char.
 * 7-bit ASCII characters are just returned
 * and arrow keys are given special character codes.
 */
char get_keycode(SDL_Keycode key) {
  switch (key) {
  case SDLK_LEFT:
    return LEFT_ARROW;
  case SDLK_UP:
    return UP_ARROW;
  case SDLK_RIGHT:
    return RIGHT_ARROW;
  case SDLK_DOWN:
    return DOWN_ARROW;
  case SDLK_SPACE:
    return SPACE_BAR;
  default:
    // Only process 7-bit ASCII characters
    return key == (SDL_Keycode)(char)key ? key : '\0';
  }
}

void sdl_init(vector_t min, vector_t max) {
  // Check parameters
  assert(min.x < max.x);
  assert(min.y < max.y);

  center = vec_multiply(0.5, vec_add(min, max));
  max_diff = vec_subtract(max, center);
  SDL_Init(SDL_INIT_EVERYTHING);
  window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT,
                            SDL_WINDOW_RESIZABLE);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
  TTF_Init();
}

bool sdl_is_done(void *state) {
  SDL_Event *event = malloc(sizeof(*event));
  assert(event != NULL);
  while (SDL_PollEvent(event)) {
    switch (event->type) {
    case SDL_QUIT:
      free(event);
      return true;
    case SDL_KEYDOWN:
    case SDL_KEYUP:
      if (key_handler == NULL)
        break;
      char key = get_keycode(event->key.keysym.sym);
      if (key == '\0')
        break;

      uint32_t timestamp = event->key.timestamp;
      if (!event->key.repeat) {
        key_start_timestamp = timestamp;
      }
      key_event_type_t type =
          event->type == SDL_KEYDOWN ? KEY_PRESSED : KEY_RELEASED;
      double held_time = (timestamp - key_start_timestamp) / MS_PER_S;
      key_handler(key, type, held_time, state);
      break;
    case SDL_MOUSEBUTTONDOWN:
      asset_cache_handle_buttons(state, event->button.x, event->button.y);
      break;
    }
  }
  free(event);
  return false;
}

void sdl_clear(void) {
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);
}

void sdl_draw_polygon(polygon_t *poly, rgb_color_t color) {
  list_t *points = polygon_get_points(poly);
  // Check parameters
  size_t n = list_size(points);
  assert(n >= 3);

  vector_t window_center = get_window_center();

  // Convert each vertex to a point on screen
  int16_t *x_points = malloc(sizeof(*x_points) * n),
          *y_points = malloc(sizeof(*y_points) * n);
  assert(x_points != NULL);
  assert(y_points != NULL);
  for (size_t i = 0; i < n; i++) {
    vector_t *vertex = list_get(points, i);
    vector_t pixel = get_window_position(*vertex, window_center);
    x_points[i] = pixel.x;
    y_points[i] = pixel.y;
  }

  // Draw polygon with the given color
  filledPolygonRGBA(renderer, x_points, y_points, n, color.r * 255,
                    color.g * 255, color.b * 255, 255);
  free(x_points);
  free(y_points);
}

SDL_Texture *sdl_load_image(const char *IMG_PATH) {
  SDL_Texture *img = IMG_LoadTexture(renderer, IMG_PATH);
  return img;
}

TTF_Font *ttf_load_text(const char *text, size_t font_size) {
  TTF_Font *font = TTF_OpenFont(text, font_size);
  return font;
}

void sdl_image_render(SDL_Texture *image, SDL_Rect bounding_box) {
  SDL_RenderCopy(renderer, image, NULL, &bounding_box);
}

void asset_text_render(TTF_Font *font, const char *text, SDL_Rect bounding_box,
                     rgb_color_t color) {
  SDL_Color sdl_color = {color.r, color.g, color.b};
  SDL_Surface *text_surface = TTF_RenderText_Solid(font, text, sdl_color);
  SDL_Texture *text_texture =
      SDL_CreateTextureFromSurface(renderer, text_surface);
  SDL_RenderCopy(renderer, text_texture, NULL, &bounding_box);
  SDL_FreeSurface(text_surface);
  SDL_DestroyTexture(text_texture);
}

void render_text(TTF_Font *font, int8_t num_captions, const char **captions,
                 const vector_t *locations) {
  SDL_Color color = COLOR_BLACK;

  for (size_t i = 0; i < num_captions; i++) {
    const char *text = captions[i];
    vector_t locs = locations[i];
    SDL_Surface *text_surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture *text_texture =
        SDL_CreateTextureFromSurface(renderer, text_surface);
    SDL_Rect dest_rect = {
        .x = locs.x, .y = locs.y, .w = text_surface->w, .h = text_surface->h};
    SDL_RenderCopy(renderer, text_texture, NULL, &dest_rect);
    SDL_FreeSurface(text_surface);
    SDL_DestroyTexture(text_texture);
  }
}

void sdl_show(void) {
  // Draw boundary lines
  vector_t window_center = get_window_center();
  vector_t max = vec_add(center, max_diff),
           min = vec_subtract(center, max_diff);
  vector_t max_pixel = get_window_position(max, window_center),
           min_pixel = get_window_position(min, window_center);
  SDL_Rect *boundary = malloc(sizeof(*boundary));
  boundary->x = min_pixel.x;
  boundary->y = max_pixel.y;
  boundary->w = max_pixel.x - min_pixel.x;
  boundary->h = min_pixel.y - max_pixel.y;
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderDrawRect(renderer, boundary);
  free(boundary);

  SDL_RenderPresent(renderer);
}

void sdl_render_scene(scene_t *scene, void *aux) {
  sdl_clear();
  size_t body_count = scene_bodies(scene);
  for (size_t i = 0; i < body_count; i++) {
    body_t *body = scene_get_body(scene, i);
    list_t *shape = body_get_shape(body);
    polygon_t *poly = polygon_init(shape, (vector_t){0, 0}, 0, 0, 0, 0);
    sdl_draw_polygon(poly, *body_get_color(body));
    list_free(shape);
  }
  if (aux != NULL) {
    body_t *body = aux;
    sdl_draw_polygon(body_get_polygon(body), *body_get_color(body));
  }
  sdl_show();
}

void sdl_on_key(key_handler_t handler) { key_handler = handler; }

double time_since_last_tick(void) {
  clock_t now = clock();
  double difference = last_clock
                          ? (double)(now - last_clock) / CLOCKS_PER_SEC
                          : 0.0; // return 0 the first time this is called
  last_clock = now;
  return difference;
}

SDL_Rect bounding_box(body_t *body) {
  list_t *points = body_get_shape(body);

  double min_x = __DBL_MAX__;
  double max_x = -__DBL_MAX__;
  double min_y = __DBL_MAX__;
  double max_y = -__DBL_MAX__;
  for (size_t i = 0; i < list_size(points); i++) {
    vector_t *curr_point = list_get(points, i);
    if ((double)curr_point->x < min_x) {
      min_x = curr_point->x;
    }
    if ((double)curr_point->x > max_x) {
      max_x = curr_point->x;
    }
    if ((double)curr_point->y < min_y) {
      min_y = curr_point->y;
    }
    if ((double)curr_point->y > max_y) {
      max_y = curr_point->y;
    }
  }

  vector_t window_center = get_window_center();
  vector_t top_left =
      get_window_position((vector_t){.x = min_x, .y = max_y}, window_center);
  vector_t bottom_right =
      get_window_position((vector_t){.x = max_x, .y = min_y}, window_center);

  SDL_Rect to_return = (SDL_Rect){.x = top_left.x,
                                  .y = bottom_right.y,
                                  .w = bottom_right.x - top_left.x,
                                  .h = -top_left.y + bottom_right.y};
  list_free(points);
  return to_return;
}
