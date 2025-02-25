#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "forces.h"
#include "scene.h"

const size_t INITIAL_SIZE = 20;

struct scene {
  size_t num_bodies;
  list_t *bodies;
  list_t *force_creators;
};

typedef struct force_creator_info {
  force_creator_t force_creator;
  void *aux;
  list_t *bodies;
} force_creator_info_t;

force_creator_info_t *init_force_creator_info(force_creator_t forcer, void *aux,
                                              list_t *bodies) {
  force_creator_info_t *info = malloc(sizeof(force_creator_info_t));
  assert(info);
  info->force_creator = forcer;
  info->aux = aux;
  info->bodies = bodies;
  return info;
}

void free_force_creator_info(void *info) {
  force_creator_info_t *force_info = (force_creator_info_t *)info;
  body_aux_free(force_info->aux);
  list_free(force_info->bodies);
  free(force_info);
}

scene_t *scene_init(void) {
  scene_t *scene = malloc(sizeof(scene_t));
  assert(scene);
  scene->num_bodies = 0;
  scene->bodies = list_init(INITIAL_SIZE, (free_func_t)body_free);
  scene->force_creators =
      list_init(INITIAL_SIZE, (free_func_t)free_force_creator_info);

  return scene;
}

void scene_free(scene_t *scene) {
  list_free(scene->bodies);
  list_free(scene->force_creators);
  free(scene);
}

size_t scene_bodies(scene_t *scene) { return list_size(scene->bodies); }

body_t *scene_get_body(scene_t *scene, size_t index) {
  assert(index >= 0 && index < list_size(scene->bodies));
  return list_get(scene->bodies, index);
}

void scene_add_body(scene_t *scene, body_t *body) {
  list_add(scene->bodies, body);
  scene->num_bodies++;
}

void scene_remove_body(scene_t *scene, size_t index) {
  assert(index >= 0 && index < list_size(scene->bodies));
  body_remove(scene_get_body(scene, index));
}

void scene_tick(scene_t *scene, double dt, list_t *assets) {
  for (size_t i = 0; i < scene->num_bodies; i++) {
    body_t *body = scene_get_body(scene, i);
    if (body_is_removed(body)) {
      list_remove(scene->bodies, i);
      list_remove(assets, i);
      i--;
      scene->num_bodies--;
      body_free(body);
    } else {
      body_tick(body, dt);
     }
  }
}

void scene_add_force_creator(scene_t *scene, force_creator_t force_creator,
                             void *aux) {
  scene_add_bodies_force_creator(scene, force_creator, aux, list_init(0, free));
}

void scene_add_bodies_force_creator(scene_t *scene, force_creator_t forcer,
                                    void *aux, list_t *bodies) {
  force_creator_info_t *info = init_force_creator_info(forcer, aux, bodies);
  list_add(scene->force_creators, info);
}
