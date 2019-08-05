#pragma once

#include <Eigen/Dense>

namespace houfem {

namespace internal {

template <class Real, std::size_t Dim> struct EigenVector {
  using type = Eigen::Matrix<Real, Dim, 1>;
};

template <class Real> struct EigenVector<Real, 1> { using type = Real; };

} // namespace internal

template <class Real, std::size_t Dim>
using EigenVector = typename internal::EigenVector<Real, Dim>::type;

//   ___             _            _     __  __
//  / __|___ _ _  __| |_ __ _ _ _| |_  |  \/  |__ _ _ __
// | (__/ _ \ ' \(_-<  _/ _` | ' \  _| | |\/| / _` | '_ \
//  \___\___/_||_/__/\__\__,_|_||_\__| |_|  |_\__,_| .__/
//                                                 |_|

template <class Target, class Source> struct constant_map {

  Target operator()(const Source &) const { return value; }

public:
  Target value;

public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

//  _    _                     __  __
// | |  (_)_ _  ___ __ _ _ _  |  \/  |__ _ _ __
// | |__| | ' \/ -_) _` | '_| | |\/| / _` | '_ \
// |____|_|_||_\___\__,_|_|   |_|  |_\__,_| .__/
//                                        |_|

template <std::size_t TargetDim, std::size_t SourceDim, class Real = double>
struct linear_map {

  using Matrix       = Eigen::Matrix<Real, TargetDim, SourceDim>;
  using TargetVector = EigenVector<Real, TargetDim>;
  using SourceVector = EigenVector<Real, SourceDim>;

  TargetVector operator()(const SourceVector &x) const { return A * x; }

  linear_map<SourceDim, TargetDim> inverse() const { return {A.inverse()}; }

  constant_map<Matrix, SourceVector> derivative() const { return {A}; }

public:
  Matrix A;

public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

//    _    __  __ _            __  __
//   /_\  / _|/ _(_)_ _  ___  |  \/  |__ _ _ __
//  / _ \|  _|  _| | ' \/ -_) | |\/| / _` | '_ \
// /_/ \_\_| |_| |_|_||_\___| |_|  |_\__,_| .__/
//                                        |_|

template <std::size_t TargetDim, std::size_t SourceDim, class Real = double>
struct affine_map {

  using Matrix       = Eigen::Matrix<Real, TargetDim, SourceDim>;
  using TargetVector = EigenVector<Real, TargetDim>;
  using SourceVector = EigenVector<Real, SourceDim>;

  TargetVector operator()(const SourceVector &x) const { return A * x + b; }

  affine_map<SourceDim, TargetDim> inverse() const {
    Matrix       iA = A.inverse();
    TargetVector ib = -iA * b;
    return {iA, ib};
  }

  constant_map<Matrix, SourceVector> derivative() { return {A}; }

public:
  Matrix       A;
  TargetVector b;

public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

//   ___                        _ _   _
//  / __|___ _ __  _ __  ___ __(_) |_(_)___ _ _  ___
// | (__/ _ \ '  \| '_ \/ _ (_-< |  _| / _ \ ' \(_-<
//  \___\___/_|_|_| .__/\___/__/_|\__|_\___/_||_/__/
//                |_|

template <std::size_t TargetDim, std::size_t MiddleDim, std::size_t SourceDim>
affine_map<TargetDim, SourceDim>
operator|(affine_map<TargetDim, MiddleDim> const &f,
          affine_map<MiddleDim, SourceDim> const &g) {
  return {f.A * g.A, f.A * g.b + f.b};
}

template <std::size_t TargetDim, std::size_t MiddleDim, std::size_t SourceDim>
affine_map<TargetDim, SourceDim>
operator|(linear_map<TargetDim, MiddleDim> const &f,
          affine_map<MiddleDim, SourceDim> const &g) {
  return {f.A * g.A, f.A * g.b};
}

template <std::size_t TargetDim, std::size_t MiddleDim, std::size_t SourceDim>
affine_map<TargetDim, SourceDim>
operator|(affine_map<TargetDim, MiddleDim> const &f,
          linear_map<MiddleDim, SourceDim> const &g) {
  return {f.A * g.A, f.b};
}

template <std::size_t TargetDim, std::size_t MiddleDim, std::size_t SourceDim>
linear_map<TargetDim, SourceDim>
operator|(linear_map<TargetDim, MiddleDim> const &f,
          linear_map<MiddleDim, SourceDim> const &g) {
  return {f.A * g.A};
}

} // namespace simplex
