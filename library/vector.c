#include "vector.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

const vector_t VEC_ZERO = {0.0, 0.0};

vector_t vec_add(vector_t v1, vector_t v2) {
  vector_t v3;
  v3.x = v1.x + v2.x;
  v3.y = v1.y + v2.y;
  return v3;
}

vector_t vec_subtract(vector_t v1, vector_t v2) {
  v2 = vec_negate(v2);
  vector_t v3 = vec_add(v1, v2);
  return v3;
}

vector_t vec_negate(vector_t v) { return vec_multiply(-1.0, v); }

vector_t vec_multiply(double scalar, vector_t v) {
  vector_t v1;
  v1.x = v.x * scalar;
  v1.y = v.y * scalar;
  return v1;
}

double vec_dot(vector_t v1, vector_t v2) {
  double final;
  final = (v1.x * v2.x) + (v1.y * v2.y);
  return final;
}

double vec_cross(vector_t v1, vector_t v2) {
  double final;
  final = (v1.x * v2.y) - (v1.y * v2.x);
  return final;
}

vector_t vec_rotate(vector_t v, double angle) {
  vector_t final;
  double cosv = cos(angle);
  double sinv = sin(angle);
  final.x = (v.x * cosv) - (v.y * sinv);
  final.y = (v.x * sinv) + (v.y * cosv);
  return final;
}

double vec_get_length(vector_t v) { return sqrt(pow(v.x, 2) + pow(v.y, 2)); }