#include "polygon.h"
#include "math.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

const double GRAVITY = -9.8;
const double INITIAL_ROT_ANGLE = 0;

struct polygon {
  list_t *points;
  vector_t velocity;
  double rotation_speed;
  rgb_color_t *colors;
  double rotation_angle;
};

polygon_t *polygon_init(list_t *points, vector_t initial_velocity,
                        double rotation_speed, double red, double green,
                        double blue) {
  polygon_t *new = malloc(sizeof(polygon_t));
  assert(new != NULL);
  new->points = points;
  new->velocity = initial_velocity;
  new->rotation_speed = rotation_speed;
  new->colors = color_init(red, green, blue);
  new->rotation_angle = INITIAL_ROT_ANGLE;
  return new;
}

list_t *polygon_get_points(polygon_t *polygon) { return polygon->points; }

void polygon_move(polygon_t *polygon, double time_elapsed) {
  vector_t gravity = (vector_t){.x = 0, .y = GRAVITY};
  vector_t velocity_change = vec_multiply(time_elapsed, gravity);
  vector_t final_velocity =
      vec_add(polygon->velocity, velocity_change);
  vector_t dist_moved = vec_multiply(time_elapsed, final_velocity);
  polygon->rotation_angle = polygon->rotation_speed * time_elapsed;
  vector_t polygon_center = polygon_centroid(polygon);
  polygon_rotate(polygon, polygon->rotation_angle, polygon_center);
  polygon_translate(polygon, dist_moved);
  polygon_set_velocity(polygon, final_velocity);
}

void polygon_set_velocity(polygon_t *polygon, vector_t v) {
  polygon->velocity = v;
}

void polygon_free(polygon_t *polygon) {
  list_t *points = polygon->points;
  list_free(points);
  color_free(polygon->colors);
  free(polygon);
}

vector_t *polygon_get_velocity(polygon_t *polygon) {
  return &polygon->velocity;
}

double polygon_area(polygon_t *polygon) {
  size_t size = list_size(polygon->points);
  double area = 0;
  for (size_t i = 0; i < size; i++) {
    vector_t *curr = list_get(polygon->points, i);
    vector_t *next = list_get(polygon->points, ((i + 1) % size));
    area += (curr->x * next->y) - (curr->y * next->x);
  }
  return area * 0.5;
}

vector_t polygon_centroid(polygon_t *polygon) {
  size_t size = list_size(polygon->points);
  double cx = 0, cy = 0;
  double area = polygon_area(polygon);
  vector_t centroid;
  for (size_t i = 0; i < size; i++) {
    vector_t *curr = list_get(polygon->points, i);
    vector_t *next = list_get(polygon->points, (i + 1) % size);
    cx += (curr->x + next->x) * ((curr->x * next->y) - (next->x * curr->y));
    cy += (curr->y + next->y) * ((curr->x * next->y) - (next->x * curr->y));
  }
  cx = cx / (6.0 * area);
  cy = cy / (6.0 * area);
  centroid.x = cx;
  centroid.y = cy;
  return centroid;
}

void polygon_translate(polygon_t *polygon, vector_t translation) {
  size_t size = list_size(polygon->points);
  for (size_t i = 0; i < size; i++) {
    vector_t *curr = list_get(polygon->points, i);
    *curr = vec_add(*curr, translation);
  }
}

void polygon_rotate(polygon_t *polygon, double angle, vector_t point) {
  polygon_translate(polygon, vec_negate(point));

  size_t size = list_size(polygon->points);
  for (size_t i = 0; i < size; i++) {
    vector_t *vertex = list_get(polygon->points, i);
    *vertex = vec_rotate(*vertex, angle);
  }
  polygon_translate(polygon, point);
}

rgb_color_t *polygon_get_color(polygon_t *polygon) { return polygon->colors; }

void polygon_set_color(polygon_t *polygon, rgb_color_t *color) {
  rgb_color_t *old_color = polygon->colors;
  polygon->colors = color;
  color_free(old_color);
}

void polygon_set_center(polygon_t *polygon, vector_t centroid) {
  vector_t curr_centroid = polygon_get_center(polygon);
  vector_t translation = vec_subtract(centroid, curr_centroid);
  polygon_translate(polygon, translation);
}

vector_t polygon_get_center(polygon_t *polygon) {
  return polygon_centroid(polygon);
}

void polygon_set_rotation(polygon_t *polygon, double rot) {
  vector_t polygon_center = polygon_get_center(polygon);
  polygon_rotate(polygon, rot - polygon->rotation_angle,
                 polygon_center);
}

double polygon_get_rotation(polygon_t *polygon) {
  return polygon->rotation_angle;
}
