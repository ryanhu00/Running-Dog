#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "body.h"
#include "color.h"
#include "list.h"
#include "math.h"
#include "polygon.h"
#include "vector.h"

const double INIT_MASS = 0.0;

struct body {
  polygon_t *poly;

  double mass;

  vector_t force;
  vector_t impulse;
  bool removed;
  bool collided;
  void *info;
  vector_t centroid;
  free_func_t info_freer;
};

body_t *body_init(list_t *shape, double mass, rgb_color_t color) {
  body_t *body = body_init_with_info(shape, mass, color, NULL, NULL);
  assert(body);
  return body;
}

body_t *body_init_with_info(list_t *shape, double mass, rgb_color_t color,
                            void *info, free_func_t info_freer) {
  body_t *body = malloc(sizeof(body_t));
  assert(body);
  body->poly =
      polygon_init(shape, VEC_ZERO, INIT_MASS, color.r, color.g, color.b);
  assert(body->poly);
  body->mass = mass;
  body->force = VEC_ZERO;
  body->impulse = VEC_ZERO;
  body->removed = false;
  body->collided = false;
  body->info = info;
  body->info_freer = info_freer;
  body->centroid = polygon_centroid(body->poly);
  return body;
}

void body_free(body_t *body) {
  polygon_free(body->poly);
  if (body->info_freer) {
    body->info_freer(body->info);
  }
  free(body);
}

list_t *body_get_shape(body_t *body) {
  list_t *original_shape = polygon_get_points(body->poly);
  size_t num_points = list_size(original_shape);
  list_t *copied_shape = list_init(num_points, (free_func_t) free);
  for (size_t i = 0; i < num_points; i++) {
    vector_t *original_point = list_get(original_shape, i);
    vector_t *copied_point = malloc(sizeof(vector_t));
    assert(copied_point);
    *copied_point = (vector_t){original_point->x, original_point->y};
    list_add(copied_shape, copied_point);
  }
  return copied_shape;
}

vector_t body_get_centroid(body_t *body) { return body->centroid; }

vector_t body_get_velocity(body_t *body) {
  return *polygon_get_velocity(body->poly);
}

rgb_color_t *body_get_color(body_t *body) {
  return (polygon_get_color(body->poly));
}

void body_set_color(body_t *body, rgb_color_t *col) {
  polygon_set_color(body->poly, col);
}

void body_set_centroid(body_t *body, vector_t x) {
  polygon_set_center(body->poly, x);
  body->centroid = x;
}

void body_set_velocity(body_t *body, const vector_t v) {
  polygon_set_velocity(body->poly, v);
}

double body_get_rotation(body_t *body) {
  return polygon_get_rotation(body->poly);
}

void body_set_rotation(body_t *body, double angle) {
  polygon_set_rotation(body->poly, angle);
}

void body_tick(body_t *body, double dt) {
  vector_t old_velocity = body_get_velocity(body);
  vector_t impulse_velocity = vec_multiply(1.0 / body->mass, body->impulse);
  vector_t new_velocity = vec_add(old_velocity, impulse_velocity);
  new_velocity =
      vec_add(new_velocity, vec_multiply(dt / body->mass, body->force));
  vector_t avg_velocity =
      vec_multiply(0.5, vec_add(old_velocity, new_velocity));
  vector_t trans = vec_multiply(dt, avg_velocity);
  polygon_translate(body->poly, trans);
  body_set_centroid(body, vec_add(body_get_centroid(body), trans));
  body_set_velocity(body, new_velocity);
  body->impulse = VEC_ZERO;
  body->force = VEC_ZERO;
}

double body_get_mass(body_t *body) { return body->mass; }

polygon_t *body_get_polygon(body_t *body) { return body->poly; }

void *body_get_info(body_t *body) { return body->info; }

void body_add_force(body_t *body, vector_t force) {
  body->force = vec_add(body->force, force);
}

void body_add_impulse(body_t *body, vector_t impulse) {
  body->impulse = vec_add(body->impulse, impulse);
}

void body_remove(body_t *body) { body->removed = true; }

void body_collided(body_t *body) {
  body->collided = true;
}

bool body_is_collided(body_t *body) {
  return body->collided;
}

bool body_is_removed(body_t *body) { return body->removed; }

void body_reset(body_t *body) {
  body->force = VEC_ZERO;
  body->impulse = VEC_ZERO;
}
