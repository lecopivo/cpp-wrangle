#pragma once

#include "cppvex_attrib.h"

namespace cppvex {

int npoints(const int input_index) {
  return internal::input_geometry[input_index]->getNumPoints();
}

int npoints() { return internal::output_geometry->getNumPoints(); }

int nprimitives(const int input_index) {
  return internal::input_geometry[input_index]->getNumPrimitives();
}

int nprimitives() { return internal::output_geometry->getNumPrimitives(); }

int nvertices(const int input_index) {
  return internal::input_geometry[input_index]->getNumVertices();
}

int nvertices() { return internal::output_geometry->getNumVertices();

}

} // namespace cppvex
