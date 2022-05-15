#include <assert.h>
#include <stdlib.h>
#include <time.h>

#include "include/list.h"

struct point {
  int x, y;
};

// points related functions
int generate_random(int min, int max) {
  int range = max - min + 1;
  double rand_val = rand() / (1.0 + RAND_MAX);
  return (int)(rand_val * range + min);
}

void destroy_points(struct point **points, size_t arr_size) {
  if (!points) return;
  for (size_t i = 0; i < arr_size; i++) {
    if (points[i]) free(points[i]);
  }
  free(points);
}

struct point **create_points(size_t arr_size, int min, int max) {
  struct point **points = calloc(arr_size, sizeof *points);
  if (!points) return NULL;

  for (size_t i = 0; i < arr_size; i++) {
    points[i] = calloc(1, sizeof *points[i]);

    if (!points[i]) {
      destroy_points(points, i);
      return NULL;
    }

    points[i]->x = generate_random(min, max);
    points[i]->y = generate_random(min, max);
  }
  return points;
}

struct point **copy_points(struct point **points, size_t arr_size) {
  if (!points) return NULL;

  struct point **copy = calloc(arr_size, sizeof *copy);
  if (!copy) return NULL;

  for (size_t i = 0; i < arr_size; i++) {
    copy[i] = calloc(1, sizeof *copy[i]);

    if (!copy[i]) {
      destroy_points(copy, i);
      return NULL;
    }

    copy[i]->x = points[i]->x;
    copy[i]->y = points[i]->y;
  }

  return copy;
}

// linked list related functions
bool equals(const void *a, const void *b) {
  const struct point *p_a = a;
  const struct point *p_b = b;
  return p_a->x == p_b->x && p_a->y == p_b->y;
}

int cmpr(const void *a, const void *b) {
  const struct point *p_a = a;
  const struct point *p_b = b;
  return (p_a->x > p_b->x) - (p_a->x < p_b->x);
}

struct list *before(struct point **points, size_t arr_size) {
  struct list *list = list_init();
  for (size_t i = 0; i < arr_size; i++) {
    list_append(list, points[i]);
  }
  return list;
}

void destroy(void *data) {
  struct point *point = data;
  if (point) free(point);
}

void after(struct list *list) { list_destroy(list, destroy); }

void list_prepend_and_peek_test(struct point **points, size_t arr_size) {
  // given
  struct list *list = list_init();

  assert(list);
  assert(list_empty(list));
  assert(list_size(list) == 0);
  assert(!list->head && !list->tail);

  // when
  for (size_t i = 0; i < arr_size; i++) {
    list_prepend(list, points[i]);
  }

  // then
  assert(list->size == arr_size);
  assert(equals(list_peek_first(list), points[arr_size - 1]));
  assert(equals(list_peek_last(list), points[0]));

  // cleanup
  after(list);
}

void list_append_and_peek_test(struct point **points, size_t arr_size) {
  // given
  struct list *list = list_init();

  assert(list);
  assert(list_empty(list));
  assert(list_size(list) == 0);
  assert(!list->tail && !list->head);

  // when
  for (size_t i = 0; i < arr_size; i++) {
    list_append(list, points[i]);
  }

  // then
  assert(list->size == arr_size);
  assert(equals(list_peek_first(list), points[0]));
  assert(equals(list_peek_last(list), points[arr_size - 1]));

  // cleanup
  after(list);
}

void list_insert_at_test(struct point **points, size_t arr_size) {
  // given
  struct list *list = before(points, arr_size);
  // and
  struct point *point = calloc(1, sizeof *point);
  point->y = point->x = -1;

  // when
  bool res = list_insert_at(list, point, 1);

  // then
  assert(res);
  assert(list_size(list) == arr_size + 1);
  assert(equals(list_at(list, 1), point));

  // cleanup
  free(point);
  after(list);
}

void list_insert_priority_test(struct point **points, size_t arr_size) {
  // given
  struct list *list = before(points, arr_size);

  // when
  for (size_t i = 0; i < arr_size; i++) {
    list_insert_priority(list, points[i], cmpr);
  }

  // sort the original array
  qsort(points, arr_size, sizeof *points, cmpr);
}

void list_at_test(struct point **points, size_t arr_size) {
  // given
  struct list *list = before(points, arr_size);

  // when
  struct point *first = list_at(list, 0);
  struct point *last = list_at(list, arr_size - 1);
  struct point *middle = list_at(list, arr_size / 2);

  // then
  assert(first && last && middle);
  assert(equals(first, points[0]));
  assert(equals(last, points[arr_size - 1]));
  assert(equals(middle, points[arr_size / 2]));

  // cleanup
  after(list);
}

void list_remove_first_test(struct point **points, size_t arr_size) {}

int main(void) {
  srand(time(NULL));
  return 0;
}
