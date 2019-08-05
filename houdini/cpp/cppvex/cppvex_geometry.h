#pragma once

#include <type_traits>

#include <GEO/GEO_PrimPoly.h>

#include "cppvex_attrib.h"

namespace cppvex {

//          _    _           _     _
//  __ _ __| |__| |_ __  ___(_)_ _| |_
// / _` / _` / _` | '_ \/ _ \ | ' \  _|
// \__,_\__,_\__,_| .__/\___/_|_||_\__|
//                |_|

GA_Index addpoint(GA_Detail *geo, const vector3 pos) {
  GA_Offset offset = geo->appendPoint();
  geo->setPos3(offset, pos);
  return geo->pointIndex(offset);
}

GA_Index addpoint(const vector3 pos) {
  return addpoint(output_geometry(), pos);
}

//          _    _          _
//  __ _ __| |__| |_ __ _ _(_)_ __
// / _` / _` / _` | '_ \ '_| | '  \
// \__,_\__,_\__,_| .__/_| |_|_|_|_|
//                |_|

// The following template should be constrained with such that all T are the
// same, but I'm missing full c++17 to do int nicely.

// template <class T, class... Ts>
// constexpr bool are_same = std::is_same_v<T, Ts>::value && ... ;

// This function should be renamed to `addprim` once I figure out how to fix the
// template
template <class... T> //, class = std::enable_if_t<are_same<GA_Offset, T...>>>
GA_Index addprimtogeo(GEO_Detail *geo, T... point_indices) {
  GEO_PrimPoly *prim =
      dynamic_cast<GEO_PrimPoly *>(geo->appendPrimitive(GEO_PRIMPOLY));

  (prim->appendVertex(geo->pointOffset(point_indices)), ...);

  prim->close();

  return prim->getMapIndex();
}

template <class... T> //, class = std::enable_if_t<are_same<GA_Offset, T...>>>
GA_Index addprim(T... point_indices) {
  return addprimtogeo(output_geometry(), point_indices...);
}

//          _    _             _
//  __ _ __| |__| |_ _____ _ _| |_ _____ __
// / _` / _` / _` \ V / -_) '_|  _/ -_) \ /
// \__,_\__,_\__,_|\_/\___|_|  \__\___/_\_\

GA_Index addvertex(GA_Detail *geo, GA_Index prim_index, GA_Index point_index) {
  GA_Offset prim_offset = geo->primitiveOffset(prim_index);
  if (GEO_PRIMPOLY != geo->getPrimitiveTypeId(prim_offset))
    return -1;

  GEO_PrimPoly *prim =
      dynamic_cast<GEO_PrimPoly *>(geo->getPrimitive(prim_offset));

  GA_Size size = prim->appendVertex(geo->pointOffset(point_index));

  return prim->getVertexIndex(size);
}

GA_Index addvertex(GA_Index prim_index, GA_Index point_index) {
  return addvertex(output_geometry(), prim_index, point_index);
}

//           _      _             _     _
//  _ __ _ _(_)_ __| |_ _ __  ___(_)_ _| |_
// | '_ \ '_| | '  \  _| '_ \/ _ \ | ' \  _|
// | .__/_| |_|_|_|_\__| .__/\___/_|_||_\__|
// |_|                 |_|

GA_Index primpoint(const GA_Detail *geo, const GA_Index prim_index,
                   const GA_Size vertex_local_index) {
  return geo->getPrimitiveByIndex(prim_index)
      ->getPointIndex(vertex_local_index);
}

GA_Index primpoint(const GA_Index prim_index,
                   const GA_Size  vertex_local_index) {
  return primpoint(output_geometry(), prim_index, vertex_local_index);
}

GA_Index primpoint(const int input_index, const GA_Index prim_index,
                   const GA_Size vertex_local_index) {
  return primpoint(input_geometry(input_index), prim_index, vertex_local_index);
}

//                                      _     _
//  _ _ ___ _ __  _____ _____ _ __  ___(_)_ _| |_
// | '_/ -_) '  \/ _ \ V / -_) '_ \/ _ \ | ' \  _|
// |_| \___|_|_|_\___/\_/\___| .__/\___/_|_||_\__|
//                           |_|

bool removepoint(GA_Detail *geo, const GA_Index point_index) {
  return geo->destroyPointIndex(point_index, GA_Detail::GA_DESTROY_DEGENERATE_INCOMPATIBLE);
}

bool removepoint(const GA_Index point_index) {
  return removepoint(output_geometry(), point_index);
}

} // namespace cppvex
