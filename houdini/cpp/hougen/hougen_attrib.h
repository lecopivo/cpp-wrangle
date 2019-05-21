#pragma once

#include "../cppvex/cppvex.h"

#include "hougen_element.h"
#include "hougen_types.h"

namespace hougen {

//           _        _   _       _ _
//  __ _ ___| |_ __ _| |_| |_ _ _(_) |__
// / _` / -_)  _/ _` |  _|  _| '_| | '_ \
// \__, \___|\__\__,_|\__|\__|_| |_|_.__/
// |___/

template <class Real, int Dim>
Eigen::Matrix<Real, Eigen::Dynamic, Dim>
getattrib(const GA_Detail *geo, const GA_AttributeOwner owner,
          const char *attribute_name) {

  static_assert(Dim > 0 && Dim <= 4, "Invalid dimesion!");

  using houdini_type = cppvex::vector<Real, Dim>;
  auto handle = cppvex::getattrib<houdini_type>(geo, owner, attribute_name);
  const int N = cppvex::nelements(geo, owner);

  Eigen::Matrix<Real, Eigen::Dynamic, Dim> out(N, Dim);

  for (int i = 0; i < N; i++) {
    auto value = handle(i);

    if constexpr (Dim == 1) {
      out(i) = value;
    } else {
      for (int j = 0; j < Dim; j++) {
        out(i, j) = value[j];
      }
    }
  }

  return out;
}

// Non-constant version which creates attribute if it does not exist
template <class Real, int Dim>
Eigen::Matrix<Real, Eigen::Dynamic, Dim>
getattrib(GA_Detail *geo, const GA_AttributeOwner owner,
          const char *attribute_name) {

  // This creates the attribute in case it does not exist
  using houdini_type = cppvex::vector<Real, Dim>;
  auto handle = cppvex::getattrib<houdini_type>(geo, owner, attribute_name);

  const GA_Detail *const_geo = geo;

  // call const version of this function
  return getattrib<Real, Dim>(const_geo, owner, attribute_name);
}

//          _        _   _       _ _
//  ___ ___| |_ __ _| |_| |_ _ _(_) |__
// (_-</ -_)  _/ _` |  _|  _| '_| | '_ \
// /__/\___|\__\__,_|\__|\__|_| |_|_.__/

template <class Derived>
void setattrib(GA_Detail *geo, const GA_AttributeOwner owner,
               const char *                      attribute_name,
               Eigen::MatrixBase<Derived> const &values) {

  constexpr int Dim = Eigen::internal::traits<Derived>::ColsAtCompileTime;
  using Real        = typename Derived::Scalar;
  using houdini_type =
      hougen_type_traits::houdini_equivalent_type<hougen::Vector<Real, Dim>>;

  static_assert(!std::is_same_v<houdini_type, void>,
                "There is no equivalent Houdini type!");

  auto handle = cppvex::getattrib<houdini_type>(geo, owner, attribute_name);
  const int N = values.rows();

  // This should be turned into some sensible error
  if (N != cppvex::nelements(geo, owner)) {
    std::cout << "Error in `setattrib`: Dimension of the matrix does not match "
                 "the number of elements!"
              << std::endl;
  }

  for (int i = 0; i < N; i++) {

    houdini_type value;

    if constexpr (Dim == 1) {
      value = values(i);
    } else {
      for (int j = 0; j < Dim; j++) {
        value[j] = values(i, j);
      }
    }

    handle.set(i, value);
  }
}

//  ___     _       _ _   _
// | _ \_ _(_)_ __ (_) |_(_)_ _____
// |  _/ '_| | '  \| |  _| \ V / -_)
// |_| |_| |_|_|_|_|_|\__|_|\_/\___|

// prim
template <class Real, int Dim>
Eigen::Matrix<Real, Eigen::Dynamic, Dim> prim(const GA_Detail *geo,
                                              const char *     attribute_name) {
  return getattrib<Real, Dim>(geo, GA_ATTRIB_PRIMITIVE, attribute_name);
}

// non-constant version - creates the attribute if it does not exist
template <class Real, int Dim>
Eigen::Matrix<Real, Eigen::Dynamic, Dim> prim(GA_Detail * geo,
                                              const char *attribute_name) {
  return getattrib<Real, Dim>(geo, GA_ATTRIB_PRIMITIVE, attribute_name);
}

// output geometry version
template <class Real, int Dim>
Eigen::Matrix<Real, Eigen::Dynamic, Dim> prim(const char *attribute_name) {
  return prim<Real, Dim>(cppvex::output_geometry(), attribute_name);
}

// input geometry version
template <class Real, int Dim>
Eigen::Matrix<Real, Eigen::Dynamic, Dim> prim(const int   input_index,
                                              const char *attribute_name) {
  return prim<Real, Dim>(cppvex::input_geometry(input_index), attribute_name);
}

// setprimattrib

template <class Derived>
void setprimattrib(GA_Detail *geo, const char *attribute_name,
                   Eigen::MatrixBase<Derived> const &values) {
  setattrib(geo, GA_ATTRIB_PRIMITIVE, attribute_name, values);
}

// output geometry version
template <class Derived>
void setprimattrib(const char *                      attribute_name,
                   Eigen::MatrixBase<Derived> const &values) {
  setprimattrib(cppvex::output_geometry(), attribute_name, values);
}

//  ___     _     _
// | _ \___(_)_ _| |_
// |  _/ _ \ | ' \  _|
// |_| \___/_|_||_\__|

// point
template <class Real, int Dim>
Eigen::Matrix<Real, Eigen::Dynamic, Dim> point(const GA_Detail *geo,
                                               const char *attribute_name) {
  return getattrib<Real, Dim>(geo, GA_ATTRIB_POINT, attribute_name);
}

// non-constant version - creates the attribute if it does not exist
template <class Real, int Dim>
Eigen::Matrix<Real, Eigen::Dynamic, Dim> point(GA_Detail * geo,
                                               const char *attribute_name) {
  return getattrib<Real, Dim>(geo, GA_ATTRIB_POINT, attribute_name);
}

// output geometry version
template <class Real, int Dim>
Eigen::Matrix<Real, Eigen::Dynamic, Dim> point(const char *attribute_name) {
  return point<Real, Dim>(cppvex::output_geometry(), attribute_name);
}

// input geometry version
template <class Real, int Dim>
Eigen::Matrix<Real, Eigen::Dynamic, Dim> point(const int   input_index,
                                               const char *attribute_name) {
  return point<Real, Dim>(cppvex::input_geometry(input_index), attribute_name);
}

// setpointattrib
template <class Derived>
void setpointattrib(GA_Detail *geo, const char *attribute_name,
                    Eigen::MatrixBase<Derived> const &values) {
  setattrib(geo, GA_ATTRIB_POINT, attribute_name, values);
}

// output geometry version
template <class Derived>
void setpointattrib(const char *                      attribute_name,
                    Eigen::MatrixBase<Derived> const &values) {
  setpointattrib(cppvex::output_geometry(), attribute_name, values);
}

// __   __       _
// \ \ / /__ _ _| |_ _____ __
//  \ V / -_) '_|  _/ -_) \ /
//   \_/\___|_|  \__\___/_\_\

// vertex
template <class Real, int Dim>
Eigen::Matrix<Real, Eigen::Dynamic, Dim> vertex(const GA_Detail *geo,
                                                const char *attribute_name) {
  return getattrib<Real, Dim>(geo, GA_ATTRIB_VERTEX, attribute_name);
}

// non-constant version - creates the attribute if it does not exist
template <class Real, int Dim>
Eigen::Matrix<Real, Eigen::Dynamic, Dim> vertex(GA_Detail * geo,
                                                const char *attribute_name) {
  return getattrib<Real, Dim>(geo, GA_ATTRIB_VERTEX, attribute_name);
}

// output geometry version
template <class Real, int Dim>
Eigen::Matrix<Real, Eigen::Dynamic, Dim> vertex(const char *attribute_name) {
  return vertex<Real, Dim>(cppvex::output_geometry(), attribute_name);
}

// input geometry version
template <class Real, int Dim>
Eigen::Matrix<Real, Eigen::Dynamic, Dim> vertex(const int   input_index,
                                                const char *attribute_name) {
  return vertex<Real, Dim>(cppvex::input_geometry(input_index), attribute_name);
}

// setvertexattrib
template <class Derived>
void setvertexattrib(GA_Detail *geo, const char *attribute_name,
                     Eigen::MatrixBase<Derived> const &values) {
  setattrib(geo, GA_ATTRIB_VERTEX, attribute_name, values);
}

// output geometry version
template <class Derived>
void setvertexattrib(const char *                      attribute_name,
                     Eigen::MatrixBase<Derived> const &values) {
  setvertexattrib(cppvex::output_geometry(), attribute_name, values);
}

} // namespace hougen
