#pragma once

#include <type_traits>

#include <GEO/GEO_PrimPoly.h>

#include "cppvex_attrib.h"

namespace cppvex {

GA_Offset addpoint(GA_Detail *geo, const vector3 pos) {
  GA_Offset offset = geo->appendPoint();
  geo->setPos3(offset, pos);
  return offset;
}

GA_Offset addpoint(const vector3 pos) {
  return addpoint(internal::output_geometry, pos);
}

// The following template should be constrained with such that all T are the
// same, but I'm missing full c++17 to do int nicely.

// template <class T, class... Ts>
// constexpr bool are_same = std::is_same_v<T, Ts>::value && ... ;

// This function should be renamed to `addprim` once I figure out how to fix the
// template
template <class... T> //, class = std::enable_if_t<are_same<GA_Offset, T...>>>
GA_Offset addprimtogeo(GEO_Detail *geo, T... point_offsets) {
  GEO_PrimPoly *prim =
      dynamic_cast<GEO_PrimPoly *>(geo->appendPrimitive(GEO_PRIMPOLY));

  (prim->appendVertex(point_offsets), ...);

  prim->close();

  return prim->getMapOffset();
}

template <class... T> //, class = std::enable_if_t<are_same<GA_Offset, T...>>>
GA_Offset addprim(T... point_offsets) {
  return addprimtogeo(internal::output_geometry, point_offsets...);
}

GA_Offset addvertex(GA_Detail *geo, GA_Offset prim_offset,
                    GA_Offset point_offset) {
  if (GEO_PRIMPOLY != geo->getPrimitiveTypeId(prim_offset))
    return -1;

  GEO_PrimPoly *prim =
      dynamic_cast<GEO_PrimPoly *>(geo->getPrimitive(prim_offset));

  GA_Size size = prim->appendVertex(point_offset);

  return prim->getVertexOffset(size);
}

GA_Offset addvertex(GA_Offset prim_offset, GA_Offset point_offset) {
  return addvertex(internal::output_geometry, prim_offset, point_offset);
}

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

int nvertices() { return internal::output_geometry->getNumVertices(); }

GA_Offset primpoint(const GA_Detail *geo, const GA_Offset prim_offset,
                    const GA_Size vertex_local_index) {
  return geo->getPrimitive(prim_offset)->getPointOffset(vertex_local_index);
}

GA_Offset primpoint(const int input_index, const GA_Offset prim_offset,
                    const GA_Size vertex_local_index) {
  return primpoint(internal::input_geometry[input_index], prim_offset,
                   vertex_local_index);
}

GA_Offset primpoint(const GA_Offset prim_offset,
                    const GA_Size   vertex_local_index) {
  return primpoint(internal::output_geometry, prim_offset, vertex_local_index);
}

} // namespace cppvex
