#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "asset.h"
#include "asset_cache.h"
#include "collision.h"
#include "forces.h"
#include "sdl_wrapper.h"
#include "vector.h"

const vector_t MIN = {0, 0};
const vector_t MAX = {1000, 500};
const vector_t DOG_POS = {250, 250};
const vector_t DOG_DUCKING_POS = {250, 150};
const vector_t OBS_START_GROUND = {900, 180};
const vector_t OBS_START_AIR = {900, 226};
const size_t GROUND_OBS_HEIGHT = 80;
const size_t GROUND_OBS_WIDTH = 70;
const size_t AIR_OBS_HEIGHT = 40;
const size_t AIR_OBS_WIDTH = 80;
const double DOG_HEIGHT = 130;
const double DOG_WIDTH = 100;
const double DUCKING_HEIGHT = 75;
const size_t POWER_UP_HEIGHT = 35;
const size_t POWER_UP_WIDTH = 80;
const size_t NUM_BUTTONS = 6;
const size_t LEADERBOARD_POSITIONS = 3;
const rgb_color_t TEXT_COLOR = (rgb_color_t) {0, 0, 0};

const char *BUSH_PATH = "assets/bush.png";
const char *COLLAR_PATH = "assets/collar.png";
const char *DOGBONE_PATH = "assets/dog_bone.png";
const char *DOGHOUSE_PATH = "assets/dog_house.png";
const char *DUCKINGDOG_PATH = "assets/ducking_dog.png";
const char *FRISBEE_PATH = "assets/frisbee.png";
const char *JUMPINGDOG_PATH = "assets/jumping_dog.png";
const char *WALKING1_PATH = "assets/walking41.png";
const char *WALKING2_PATH = "assets/walking42.png";
const char *DOGBOWL_PATH = "assets/dog_bowl.png";
const char *LIFE1_PATH = "assets/life1.png";
const char *LIFE2_PATH = "assets/life2.png";
const char *LIFE3_PATH = "assets/life3.png";
const char *BONE_PATH = "assets/dog_bone.png";
const char *FONT_PATH = "assets/Comic-Sans-MS-Bold.ttf";
const char *FOREGROUND_PATH = "assets/game_background.png";
const char *BACKGROUND_PATH = "assets/parallax_background.png";
const char *HELP_BOARD = "assets/help_board.png";
const char *PLAY_BUTTON = "assets/play_button.png";
const char *PAUSE_BUTTON = "assets/pause_button.png";
const char *REPLAY_BUTTON = "assets/replay_button.png";
const char *RESUME_BUTTON = "assets/resume_button.png";
const char *X_BUTTON = "assets/X-button.png";
const char *HELP_BUTTON = "assets/help_button.png";

const size_t CHOOSE_RANGE = 10;

const size_t FOREGROUND_SPEED = 4;
const size_t BACKGROUND_SPEED = 2;
const size_t FOREGROUND_HEIGHT = 100;
const size_t FOREGROUND_START_HEIGHT = 400;
const size_t BACKGROUND_START_HEIGHT = 0;
const size_t BACKGROUND_HEIGHT = 500;
const size_t BACKGROUND_LENGTH = 1500;
const ssize_t BACKGROUND_THRESHOLD = -1500;
const vector_t STARTING_VELOCITY = {-250, 0};
const double ACCEL = -50;
const size_t NUM_CAPTIONS = 2;
const double FLASH_DIFFERENCE = 0.1;
const double FLASH_TIME_TOTAL = 0.5;
const size_t SCORE_GROWTH = 100;
const size_t SCORE_LOSS = 200; 
const double WALK_CHANGE = 0.3;
const double DUCKING_TIME = 1;
const vector_t INITIAL_JUMP = {0, 600};
const size_t INITIAL_GRAV = 100;
const size_t INVINCIBLE_SEC = 4;

const int8_t TITLE_TEXT_SIZE = 48;
const int8_t NORM_TEXT_SIZE = 20;

const double INITIAL_JUMP_VELOCITY = 500;
const double DOG_GRAVITY = 40;

const char *SCORE_TEXT = "Score: %zu";
const char *SCORE1_TEXT = "#1: %zu";
const char *SCORE2_TEXT = "#2: %zu";
const char *SCORE3_TEXT = "#3: %zu";
const char *INIVINCIBILITY_TEXT = "Invincibility: %zu";
const char *START_CAPTIONS[] = {"Welcome to Running Dog", "Press play to start."};
const SDL_Rect START_BOX[] = {(SDL_Rect){
      .x = 250, .y = 150, .w = 500, .h = 50}, (SDL_Rect){
      .x = 400, .y = 250, .w = 200, .h = 25}};

const char *END_CAPTIONS[] = {"Game Over", "Leaderboard"};
const SDL_Rect END_BOXES[] = {(SDL_Rect){
      .x = 400, .y = 100, .w = 200, .h = 50}, (SDL_Rect){
      .x = 250, .y = 200, .w = 200, .h = 25}};
const SDL_Rect SCORES[] = {(SDL_Rect){
      .x = 250, .y = 250, .w = 100, .h = 25}, (SDL_Rect){
      .x = 250, .y = 300, .w = 100, .h = 25}, (SDL_Rect){
      .x = 250, .y = 350, .w = 100, .h = 25}};

const char *PAUSED_CAPTIONS[] = {"Paused.", "Press play to resume."};
const SDL_Rect PAUSED_BOXES[] = {(SDL_Rect){
      .x = 400, .y = 175, .w = 200, .h = 50}, (SDL_Rect){
      .x = 400, .y = 250, .w = 200, .h = 25}};

const SDL_Rect SCORE_BOX = (SDL_Rect) {
      .x = 10, .y = 10, .w = 100, .h = 20};
      const SDL_Rect INVINCIBLE = (SDL_Rect) {
      .x = 10, .y = 40, .w = 100, .h = 20};

const SDL_Rect LIFE1_BOX = (SDL_Rect) {
      .x = 130, .y = 10, .w = 20, .h = 20};
const SDL_Rect LIFE2_BOX = (SDL_Rect) {
      .x = 150, .y = 10, .w = 20, .h = 20};
const SDL_Rect LIFE3_BOX = (SDL_Rect) {
      .x = 170, .y = 10, .w = 20, .h = 20};


const SDL_Rect BACKGROUND_BOX1 = (SDL_Rect) {
      .x = 0, .y = 0, .w = 1500, .h = 500};
const SDL_Rect BACKGROUND_BOX2 = (SDL_Rect) {
      .x = 1500, .y = 0, .w = 1500, .h = 500};
const SDL_Rect FOREGROUND_BOX1 = (SDL_Rect) {
      .x = 0, .y = 400, .w = 1500, .h = 100 };
const SDL_Rect FOREGROUND_BOX2 = (SDL_Rect) {
      .x = 1500, .y = 400, .w = 1500, .h = 100};

const SDL_Rect HELP_BOARD_LOC = (SDL_Rect) {.x = 250, .y = 75, .w = 600, .h = 500};

const rgb_color_t OBS_COLOR = (rgb_color_t) {0.2, 0.2, 0.3};
const rgb_color_t DOG_COLOR = (rgb_color_t) {0.1, 0.9, 0.2};

struct state {
  list_t *obstacles;
  asset_t *dog;
  asset_t *dog1;
  asset_t *dog2;
  asset_t *jump_dog;
  asset_t *duck_dog;
  body_t *dog_body;
  body_t *dog_body1;
  body_t *dog_body2;
  body_t *dog_ducking;
  body_t *dog_jumping;
  scene_t *scene;
  size_t score;
  TTF_Font *font;
  size_t lives;
  size_t curr_state_num;
  asset_t *start_button;
  asset_t *pause_button;
  asset_t *resume_button;
  asset_t *replay_button;
  asset_t *help_button;
  asset_t *x_button;
  list_t *foregrounds;
  list_t *backgrounds;
  bool paused;
  list_t *hearts;
  bool is_jumping;
  bool is_ducking;
  size_t score1;
  size_t score2;
  size_t score3;
  size_t invincibility_countdown;
  bool is_collided;
};

typedef struct button_info {
  const char *image_path;
  const char *font_path;
  SDL_Rect image_box;
  rgb_color_t text_color;
  button_handler_t handler;
} button_info_t;

void start_game(state_t *state);
void pause_game(state_t *state);
void resume_game(state_t *state);
void restart_game(state_t *state);
void help_menu(state_t *state);
void leave_help_menu(state_t *state);

button_info_t button_templates[] = {
    {.image_path = "assets/play_button.png",
     .image_box = (SDL_Rect) {450, 300, 100, 100},
     .handler = (void *) start_game},
    {.image_path = "assets/resume_button.png",
     .image_box = (SDL_Rect) {450, 300, 100, 100},
     .handler = (void *) resume_game},
    {.image_path = "assets/pause_button.png",
     .image_box = (SDL_Rect) {0, 60, 25, 25},
     .handler = (void *) pause_game},
     {.image_path = "assets/replay_button.png",
     .image_box = (SDL_Rect) {450, 250, 100, 100},
     .handler = (void *) restart_game},
     {.image_path = "assets/help_button.png",
     .image_box = (SDL_Rect) {900, 40, 50, 50},
     .handler = (void *) help_menu},
     {.image_path = "assets/X-button.png", 
     .image_box = (SDL_Rect) {525, 400, 50, 50},
     .handler = (void *) leave_help_menu}
     };

typedef enum { DOG, DOGHOUSE, BUSH, FRISBEE, COLLAR, BONE, DOGBOWL} body_type_t;

body_type_t *make_type_info(body_type_t type) {
  body_type_t *info = malloc(sizeof(body_type_t));
  assert(info);
  *info = type;
  return info;
}

body_type_t get_type(body_t *body) {
  return *(body_type_t *) body_get_info(body);
}

body_t *make_body(size_t w, size_t h, vector_t center, body_type_t type) {
  list_t *c = list_init(4, free);
  vector_t *v1 = malloc(sizeof(vector_t));
  assert(v1);
  *v1 = (vector_t) {0, 0};
  list_add(c, v1);

  vector_t *v2 = malloc(sizeof(vector_t));
  assert(v2);
  *v2 = (vector_t) {w, 0};
  list_add(c, v2);

  vector_t *v3 = malloc(sizeof(vector_t));
  assert(v3);
  *v3 = (vector_t) {w, h};
  list_add(c, v3);

  vector_t *v4 = malloc(sizeof(vector_t));
  assert(v4);
  *v4 = (vector_t) {0, h};
  list_add(c, v4);
  body_t *body = body_init_with_info(c, 1, OBS_COLOR, make_type_info(type), free);
  body_set_centroid(body, center);
  return body;
}

void add_asset_body(state_t *state, const char *obs_path, body_t *obstacle) {
  if (obs_path) {
    scene_add_body(state->scene, obstacle);
    asset_t *obs_asset = asset_make_image_with_body(obs_path, obstacle);
    list_add(state->obstacles, obs_asset);
  }
}

void add_obj_asset(state_t *state, body_type_t type) {
  const char *obs_path;
  switch (type) {
    case DOGHOUSE:
      obs_path = DOGHOUSE_PATH;
      body_t *doghouse = make_body(GROUND_OBS_WIDTH, 
                          GROUND_OBS_HEIGHT, OBS_START_GROUND, type);
      add_asset_body(state, obs_path, doghouse);
      break;
    case BUSH:
      obs_path = BUSH_PATH;
      body_t *bush = make_body(GROUND_OBS_WIDTH, GROUND_OBS_HEIGHT, 
                                OBS_START_GROUND, type);
      add_asset_body(state, obs_path, bush);
      break;
    case FRISBEE: 
      obs_path = FRISBEE_PATH;
      body_t *frisbee = make_body(AIR_OBS_WIDTH, AIR_OBS_HEIGHT, OBS_START_AIR, type);
      add_asset_body(state, obs_path, frisbee);
      break;
    case COLLAR: 
      obs_path = COLLAR_PATH;
      body_t *collar = make_body(GROUND_OBS_WIDTH, GROUND_OBS_HEIGHT, 
                                OBS_START_GROUND, type);
      add_asset_body(state, obs_path, collar);
      break;
    case BONE: 
      obs_path = BONE_PATH;
      body_t *bone = make_body(POWER_UP_WIDTH, POWER_UP_HEIGHT, OBS_START_GROUND, type);
      add_asset_body(state, obs_path, bone);
      break;
    case DOGBOWL: 
      obs_path = DOGBOWL_PATH;
      body_t *dogbowl = make_body(GROUND_OBS_WIDTH, GROUND_OBS_HEIGHT, 
      OBS_START_GROUND, type);
      add_asset_body(state, obs_path, dogbowl);
      break;
    default:
      obs_path = NULL;
      break;
  }
}

void clear_game(state_t *state) {
  size_t num_bodies = scene_bodies(state->scene);
  for (size_t i = 0; i < num_bodies; i++) {
    body_t *body = scene_get_body(state->scene, i);
    if (get_type(body) != DOG) {
      body_remove(body);
    }
  }
}

void start_game(state_t *state) {
  state->curr_state_num = 3;
  state->paused = false;
}

void pause_game(state_t *state) {
  state->paused = true;
}

void resume_game(state_t *state) {
  state->paused = false;
}

void restart_game(state_t *state) {
  state->curr_state_num = 3;
  state->paused = false;
}

void help_menu(state_t *state) {
  state->curr_state_num = 4;
}

void leave_help_menu(state_t *state) {
  state->curr_state_num = 0;
}

void on_key(char key, key_event_type_t type, double held_time, void *state) {
  struct state *curr = (struct state *) state;
  if (type == KEY_PRESSED) {
    switch (key) {
    case LEFT_ARROW:
      break;
    case RIGHT_ARROW:
      break;
    case UP_ARROW:
      if (!curr->is_ducking) {
        curr->is_jumping = true;
      }
      break;
    case DOWN_ARROW:
      if (!curr->is_jumping) {
        curr->is_ducking = true;
      }
      break;
    default:
      break;
    }
  }
}

void reset_jump(state_t *state) {
  state->is_jumping = false;
  state->dog_body = state->dog_body1;
  state->dog = state->dog1;
  body_set_centroid(state->dog_jumping, DOG_POS);
  body_set_velocity(state->dog_jumping, (vector_t) {0, INITIAL_JUMP_VELOCITY});
}

void update_jump_velocity(state_t *state, double a, double time) {
  double velocity = body_get_velocity(state->dog_jumping).y - a * time;
  body_set_velocity(state->dog_jumping, (vector_t) {0, velocity});
}

asset_t *create_button_from_info(state_t *state, button_info_t info) {
  SDL_Rect img_box = info.image_box;
  asset_t *image_asset = NULL;
  asset_t *text_asset = NULL;
  if (info.image_path != NULL) {
    image_asset = asset_make_image(info.image_path, img_box);
  }
  asset_t *button_asset =
      asset_make_button(img_box, image_asset, text_asset, info.handler);
  asset_cache_register_button(button_asset);
  return button_asset;
}

void create_buttons(state_t *state) {
  for (size_t i = 0; i < NUM_BUTTONS; i++) {
    button_info_t info = button_templates[i];
    asset_t *button = create_button_from_info(state, info);
    if (strcmp(info.image_path, PLAY_BUTTON) == 0){
      state->start_button = button;
    } else if (strcmp(info.image_path, RESUME_BUTTON) == 0) {
      state->resume_button = button;
    } else if (strcmp(info.image_path, REPLAY_BUTTON) == 0) {
      state->replay_button = button;
    } else if (strcmp(info.image_path, PAUSE_BUTTON) == 0) {
      state->pause_button = button;
    } else if (strcmp(info.image_path, HELP_BUTTON) == 0) {
      state->help_button = button;
    } else {
      state->x_button = button;
    }
  }
}

void create_text(state_t *state, const char *captions[], const SDL_Rect *boxes,
                 size_t num_captions) {
  for (size_t i = 0; i < num_captions; i++) {
    asset_text_render(state->font, captions[i], boxes[i], TEXT_COLOR);
  }
}

void create_lives(state_t *state) {
  asset_t *life1 = asset_make_image(LIFE1_PATH, LIFE1_BOX);
  list_add(state->hearts, life1);
  asset_t *life2 = asset_make_image(LIFE2_PATH, LIFE2_BOX);
  list_add(state->hearts, life2);
  asset_t *life3 = asset_make_image(LIFE3_PATH, LIFE3_BOX);
  list_add(state->hearts, life3);
}

void tick_backgrounds(list_t *background_list) {
  size_t bg_list_size = list_size(background_list);
  for (size_t i = 0; i < bg_list_size; i++) {
    asset_t *background = list_get(background_list, i);
    asset_background_tick(background);
  }
}

void remove_backgrounds(list_t *background_list, const char *filepath, 
                        size_t scrolling_speed, size_t background_height, 
                        size_t start_height) {
  size_t bg_list_size = list_size(background_list);
  for (size_t i = 0; i < bg_list_size; i++) {
    asset_t *background = list_get(background_list, i);
    ssize_t x_pos = asset_background_xpos(background);
    if (x_pos < BACKGROUND_THRESHOLD) {
      list_remove(background_list, i);
      ssize_t next_x_pos = asset_background_xpos(list_get(background_list, i));
      asset_t *image = asset_make_image(filepath, (SDL_Rect) {next_x_pos +
                                        BACKGROUND_LENGTH, start_height,
                                        BACKGROUND_LENGTH, background_height});
      asset_t *new_background = asset_make_background((SDL_Rect) {next_x_pos + 
                                        BACKGROUND_LENGTH, start_height,
                                        BACKGROUND_LENGTH, background_height}, image,
                                        scrolling_speed);
      list_add(background_list, new_background);
    }
  }
}

void change_leaderboard(state_t *state) {
  if (state->score > state->score1) {
    state->score3 = state->score2;
    state->score2 = state->score1;
    state->score1 = state->score;
  } else if (state->score > state->score2) {
    state->score3 = state->score2;
    state->score2 = state->score;
  } else if (state->score > state->score3) {
    state->score3 = state->score;
  }
}

void pause_obstacles(state_t *state) {
  size_t num_bodies = scene_bodies(state->scene);
  for (size_t i = 0; i < num_bodies; i++) {
    body_t *body = scene_get_body(state->scene, i);
    body_type_t type = get_type(body);
    if (type != DOG){
      if (state->paused) {
        body_set_velocity(body, VEC_ZERO);
      } else {
        body_set_velocity(body, STARTING_VELOCITY);
      }
    }
  }
}

state_t *emscripten_init() {
  asset_cache_init();
  sdl_init(MIN, MAX);

  state_t *state = malloc(sizeof(state_t));
  assert(state);
  state->score = 0;
  srand(time(NULL));
  state->scene = scene_init();
  state->lives = 3;
  state->curr_state_num = 0;
  state->is_ducking = false;
  state->is_jumping = false;
  state->is_collided = false;
  state->score1 = 0;
  state->score2 = 0;
  state->score3 = 0;
  state->invincibility_countdown = 0; 

  state->dog_body1 = make_body(DOG_WIDTH, DOG_HEIGHT, (vector_t) DOG_POS, DOG);
  state->dog_body2 = make_body(DOG_WIDTH, DOG_HEIGHT, (vector_t) DOG_POS, DOG);
  state->dog_jumping = make_body(DOG_WIDTH, DOG_HEIGHT, (vector_t) DOG_POS, DOG);
  body_set_velocity(state->dog_jumping, (vector_t) {0, INITIAL_JUMP_VELOCITY});
  state->dog_ducking = make_body(DOG_WIDTH, DUCKING_HEIGHT, 
                            (vector_t) DOG_DUCKING_POS, DOG);
  state->dog_body = state->dog_body1;
  scene_add_body(state->scene, state->dog_body);
  
  state->dog1 = asset_make_image_with_body(WALKING1_PATH, state->dog_body1);
  state->dog2 = asset_make_image_with_body(WALKING2_PATH, state->dog_body2);
  state->jump_dog = asset_make_image_with_body(JUMPINGDOG_PATH, state->dog_jumping);
  state->duck_dog = asset_make_image_with_body(DUCKINGDOG_PATH, state->dog_ducking);
  state->dog = asset_make_image_with_body(WALKING1_PATH, state->dog_body);

  state->obstacles = list_init(2, (free_func_t)asset_destroy);
  list_add(state->obstacles, state->dog);

  state->paused = false;
  state->hearts = list_init(3, (free_func_t)asset_destroy);
  create_lives(state);
  create_buttons(state);
  sdl_on_key((key_handler_t)on_key);
  state->font = ttf_load_text(FONT_PATH, TITLE_TEXT_SIZE);


  state->backgrounds = list_init(2, (free_func_t) asset_destroy);
  state->foregrounds = list_init(2, (free_func_t) asset_destroy);
  //initialize first two background images
  asset_t *first_background_image = asset_make_image(BACKGROUND_PATH, BACKGROUND_BOX1);
  asset_t *second_background_image = asset_make_image(BACKGROUND_PATH, BACKGROUND_BOX2);
  asset_t *first_background = asset_make_background(BACKGROUND_BOX1,
                                                      first_background_image,
                                                      BACKGROUND_SPEED);
  asset_t *second_background = asset_make_background(BACKGROUND_BOX2, 
                                                      second_background_image,
                                                      BACKGROUND_SPEED);
  list_add(state->backgrounds, first_background);
  list_add(state->backgrounds, second_background);
  //initialize first two foreground images
  asset_t *first_foreground_image = asset_make_image(FOREGROUND_PATH, FOREGROUND_BOX1);
  asset_t *second_foreground_image = asset_make_image(FOREGROUND_PATH, FOREGROUND_BOX2);
  asset_t *first_foreground = asset_make_background(FOREGROUND_BOX1, 
                                                      first_foreground_image,
                                                      FOREGROUND_SPEED);
  asset_t *second_foreground = asset_make_background(FOREGROUND_BOX2, 
                                                      second_foreground_image,
                                                      FOREGROUND_SPEED);
  list_add(state->foregrounds, first_foreground);
  list_add(state->foregrounds, second_foreground);
                                                      
  return state;
}

bool in_window(body_t *body) {
  list_t *points = body_get_shape(body);
  size_t len = list_size(points);
  for (size_t i = 0; i < len; i++) {
    vector_t *point = list_get(points, i);
    if (point->x < 0) {
      return false;
    }
  }
  list_free(points);
  return true;
}

void check_obstacle(state_t *state) {
  size_t num_bodies = scene_bodies(state->scene);
  for (size_t i = 0; i < num_bodies; i++) {
    body_t *obstacle = scene_get_body(state->scene, i);
    if (!in_window(obstacle)) {
      body_remove(obstacle);
    } else if (get_type(obstacle) != DOG 
              && find_collision(state->dog_body, obstacle).collided) {
      if (!body_is_collided(obstacle)) {
        body_type_t type = get_type(obstacle);
        if ((type == BUSH || type == DOGHOUSE || type == FRISBEE) &&
            state->invincibility_countdown == 0) {
          state->lives--;
          state->is_collided = true;
        } else if (type == COLLAR) {
          state->score -= SCORE_LOSS;
        } else if (type == BONE) {
          if (state->lives < 3) {
            state->lives += 1; 
          }
        } else if (type == DOGBOWL) {
          state->invincibility_countdown = INVINCIBLE_SEC; 
        }
        body_collided(obstacle);
      }
    }
  }
}

//obstacles are more likely to be spawned than power ups
body_type_t choose_type() {
  size_t num = rand() % CHOOSE_RANGE;
  if (num >= 0 && num < 2) {
    return DOGHOUSE;
  } else if (num < 4) {
    return BUSH;
  } else if (num < 6) {
    return FRISBEE;
  } else if (num < 8) {
    return COLLAR;
  } else if (num == 9) {
    return BONE;
  } else {
    return DOGBOWL;
  }
}

bool emscripten_main(state_t *state) {
  sdl_clear();
  double dt = time_since_last_tick();
  static double last_counter = 0;
  static double ducking_counter = 0;
  static double jumping_counter = 0;
  static double game_animation_counter = 0;
  static double walking_counter = 0;
  static double flashing_counter1 = 0;
  static double flashing_counter2 = 0;

  //render backgrounds
  size_t bg_list_size = list_size(state->backgrounds);
  for (size_t i = 0; i < bg_list_size; i++) {
    asset_t *background = list_get(state->backgrounds, i);
    asset_render(background);
  }
  size_t fg_list_size = list_size(state->foregrounds);
  for (size_t i = 0; i < fg_list_size; i++) {
    asset_t *foreground = list_get(state->foregrounds, i);
    asset_render(foreground);
  }

  if (state->curr_state_num == 2) { //game over
    state->is_collided = false;
    change_leaderboard(state);
    asset_render(state->replay_button);
    clear_game(state);
    state->lives = 3;
    state->score = 0;
    state->invincibility_countdown = 0;
    state->dog_body = state->dog_body1;
    state->dog = state->dog1;
    
    create_text(state, END_CAPTIONS, END_BOXES, NUM_CAPTIONS);

    char score[30];
    sprintf(score, SCORE1_TEXT, state->score1);
    asset_text_render(state->font, score, SCORES[0], TEXT_COLOR);
    sprintf(score, SCORE2_TEXT, state->score2);
    asset_text_render(state->font, score, SCORES[1], TEXT_COLOR);
    sprintf(score, SCORE3_TEXT, state->score3);
    asset_text_render(state->font, score, SCORES[2], TEXT_COLOR);
  } else if (state->curr_state_num == 0) { //start screen
    asset_render(state->start_button);
    asset_render(state->help_button);

    create_text(state, START_CAPTIONS, START_BOX, NUM_CAPTIONS);
    
  } else if (state->curr_state_num == 3) { //game animation
    if (game_animation_counter == 0){
      body_set_velocity(state->dog_jumping, INITIAL_JUMP);
      state->dog_body = state->dog_jumping;
      state->dog = state->jump_dog;
    }
    game_animation_counter += dt;
    if (body_get_centroid(state->dog_body).y < DOG_POS.y) {
      reset_jump(state);
      game_animation_counter = 0.0;
      state->curr_state_num = 1;
    } else {
      update_jump_velocity(state, INITIAL_GRAV, game_animation_counter);
    }
    body_tick(state->dog_jumping, dt);
    asset_render(state->dog);
  } else if (state->curr_state_num == 4) { //help board
    asset_t *board = asset_make_image(HELP_BOARD, HELP_BOARD_LOC);
    asset_render(board);
    asset_render(state->x_button);
  } else if (state->curr_state_num == 1) { //normal game
    pause_obstacles(state);
    char caption[30];
    sprintf(caption, SCORE_TEXT, state->score);
    asset_text_render(state->font, caption, SCORE_BOX, TEXT_COLOR);
    if (!state->paused) { //when game is not paused
      last_counter += dt;
      if (state->is_jumping && !state->is_ducking) { //jumping
        if (jumping_counter == 0) {
          state->dog_body = state->dog_jumping;
          state->dog = state->jump_dog;
        }
        jumping_counter += dt;
        if (body_get_centroid(state->dog_jumping).y < DOG_POS.y) {
          reset_jump(state);
          jumping_counter = 0.0; 
        } else {
          update_jump_velocity(state, DOG_GRAVITY, jumping_counter);
        }
        body_tick(state->dog_jumping, dt);
      } else if (state->is_ducking && !state->is_jumping){ //ducking
        if (ducking_counter == 0) {
          state->dog_body = state->dog_ducking;
          state->dog = state->duck_dog;
        }
        ducking_counter += dt;
        if (ducking_counter >= DUCKING_TIME) {
          state->is_ducking = false;
          state->dog_body = state->dog_body1;
          ducking_counter = 0.0;
        }
      } else { //walking normally
        walking_counter += dt;
        if (walking_counter >= WALK_CHANGE) {
          if (state->dog_body == state->dog_body1) {
            state->dog_body = state->dog_body2;
            state->dog = state->dog2;
          } else {
            state->dog_body = state->dog_body1;
            state->dog = state->dog1;
          }
          walking_counter = 0;
        }
      }
      asset_render(state->pause_button);

      if (state->invincibility_countdown != 0) {
        sprintf(caption, INIVINCIBILITY_TEXT, state->invincibility_countdown);
        asset_text_render(state->font, caption, INVINCIBLE, TEXT_COLOR);
      }
      if (last_counter >= 1.0) {
        state->score += SCORE_GROWTH;
        if (state->invincibility_countdown > 0) {
          state->invincibility_countdown -= 1; 
        }
        if ((state->score / SCORE_GROWTH) % 2 == 0) {
          add_obj_asset(state, choose_type());
        }
        last_counter = 0.0;
      }
      check_obstacle(state);
      if (state->is_collided) { //flash dog when hit obstacle
        flashing_counter1 += dt;
        flashing_counter2 += dt;
        if (flashing_counter1 >= FLASH_TIME_TOTAL) {
          state->curr_state_num = 1;
          flashing_counter1 = 0;
          flashing_counter2 = 0;
          state->is_collided = false;
        }
        if (flashing_counter2 >= FLASH_DIFFERENCE) { 
          asset_render(state->dog);
          flashing_counter2 = 0;
      }
    }
      //asset renders all bodies
      size_t obstacles_len = list_size(state->obstacles);
      for (size_t i = 1; i < obstacles_len; i++) {
        asset_render(list_get(state->obstacles, i));
      }
      if (!state->is_collided) { //render dog normally
        asset_render(state->dog);
      }
    } else { //paused
      asset_render(state->resume_button);
      create_text(state, PAUSED_CAPTIONS, PAUSED_BOXES, NUM_CAPTIONS);
    }
    //render hearts
    for (size_t i = 0; i < state->lives; i++) {
      asset_render(list_get(state->hearts, i));
    }
    //game over bc no more lives
    if (state->lives == 0) {
      state->curr_state_num = 2;
    }
  }
    
  sdl_show();
  
  scene_tick(state->scene, dt, state->obstacles);
  tick_backgrounds(state->foregrounds);
  tick_backgrounds(state->backgrounds);
  remove_backgrounds(state->foregrounds, FOREGROUND_PATH, FOREGROUND_SPEED, 
                    FOREGROUND_HEIGHT, FOREGROUND_START_HEIGHT);
  remove_backgrounds(state->backgrounds, BACKGROUND_PATH, BACKGROUND_SPEED,     
                    BACKGROUND_HEIGHT, BACKGROUND_START_HEIGHT);
  return false;
}

void emscripten_free(state_t *state) {
  TTF_CloseFont(state->font);
  TTF_Quit();
  list_free(state->hearts);
  asset_cache_destroy();
  free(state);
}
