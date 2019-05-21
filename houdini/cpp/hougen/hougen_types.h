#pragma once

#include <type_traits>

namespace hougen {

namespace internal {
template <class Real, int Dim> struct Vector {
  using type = Eigen::Matrix<Real, Dim, 1>;
};

template <class Real> struct Vector<Real, 1> { using type = Real; };
} // namespace internal

template <class Real, int Dim> using Vector = typename internal::Vector<Real, Dim>::type;

using Vector2f = Vector<float, 2>;
using Vector3f = Vector<float, 3>;
using Vector4f = Vector<float, 4>;

using Vector2d = Vector<double, 2>;
using Vector3d = Vector<double, 3>;
using Vector4d = Vector<double, 4>;

using Vector2 = Vector2f;
using Vector3 = Vector3f;
using Vector4 = Vector4f;

template <class Real, int RowDim, int ColDim>
using Matrix = Eigen::Matrix<Real, RowDim, ColDim>;

using Matrix2f = Matrix<float, 2, 2>;
using Matrix3f = Matrix<float, 3, 3>;
using Matrix4f = Matrix<float, 4, 4>;

using Matrix2d = Matrix<double, 2, 2>;
using Matrix3d = Matrix<double, 3, 3>;
using Matrix4d = Matrix<double, 4, 4>;

using Matrix2 = Matrix2f;
using Matrix3 = Matrix3f;
using Matrix4 = Matrix4f;

namespace hougen_type_traits {
namespace internal {

template <class T> struct houdini_equivalent_type { using type = void; };
template <> struct houdini_equivalent_type<float> { using type = float; };
template <> struct houdini_equivalent_type<double> { using type = double; };

template <> struct houdini_equivalent_type<Vector2f> {
  using type = UT_Vector2F;
};
template <> struct houdini_equivalent_type<Vector2d> {
  using type = UT_Vector2D;
};
template <> struct houdini_equivalent_type<Vector3f> {
  using type = UT_Vector3F;
};
template <> struct houdini_equivalent_type<Vector3d> {
  using type = UT_Vector3D;
};
template <> struct houdini_equivalent_type<Vector4f> {
  using type = UT_Vector4F;
};
template <> struct houdini_equivalent_type<Vector4d> {
  using type = UT_Vector4D;
};
template <> struct houdini_equivalent_type<Matrix2f> {
  using type = UT_Matrix2F;
};
template <> struct houdini_equivalent_type<Matrix2d> {
  using type = UT_Matrix2D;
};
template <> struct houdini_equivalent_type<Matrix3f> {
  using type = UT_Matrix3F;
};
template <> struct houdini_equivalent_type<Matrix3d> {
  using type = UT_Matrix3D;
};
template <> struct houdini_equivalent_type<Matrix4f> {
  using type = UT_Matrix4F;
};
template <> struct houdini_equivalent_type<Matrix4d> {
  using type = UT_Matrix4D;
};

template <class T> struct is_hougen_type {
  static constexpr bool value = false;
};
template <> struct is_hougen_type<float> {
  static constexpr bool value = true;
};
template <> struct is_hougen_type<double> {
  static constexpr bool value = true;
};

template <class Real, int RowDim, int ColDim>
struct is_hougen_type<Matrix<Real, RowDim, ColDim>> {
  static constexpr bool value = (RowDim > 0) && (ColDim > 0);
};

template <class T> struct row_dim { static constexpr int value = 0; };
template <> struct row_dim<float> { static constexpr int value = 1; };
template <> struct row_dim<double> { static constexpr int value = 1; };

template <class Real, int RowDim, int ColDim>
struct row_dim<Matrix<Real, RowDim, ColDim>> {
  static constexpr int value = RowDim;
};

template <class T> struct col_dim { static constexpr int value = 0; };
template <> struct col_dim<float> { static constexpr int value = 1; };
template <> struct col_dim<double> { static constexpr int value = 1; };

template <class Real, int RowDim, int ColDim>
struct col_dim<Matrix<Real, RowDim, ColDim>> {
  static constexpr int value = ColDim;
};

template <class T> struct scalar_type { using type = void; };
template <> struct scalar_type<float> { using type = float; };
template <> struct scalar_type<double> { using type = double; };

template <class Real, int RowDim, int ColDim>
struct scalar_type<Matrix<Real, RowDim, ColDim>> {
  using type = Real;
};

} // namespace internal

template <class T>
using houdini_equivalent_type =
    typename internal::houdini_equivalent_type<T>::type;

template <class T>
constexpr bool is_hougen_type = internal::is_hougen_type<T>::value;

template <class T> constexpr int row_dim = internal::row_dim<T>::value;

template <class T> constexpr int col_dim = internal::col_dim<T>::value;

template <class T> constexpr int dim = row_dim<T> *col_dim<T>;

template <class T> using scalar_type = typename internal::scalar_type<T>::type;
} // namespace hougen_type_traits

} // namespace hougen
