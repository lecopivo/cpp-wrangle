#pragma once

#include "hougen_element.h"
#include "hougen_types.h"

namespace hougen {

template <class T,
          class = std::enable_if_t<hougen_type_traits::is_hougen_type<T>>>
Eigen::Matrix<hougen_type_traits::scalar_type<T>, hougen_type_traits::dim<T>,
              Eigen::Dynamic>
getattrib(const GA_Detail *geo, const GA_AttributeOwner owner,
          const char *attribute_name) {

  constexpr int Dim  = hougen_type_traits::dim<T>;
  using Real         = hougen_type_traits::scalar_type<T>;
  using houdini_type = hougen_type_traits::houdini_equivalent_type<T>;

  static_assert(!std::is_same_v<houdini_type, void>,
                "There is no equivalent Houdini type!");
  static_assert(hougen_type_traits::col_dim<T> == 1,
                "Only vector attributes are supported right now!");

  auto handle = GA_ROHandleT<houdini_type>(geo, owner, attribute_name);

  const int N = getNumElements(geo, owner);

  Eigen::Matrix<Real, Dim, Eigen::Dynamic> out(Dim, N);

  for (int j = 0; j < N; j++) {
    auto offset = elementOffset(geo, owner, j);
    auto value  = handle(offset);

    if constexpr (Dim == 1) {
      out(0, j) = value;
    } else {
      for (int i = 0; i < Dim; i++) {
        out(i, j) = value[i];
      }
    }
  }

  return out;
}

template <class Derived>
void setattrib(GA_Detail *geo, const GA_AttributeOwner owner,
               const char *                      attribute_name,
               Eigen::MatrixBase<Derived> const &values) {

  constexpr int Dim = Eigen::internal::traits<Derived>::RowsAtCompileTime;
  using Real        = typename Derived::Scalar;
  using houdini_type =
      hougen_type_traits::houdini_equivalent_type<hougen::Vector<Real, Dim>>;

  static_assert(!std::is_same_v<houdini_type, void>,
                "There is no equivalent Houdini type!");

  auto handle = GA_RWHandleT<houdini_type>(geo, owner, attribute_name);

  const int N = values.cols();

  for (int j = 0; j < N; j++) {
    auto offset = elementOffset(geo, owner, j);

    houdini_type value;
    if constexpr (Dim == 1) {
      value = values(0, j);
    } else {
      for (int i = 0; i < Dim; i++) {
        value[i] = values(i, j);
      }
    }

    handle.set(offset, value);
  }
}

} // namespace hougen
