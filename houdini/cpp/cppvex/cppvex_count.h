#pragma once

#include "cppvex_context.h"

namespace cppvex {

//           _                   _
//  _ _  ___| |___ _ __  ___ _ _| |_ ___
// | ' \/ -_) / -_) '  \/ -_) ' \  _(_-<
// |_||_\___|_\___|_|_|_\___|_||_\__/__/

template <int Owner> int nelements(const GA_Detail *geo) {
  if constexpr (Owner == GA_ATTRIB_VERTEX) {
    return geo->getNumVertices();
  } else if constexpr (Owner == GA_ATTRIB_POINT) {
    return geo->getNumPoints();
  } else if constexpr (Owner == GA_ATTRIB_PRIMITIVE) {
    return geo->getNumPrimitives();
  } else {
    return 0;
  }
}

int nelements(const GA_Detail *geo, const GA_AttributeOwner owner) {
  switch (owner) {
  case GA_ATTRIB_VERTEX:
    return nelements<GA_ATTRIB_VERTEX>(geo);
  case GA_ATTRIB_POINT:
    return nelements<GA_ATTRIB_POINT>(geo);
  case GA_ATTRIB_PRIMITIVE:
    return nelements<GA_ATTRIB_PRIMITIVE>(geo);
  case GA_ATTRIB_DETAIL:
    return 1;
  case GA_ATTRIB_OWNER_N:
    return 0;
  case GA_ATTRIB_INVALID:
    return 0;
  }
  return 0;
}

//                 _     _
//  _ _  _ __  ___(_)_ _| |_ ___
// | ' \| '_ \/ _ \ | ' \  _(_-<
// |_||_| .__/\___/_|_||_\__/__/
//      |_|

int npoints(const GA_Detail *geo) { return geo->getNumPoints(); }

int npoints() { return npoints(output_geometry()); }

int npoints(const int input_index) {
  return npoints(input_geometry(input_index));
}

//                _       _ _   _
//  _ _  _ __ _ _(_)_ __ (_) |_(_)_ _____ ___
// | ' \| '_ \ '_| | '  \| |  _| \ V / -_|_-<
// |_||_| .__/_| |_|_|_|_|_|\__|_|\_/\___/__/
//      |_|

int nprimitives(const GA_Detail *geo) { return geo->getNumPrimitives(); }

int nprimitives() { return nprimitives(output_geometry()); }

int nprimitives(const int input_index) {
  return nprimitives(input_geometry(input_index));
}

//                  _   _
//  _ ___ _____ _ _| |_(_)__ ___ ___
// | ' \ V / -_) '_|  _| / _/ -_|_-<
// |_||_\_/\___|_|  \__|_\__\___/__/

int nvertices(const GA_Detail *geo) { return geo->getNumVertices(); }

int nvertices() { return nvertices(output_geometry()); }

int nvertices(const int input_index) {
  return nvertices(input_geometry(input_index));
}

} // namespace cppvex
