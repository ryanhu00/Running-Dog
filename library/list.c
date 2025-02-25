#include "list.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

const size_t GROWTH_FACTOR = 2;

struct list {
  size_t size;
  size_t capacity;
  void **data;
  free_func_t freer;
};

list_t *list_init(size_t initial_size, free_func_t freer) {
  list_t *new = malloc(sizeof(list_t));
  assert(new != NULL);
  new->size = 0;
  new->capacity = initial_size;
  new->data = malloc(initial_size * sizeof(void *));
  assert(new->data != NULL);
  new->freer = freer;
  return new;
}

void list_free(list_t *list) {
  if (list->freer != NULL) {
    for (size_t i = 0; i < list->size; i++) {
      list->freer(list->data[i]);
    }
  }
  free(list->data);
  free(list);
}

size_t list_size(list_t *list) { return list->size; }

void *list_get(list_t *list, size_t index) {
  assert(index >= 0 && index < list->size);
  return list->data[index];
}

void list_add(list_t *list, void *value) {
  assert(value != NULL);

  if (list->size >= list->capacity) {
    void **temp_list =
        malloc((list->capacity * GROWTH_FACTOR) * sizeof(void *));
    assert(temp_list != NULL);
    for (size_t i = 0; i < list->capacity; i++) {
      temp_list[i] = list->data[i];
    }
    free(list->data);

    list->data = temp_list;
    list->capacity *= GROWTH_FACTOR;
  }
  list->data[list->size] = value;
  list->size++;
}

void *list_remove(list_t *list, size_t index) {
  assert(list->size > 0);
  assert(index < list->size);
  void *removed = (list->data[index]);
  for (size_t i = index + 1; i < list->size; i++) {
    list->data[i - 1] = list->data[i];
  }
  list->size--;
  return (removed);
}
