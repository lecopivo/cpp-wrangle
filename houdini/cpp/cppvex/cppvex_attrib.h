#pragma once

#include <vector>

#include "cppvex_types.h"

namespace cppvex {

namespace internal {

std::vector<const GU_Detail *> input_geometry;

GU_Detail *output_geometry;

template <typename T>
GA_Attribute *ensure_attribute_existence(GA_Detail *             geo,
                                         const GA_AttributeOwner owner,
                                         const char *attribute_name) {
  GA_Attribute *attr = geo->findAttribute(owner, attribute_name);
  if (!attr) {
    attr = geo->createTupleAttribute(owner, attribute_name,
                                     internal::traits::storage<T>,
                                     internal::traits::tuple_size<T>);
  }
  return attr;
}
} // namespace internal

//   ___     _       _  _   _       _ _
//  / __|___| |_    /_\| |_| |_ _ _(_) |__
// | (_ / -_)  _|  / _ \  _|  _| '_| | '_ \
//  \___\___|\__| /_/ \_\__|\__|_| |_|_.__/

// general

template <typename T>
GA_RWHandleT<T> getattrib(GA_Detail *geo, const GA_AttributeOwner owner,
                          const char *attribute_name) {
  internal::ensure_attribute_existence<T>(geo, owner, attribute_name);
  return GA_RWHandleT<T>(geo, owner, attribute_name);
}

template <typename T>
GA_ROHandleT<T> getattrib(const GA_Detail *geo, const GA_AttributeOwner owner,
                          const char *attribute_name) {
  return GA_ROHandleT<T>(geo, owner, attribute_name);
}

template <typename T>
T getattrib(const GA_Detail *geo, const GA_AttributeOwner owner,
            const char *attribute_name, const GA_Offset element_offset) {
  return getattrib<T>(geo, owner, attribute_name)(element_offset);
}

// output geometry

template <typename T>
GA_RWHandleT<T> getattrib(const GA_AttributeOwner owner,
                          const char *            attribute_name) {
  return getattrib<T>(internal::output_geometry, owner, attribute_name);
}

template <typename T>
T getattrib(const GA_AttributeOwner owner, const char *attribute_name,
            const GA_Offset element_offset) {
  return getattrib<T>(owner, attribute_name, element_offset)(element_offset);
}

// indexed input geometry

template <typename T>
GA_ROHandleT<T> getattrib(const int input_index, const GA_AttributeOwner owner,
                          const char *attribute_name) {
  return getattrib<T>(internal::input_geometry[input_index], owner,
                      attribute_name);
}

template <typename T>
T getattrib(int input_index, const GA_AttributeOwner owner,
            const char *attribute_name, const GA_Offset element_offset) {
  return getattrib<T>(input_index, owner, attribute_name)(element_offset);
}

//  ___     _       _ _   _
// | _ \_ _(_)_ __ (_) |_(_)_ _____
// |  _/ '_| | '  \| |  _| \ V / -_)
// |_| |_| |_|_|_|_|_|\__|_|\_/\___|

template <typename T>
GA_RWHandleT<T> prim(GA_Detail *geo, const char *attribute_name) {
  return getattrib<T>(geo, GA_ATTRIB_PRIMITIVE, attribute_name);
}

template <typename T>
GA_ROHandleT<T> prim(const GA_Detail *geo, const char *attribute_name) {
  return getattrib<T>(geo, GA_ATTRIB_PRIMITIVE, attribute_name);
}

template <typename T>
T prim(const GA_Detail *geo, const char *attribute_name,
       const GA_Offset prim_offset) {
  return prim<T>(geo, attribute_name)(prim_offset);
}

template <typename T>
void setprimattrib(GA_Detail *geo, const char *attribute_name,
                   const GA_Offset prim_offset, T const &value) {
  prim<T>(geo, attribute_name).set(prim_offset, value);
}

// output geometry

template <typename T>
GA_RWHandleT<T> prim(const char *attribute_name) {
  return getattrib<T>(GA_ATTRIB_PRIMITIVE, attribute_name);
}

template <typename T>
T prim(const char *attribute_name, const GA_Offset prim_offset) {
  return prim<T>(attribute_name)(prim_offset);
}

template <typename T>
void setprimattrib(const char *attribute_name, const GA_Offset prim_offset,
                   T const &value) {
  prim<T>(attribute_name).set(prim_offset, value);
}

// input geometry

template <typename T>
GA_ROHandleT<T> prim(const int input_index, const char *attribute_name) {
  return getattrib<T>(input_index, GA_ATTRIB_PRIMITIVE, attribute_name);
}

template <typename T>
T prim(const int input_index, const char *attribute_name,
       const GA_Offset prim_offset) {
  return prim<T>(input_index, attribute_name)(prim_offset);
}

//  ___     _     _
// | _ \___(_)_ _| |_
// |  _/ _ \ | ' \  _|
// |_| \___/_|_||_\__|

template <typename T>
GA_RWHandleT<T> point(GA_Detail *geo, const char *attribute_name) {
  return getattrib<T>(geo, GA_ATTRIB_POINT, attribute_name);
}

template <typename T>
GA_ROHandleT<T> point(const GA_Detail *geo, const char *attribute_name) {
  return getattrib<T>(geo, GA_ATTRIB_POINT, attribute_name);
}

template <typename T>
T point(const GA_Detail *geo, const char *attribute_name,
        const GA_Offset point_offset) {
  return point<T>(geo, attribute_name)(point_offset);
}

template <typename T>
void setpointattrib(GA_Detail *geo, const char *attribute_name,
                    const GA_Offset point_offset, T const &value) {
  point<T>(geo, attribute_name).set(point_offset, value);
}

// output geometry

template <typename T>
GA_RWHandleT<T> point(const char *attribute_name) {
  return getattrib<T>(GA_ATTRIB_POINT, attribute_name);
}

template <typename T>
T point(const char *attribute_name, const GA_Offset point_offset) {
  return point<T>(attribute_name)(point_offset);
}

template <typename T>
void setpointattrib(const char *attribute_name, const GA_Offset point_offset,
                    T const &value) {
  point<T>(attribute_name).set(point_offset, value);
}

// input geometry

template <typename T>
GA_ROHandleT<T> point(const int input_index, const char *attribute_name) {
  return getattrib<T>(input_index, GA_ATTRIB_POINT, attribute_name);
}

template <typename T>
T point(const int input_index, const char *attribute_name,
        const GA_Offset point_offset) {
  return point<T>(input_index, attribute_name)(point_offset);
}

// __   __       _
// \ \ / /__ _ _| |_ _____ __
//  \ V / -_) '_|  _/ -_) \ /
//   \_/\___|_|  \__\___/_\_\


template <typename T>
GA_RWHandleT<T> vertex(GA_Detail *geo, const char *attribute_name) {
  return getattrib<T>(geo, GA_ATTRIB_VERTEX, attribute_name);
}

template <typename T>
GA_ROHandleT<T> vertex(const GA_Detail *geo, const char *attribute_name) {
  return getattrib<T>(geo, GA_ATTRIB_VERTEX, attribute_name);
}

template <typename T>
T vertex(const GA_Detail *geo, const char *attribute_name,
         const GA_Offset vertex_offset) {
  return vertex<T>(geo, attribute_name)(vertex_offset);
}

template <typename T>
void setvertexattrib(GA_Detail *geo, const char *attribute_name,
                     const GA_Offset vertex_offset, T const &value) {
  vertex<T>(geo, attribute_name).set(vertex_offset, value);
}

// output geometry

template <typename T>
GA_RWHandleT<T> vertex(const char *attribute_name) {
  return getattrib<T>(GA_ATTRIB_VERTEX, attribute_name);
}

template <typename T>
T vertex(const char *attribute_name, const GA_Offset vertex_offset) {
  return vertex<T>(attribute_name)(vertex_offset);
}

template <typename T>
void setvertexattrib(const char *attribute_name, const GA_Offset vertex_offset,
                     T const &value) {
  vertex<T>(attribute_name).set(vertex_offset, value);
}

// input geometry

template <typename T>
GA_ROHandleT<T> vertex(const int input_index, const char *attribute_name) {
  return getattrib<T>(input_index, GA_ATTRIB_VERTEX, attribute_name);
}

template <typename T>
T vertex(const int input_index, const char *attribute_name,
         const GA_Offset vertex_offset) {
  return vertex<T>(input_index, attribute_name)(vertex_offset);
}

//  ___      _        _ _
// |   \ ___| |_ __ _(_) |
// | |) / -_)  _/ _` | | |
// |___/\___|\__\__,_|_|_|

template <typename T>
GA_RWHandleT<T> detail(GA_Detail *geo, const char *attribute_name) {
  return getattrib<T>(geo, GA_ATTRIB_DETAIL, attribute_name);
}

template <typename T>
GA_ROHandleT<T> detail(const GA_Detail *geo, const char *attribute_name) {
  return getattrib<T>(geo, GA_ATTRIB_DETAIL, attribute_name);
}

template <typename T>
T detail(const GA_Detail *geo, const char *attribute_name,
         const GA_Offset detail_offset) {
  return detail<T>(geo, attribute_name)(detail_offset);
}

template <typename T>
void setdetailattrib(GA_Detail *geo, const char *attribute_name,
                     const GA_Offset detail_offset, T const &value) {
  detail<T>(geo, attribute_name).set(detail_offset, value);
}

// output geometry

template <typename T>
GA_RWHandleT<T> detail(const char *attribute_name) {
  return getattrib<T>(GA_ATTRIB_DETAIL, attribute_name);
}

template <typename T>
T detail(const char *attribute_name, const GA_Offset detail_offset) {
  return detail<T>(attribute_name)(detail_offset);
}

template <typename T>
void setdetailattrib(const char *attribute_name, const GA_Offset detail_offset,
                    T const &value) {
  detail<T>(attribute_name).set(detail_offset, value);
}

// input geometry

template <typename T>
GA_ROHandleT<T> detail(const int input_index, const char *attribute_name) {
  return getattrib<T>(input_index, GA_ATTRIB_DETAIL, attribute_name);
}

template <typename T>
T detail(const int input_index, const char *attribute_name,
        const GA_Offset detail_offset) {
  return detail<T>(input_index, attribute_name)(detail_offset);
}

} // namespace cppvex
