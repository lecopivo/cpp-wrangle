#pragma once

#include "cppvex_types.h"

namespace cppvex {

vector2 normalize(vector2 v) {
  v.normalize();
  return v;
}

vector3 normalize(vector3 v) {
  v.normalize();
  return v;
}

vector4 normalize(vector4 v) {
  v.normalize();
  return v;
}

matrix2 transpose(matrix2 A) {
  A.transpose();
  return A;
}

matrix3 transpose(matrix3 A) {
  A.transpose();
  return A;
}

matrix4 transpose(matrix4 A) {
  A.transpose();
  return A;
}

} // namespace cppvex
