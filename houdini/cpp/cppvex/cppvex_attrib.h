#pragma once

#include <vector>

#include "cppvex_context.h"
#include "cppvex_element.h"
#include "cppvex_types.h"

namespace cppvex {

namespace internal {

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

template <typename T> struct AttributeReadHandle {

  AttributeReadHandle(const GA_Detail *_geo, const GA_AttributeOwner _owner,
                      const char *_attribute_name)
      : geo(_geo), owner(_owner), attribute_name(_attribute_name),
        handle(_geo, _owner, _attribute_name) {}

  T get(const GA_Index index) const {
    GA_Offset offset = elementOffset(geo, owner, index);
    return handle(offset);
  }

  T operator()(const GA_Index index) const { return get(index); }

private:
  const GA_Detail *       geo;
  const GA_ROHandleT<T>   handle;
  const GA_AttributeOwner owner;
  const std::string       attribute_name;
};

template <typename T> struct AttributeWriteHandle {

  AttributeWriteHandle(GA_Detail *_geo, const GA_AttributeOwner _owner,
                       const char *_attribute_name)
      : geo(_geo), owner(_owner), attribute_name(_attribute_name),
        handle(_geo, _owner, _attribute_name) {}

  T get(const GA_Index index) const {
    GA_Offset offset = elementOffset(geo, owner, index);
    return handle(offset);
  }

  T operator()(const GA_Index index) const { return get(index); }

  void set(const GA_Index index, T const &value) {
    const GA_Offset offset = elementOffset(geo, owner, index);
    handle.set(offset, value);
  }

  void add(const GA_Index index, T const &value) {
    T org_value = get(index);
    set(index, org_value + value);
  }

private:
  GA_Detail *             geo;
  GA_RWHandleT<T>         handle;
  const GA_AttributeOwner owner;
  const std::string       attribute_name;
};

//   ___     _       _  _   _       _ _
//  / __|___| |_    /_\| |_| |_ _ _(_) |__
// | (_ / -_)  _|  / _ \  _|  _| '_| | '_ \
//  \___\___|\__| /_/ \_\__|\__|_| |_|_.__/

// general

template <typename T>
AttributeWriteHandle<T> getattrib(GA_Detail *geo, const GA_AttributeOwner owner,
                                  const char *attribute_name) {
  internal::ensure_attribute_existence<T>(geo, owner, attribute_name);
  return AttributeWriteHandle<T>(geo, owner, attribute_name);
}

template <typename T>
AttributeReadHandle<T> getattrib(const GA_Detail *       geo,
                                 const GA_AttributeOwner owner,
                                 const char *            attribute_name) {
  return AttributeReadHandle<T>(geo, owner, attribute_name);
}

template <typename T>
T getattrib(const GA_Detail *geo, const GA_AttributeOwner owner,
            const char *attribute_name, const GA_Index element_index) {
  return getattrib<T>(geo, owner, attribute_name)(element_index);
}

// output geometry

template <typename T>
AttributeWriteHandle<T> getattrib(const GA_AttributeOwner owner,
                                  const char *            attribute_name) {
  return getattrib<T>(output_geometry(), owner, attribute_name);
}

template <typename T>
T getattrib(const GA_AttributeOwner owner, const char *attribute_name,
            const GA_Index element_index) {
  return getattrib<T>(owner, attribute_name, element_index)(element_index);
}

// indexed input geometry

template <typename T>
AttributeReadHandle<T> getattrib(const int               input_index,
                                 const GA_AttributeOwner owner,
                                 const char *            attribute_name) {
  return getattrib<T>(input_geometry(input_index), owner, attribute_name);
}

template <typename T>
T getattrib(int input_index, const GA_AttributeOwner owner,
            const char *attribute_name, const GA_Index element_index) {
  return getattrib<T>(input_index, owner, attribute_name)(element_index);
}

//  ___     _       _ _   _
// | _ \_ _(_)_ __ (_) |_(_)_ _____
// |  _/ '_| | '  \| |  _| \ V / -_)
// |_| |_| |_|_|_|_|_|\__|_|\_/\___|

template <typename T>
AttributeWriteHandle<T> prim(GA_Detail *geo, const char *attribute_name) {
  return getattrib<T>(geo, GA_ATTRIB_PRIMITIVE, attribute_name);
}

template <typename T>
AttributeReadHandle<T> prim(const GA_Detail *geo, const char *attribute_name) {
  return getattrib<T>(geo, GA_ATTRIB_PRIMITIVE, attribute_name);
}

template <typename T>
T prim(const GA_Detail *geo, const char *attribute_name,
       const GA_Index prim_index) {
  return prim<T>(geo, attribute_name)(prim_index);
}

template <typename T>
void setprimattrib(GA_Detail *geo, const char *attribute_name,
                   const GA_Index prim_index, T const &value) {
  prim<T>(geo, attribute_name).set(prim_index, value);
}

// output geometry

template <typename T> AttributeWriteHandle<T> prim(const char *attribute_name) {
  return getattrib<T>(GA_ATTRIB_PRIMITIVE, attribute_name);
}

template <typename T>
T prim(const char *attribute_name, const GA_Index prim_index) {
  return prim<T>(attribute_name)(prim_index);
}

template <typename T>
void setprimattrib(const char *attribute_name, const GA_Index prim_index,
                   T const &value) {
  prim<T>(attribute_name).set(prim_index, value);
}

// input geometry

template <typename T>
AttributeReadHandle<T> prim(const int input_index, const char *attribute_name) {
  return getattrib<T>(input_index, GA_ATTRIB_PRIMITIVE, attribute_name);
}

template <typename T>
T prim(const int input_index, const char *attribute_name,
       const GA_Index prim_index) {
  return prim<T>(input_index, attribute_name)(prim_index);
}

//  ___     _     _
// | _ \___(_)_ _| |_
// |  _/ _ \ | ' \  _|
// |_| \___/_|_||_\__|

template <typename T>
AttributeWriteHandle<T> point(GA_Detail *geo, const char *attribute_name) {
  return getattrib<T>(geo, GA_ATTRIB_POINT, attribute_name);
}

template <typename T>
AttributeReadHandle<T> point(const GA_Detail *geo, const char *attribute_name) {
  return getattrib<T>(geo, GA_ATTRIB_POINT, attribute_name);
}

template <typename T>
T point(const GA_Detail *geo, const char *attribute_name,
        const GA_Index point_index) {
  return point<T>(geo, attribute_name)(point_index);
}

template <typename T>
void setpointattrib(GA_Detail *geo, const char *attribute_name,
                    const GA_Index point_index, T const &value) {
  point<T>(geo, attribute_name).set(point_index, value);
}

// output geometry

template <typename T>
AttributeWriteHandle<T> point(const char *attribute_name) {
  return getattrib<T>(GA_ATTRIB_POINT, attribute_name);
}

template <typename T>
T point(const char *attribute_name, const GA_Index point_index) {
  return point<T>(attribute_name)(point_index);
}

template <typename T>
void setpointattrib(const char *attribute_name, const GA_Index point_index,
                    T const &value) {
  point<T>(attribute_name).set(point_index, value);
}

// input geometry

template <typename T>
AttributeReadHandle<T> point(const int   input_index,
                             const char *attribute_name) {
  return getattrib<T>(input_index, GA_ATTRIB_POINT, attribute_name);
}

template <typename T>
T point(const int input_index, const char *attribute_name,
        const GA_Index point_index) {
  return point<T>(input_index, attribute_name)(point_index);
}

// __   __       _
// \ \ / /__ _ _| |_ _____ __
//  \ V / -_) '_|  _/ -_) \ /
//   \_/\___|_|  \__\___/_\_\


template <typename T>
AttributeWriteHandle<T> vertex(GA_Detail *geo, const char *attribute_name) {
  return getattrib<T>(geo, GA_ATTRIB_VERTEX, attribute_name);
}

template <typename T>
AttributeReadHandle<T> vertex(const GA_Detail *geo,
                              const char *     attribute_name) {
  return getattrib<T>(geo, GA_ATTRIB_VERTEX, attribute_name);
}

template <typename T>
T vertex(const GA_Detail *geo, const char *attribute_name,
         const GA_Index vertex_index) {
  return vertex<T>(geo, attribute_name)(vertex_index);
}

template <typename T>
void setvertexattrib(GA_Detail *geo, const char *attribute_name,
                     const GA_Index vertex_index, T const &value) {
  vertex<T>(geo, attribute_name).set(vertex_index, value);
}

// output geometry

template <typename T>
AttributeWriteHandle<T> vertex(const char *attribute_name) {
  return getattrib<T>(GA_ATTRIB_VERTEX, attribute_name);
}

template <typename T>
T vertex(const char *attribute_name, const GA_Index vertex_index) {
  return vertex<T>(attribute_name)(vertex_index);
}

template <typename T>
void setvertexattrib(const char *attribute_name, const GA_Index vertex_index,
                     T const &value) {
  vertex<T>(attribute_name).set(vertex_index, value);
}

// input geometry

template <typename T>
AttributeReadHandle<T> vertex(const int   input_index,
                              const char *attribute_name) {
  return getattrib<T>(input_index, GA_ATTRIB_VERTEX, attribute_name);
}

template <typename T>
T vertex(const int input_index, const char *attribute_name,
         const GA_Index vertex_index) {
  return vertex<T>(input_index, attribute_name)(vertex_index);
}

//  ___      _        _ _
// |   \ ___| |_ __ _(_) |
// | |) / -_)  _/ _` | | |
// |___/\___|\__\__,_|_|_|

template <typename T>
AttributeWriteHandle<T> detail(GA_Detail *geo, const char *attribute_name) {
  return getattrib<T>(geo, GA_ATTRIB_DETAIL, attribute_name);
}

template <typename T>
AttributeReadHandle<T> detail(const GA_Detail *geo,
                              const char *     attribute_name) {
  return getattrib<T>(geo, GA_ATTRIB_DETAIL, attribute_name);
}

template <typename T>
T detail(const GA_Detail *geo, const char *attribute_name,
         const GA_Index detail_index) {
  return detail<T>(geo, attribute_name)(detail_index);
}

template <typename T>
void setdetailattrib(GA_Detail *geo, const char *attribute_name,
                     const GA_Index detail_index, T const &value) {
  detail<T>(geo, attribute_name).set(detail_index, value);
}

// output geometry

template <typename T>
AttributeWriteHandle<T> detail(const char *attribute_name) {
  return getattrib<T>(GA_ATTRIB_DETAIL, attribute_name);
}

template <typename T>
T detail(const char *attribute_name, const GA_Index detail_index) {
  return detail<T>(attribute_name)(detail_index);
}

template <typename T>
void setdetailattrib(const char *attribute_name, const GA_Index detail_index,
                     T const &value) {
  detail<T>(attribute_name).set(detail_index, value);
}

// input geometry

template <typename T>
AttributeReadHandle<T> detail(const int   input_index,
                              const char *attribute_name) {
  return getattrib<T>(input_index, GA_ATTRIB_DETAIL, attribute_name);
}

template <typename T>
T detail(const int input_index, const char *attribute_name,
         const GA_Index detail_index) {
  return detail<T>(input_index, attribute_name)(detail_index);
}

} // namespace cppvex
