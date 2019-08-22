#pragma once

#include <GA/GA_Detail.h>

namespace cppvex {
  
GA_Size getNumElements(const GA_Detail *geo, const GA_AttributeOwner owner);

template <int Owner>
GA_Offset elemenOffset(const GA_Detail *geo, const GA_Index index) {

  if constexpr (Owner == GA_ATTRIB_VERTEX) {
    return geo->vertexOffset(index);
  } else if constexpr (Owner == GA_ATTRIB_POINT) {
    return geo->pointOffset(index);
  } else if constexpr (Owner == GA_ATTRIB_PRIMITIVE) {
    return geo->primitiveOffset(index);
  } else {
    return 0;
  }
}

GA_Offset elementOffset(const GA_Detail *geo, const GA_AttributeOwner owner,
                        const GA_Index index);
  
GA_Index elementIndex(const GA_Detail *geo, const GA_AttributeOwner owner,
                      const GA_Offset offset);
} // namespace cppvex
