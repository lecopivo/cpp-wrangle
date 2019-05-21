#pragma once

#include "cppvex_attrib.h"

namespace cppvex {

namespace internal {

template <typename T>
class attrib_buffer {
public:
  attrib_buffer(GA_Detail *_geo, const GA_AttributeOwner owner,
                const char *attribute_name)
      : geo(_geo)
      , attr(cppvex::internal::ensure_attribute_existence<T>(geo, owner,
                                                             attribute_name))
      , range(geo->getIndexMap(owner)) {
    geo->getAttributeAsArray(attr, range, data);
  }

  ~attrib_buffer() { geo->setAttributeFromArray(attr, range, data); }

  T &operator()(const GA_Index i) { return data[i]; }
  T &operator[](const GA_Index i) { return data[i]; }

private:
  GA_Detail *   geo;
  GA_Attribute *attr;
  GA_Range      range;
  UT_Array<T>   data;
};

} // namespace internal

template <typename T>
internal::attrib_buffer<T> getattrib_buffer(GA_Detail *             geo,
                                            const GA_AttributeOwner owner,
                                            const char *attribute_name) {
  return internal::attrib_buffer<T>(geo, owner, attribute_name);
}

template <typename T>
internal::attrib_buffer<T> getattrib_buffer(const GA_AttributeOwner owner,
                                            const char *attribute_name) {
  return getattrib_buffer<T>(internal::output_geometry, owner, attribute_name);
}

} // namespace cppvex
