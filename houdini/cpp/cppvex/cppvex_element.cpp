#include "cppvex_element.h"

namespace cppvex {

GA_Size getNumElements(const GA_Detail *geo, const GA_AttributeOwner owner) {
  switch (owner) {
  case GA_ATTRIB_VERTEX:
    return geo->getNumVertices();
  case GA_ATTRIB_POINT:
    return geo->getNumPoints();
  case GA_ATTRIB_PRIMITIVE:
    return geo->getNumPrimitives();
  case GA_ATTRIB_DETAIL:
    return 1;
  case GA_ATTRIB_OWNER_N:
    return 0;
  case GA_ATTRIB_INVALID:
    return 0;
  }
  return 0;
}

GA_Offset elementOffset(const GA_Detail *geo, const GA_AttributeOwner owner,
                        const GA_Index index) {
  switch (owner) {
  case GA_ATTRIB_VERTEX:
    return elemenOffset<GA_ATTRIB_VERTEX>(geo, index);
  case GA_ATTRIB_POINT:
    return elemenOffset<GA_ATTRIB_POINT>(geo, index);
  case GA_ATTRIB_PRIMITIVE:
    return elemenOffset<GA_ATTRIB_PRIMITIVE>(geo, index);
  case GA_ATTRIB_DETAIL:
    return 0;
  case GA_ATTRIB_OWNER_N:
    return 0;
  case GA_ATTRIB_INVALID:
    return 0;
  }
  return 0;
}

GA_Index elementIndex(const GA_Detail *geo, const GA_AttributeOwner owner,
                      const GA_Offset offset) {
  switch (owner) {
  case GA_ATTRIB_VERTEX:
    return geo->vertexIndex(offset);
  case GA_ATTRIB_POINT:
    return geo->pointIndex(offset);
  case GA_ATTRIB_PRIMITIVE:
    return geo->primitiveIndex(offset);
  case GA_ATTRIB_DETAIL:
    return 0;
  case GA_ATTRIB_OWNER_N:
    return 0;
  case GA_ATTRIB_INVALID:
    return 0;
  }
  return 0;
}

} // namespace cppvex
