#pragma once

#include <utility>

#include "cppvex_context.h"
#include "cppvex_types.h"

namespace cppvex {

//   ___     _   ___ ___
//  / __|___| |_| _ ) _ ) _____ __
// | (_ / -_)  _| _ \ _ \/ _ \ \ /
//  \___\___|\__|___/___/\___/_\_\

std::pair<vector3, vector3> getbbox(const GU_Detail *geo) {
  UT_BoundingBox bbox;
  geo->getCachedBounds(bbox);
  return {bbox.minvec(), bbox.maxvec()};
}

std::pair<vector3, vector3> getbbox() { return getbbox(output_geometry()); }

std::pair<vector3, vector3> getbbox(const int input_index) {
  return getbbox(input_geometry(input_index));
}

//   ___     _   ___ ___
//  / __|___| |_| _ ) _ ) _____ __
// | (_ / -_)  _| _ \ _ \/ _ \ \ /
//  \___\___|\__|___/___/\___/_\_\

void getbbox(const GU_Detail *geo, vector3 &min, vector3 &max) {
  auto bbox = getbbox(geo);
  min       = bbox.first;
  max       = bbox.second;
}

void getbbox(vector3 &min, vector3 &max) {
  return getbbox(output_geometry(), min, max);
}

void getbbox(const int input_index, vector3 &min, vector3 &max) {
  return getbbox(input_geometry(input_index), min, max);
}

//  ___     _ ___ ___
// | _ \___| | _ ) _ ) _____ __
// |   / -_) | _ \ _ \/ _ \ \ /
// |_|_\___|_|___/___/\___/_\_\

vector3 relbbox(const GU_Detail *geo, const vector3 position) {
  auto [min, max] = getbbox(geo);
  return (position - min) / (max - min);
}

vector3 relbbox(const vector3 position) {
  return relbbox(output_geometry(), position);
}

vector3 relbbox(const int input_index, const vector3 position) {
  return relbbox(input_geometry(input_index), position);
}

} // namespace cppvex
