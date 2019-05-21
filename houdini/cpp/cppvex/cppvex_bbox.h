#pragma once

#include <utility>

#include "cppvex_types.h"

namespace cppvex {

std::pair<vector3, vector3> getbbox(const GU_Detail *geo) {
  UT_BoundingBox bbox;
  geo->getCachedBounds(bbox);
  return {bbox.minvec(), bbox.maxvec()};
}

void getbbox(const GU_Detail *geo, vector3 &min, vector3 &max) {
  auto bbox = getbbox(geo);
  min       = bbox.first;
  max       = bbox.second;
}

vector3 relbbox(const GU_Detail *geo, const vector3 position) {
  auto [min, max] = getbbox(geo);
  return (position - min) / (max - min);
}

} // namespace vex
