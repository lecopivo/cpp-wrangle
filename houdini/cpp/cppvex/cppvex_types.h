#pragma once

#include <type_traits>

namespace cppvex {

namespace internal {

template <class Real, std::size_t Dim> struct vector {
  using type = UT_FixedVector<Real, Dim, true>;
};

template <class Real> struct vector<Real, 1> { using type = Real; };

template <class Real> struct vector<Real, 2> {
  using type = UT_Vector2T<Real>;
};
template <class Real> struct vector<Real, 3> {
  using type = UT_Vector3T<Real>;
};
template <class Real> struct vector<Real, 4> {
  using type = UT_Vector4T<Real>;
};

template <class Real, std::size_t RowDim, std::size_t ColDim> struct matrix {
  using type = void;
};

template <class Real> struct matrix<Real, 1, 1> { using type = Real; };

template <class Real> struct matrix<Real, 2, 2> {
  using type = UT_Matrix2T<Real>;
};
template <class Real> struct matrix<Real, 3, 3> {
  using type = UT_Matrix3T<Real>;
};
template <class Real> struct matrix<Real, 4, 4> {
  using type = UT_Matrix4T<Real>;
};

} // namespace internal

template <class Real, std::size_t Dim>
using vector = typename internal::vector<Real, Dim>::type;

using vector2f = vector<float, 2>;
using vector3f = vector<float, 3>;
using vector4f = vector<float, 4>;

using vector2d = vector<double, 2>;
using vector3d = vector<double, 3>;
using vector4d = vector<double, 4>;

using vector2 = vector2f;
using vector3 = vector3f;
using vector4 = vector4f;

template <class Real, std::size_t RowDim, std::size_t ColDim>
using matrix = typename internal::matrix<Real, RowDim, ColDim>::type;

using matrix2f = matrix<float, 2, 2>;
using matrix3f = matrix<float, 3, 3>;
using matrix4f = matrix<float, 4, 4>;

using matrix2d = matrix<double, 2, 2>;
using matrix3d = matrix<double, 3, 3>;
using matrix4d = matrix<double, 4, 4>;

using matrix2 = matrix2f;
using matrix3 = matrix3f;
using matrix4 = matrix4f;

namespace internal::traits {

template <typename T> constexpr GA_Storage storage = storage<typename T::value_type>;

template <> constexpr GA_Storage storage<int>    = GA_STORE_INT32;
template <> constexpr GA_Storage storage<float>  = GA_STORE_REAL32;
template <> constexpr GA_Storage storage<double> = GA_STORE_REAL64;

template <typename T> constexpr int tuple_size_call() {
  if constexpr (std::is_same<int, T>::value || std::is_same<float, T>::value || std::is_same<double, T>::value){
    return 1;
  }else{
    return T::tuple_size;
  }
}

template <typename T> constexpr int tuple_size = tuple_size_call<T>();

} // namespace internal::traits
} // namespace cppvex
