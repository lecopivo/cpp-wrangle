#pragma once

#include <cmath>
#include <iostream>

#include "integrator.h"

namespace houfem {

constexpr int factorial(int n) { return n > 0 ? n * factorial(n - 1) : 1; }

//  _   _      _ _     ___ _            _
// | | | |_ _ (_) |_  / __(_)_ __  _ __| |_____ __
// | |_| | ' \| |  _| \__ \ | '  \| '_ \ / -_) \ /
//  \___/|_||_|_|\__| |___/_|_|_|_| .__/_\___/_\_\
//                                |_|

template <std::size_t Dim>
Integrator<Dim> unit_simplex_integrator(const int rule) {
  const int N = gm_rule_size(rule, Dim);

  std::vector<double> weights(N);
  std::vector<double> nodes(N * Dim);

  // Compute integration rules on an unit simplex
  gm_unit_rule_set(rule, Dim, N, &weights[0], &(nodes[0]));

  Integrator<Dim> out;

  out.weights.resize(N);
  out.integration_nodes.resize(Dim, N);

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < Dim; j++) {
      out.integration_nodes(j, i) = nodes[j + i * Dim];
    }
    out.weights(i) = weights[i];
  }

  return out;
}

//  ___ _            _
// / __(_)_ __  _ __| |_____ __
// \__ \ | '  \| '_ \ / -_) \ /
// |___/_|_|_|_| .__/_\___/_\\_
//             |_|

template <std::size_t Dim> struct Simplex {
public:
  Simplex() {}

  Simplex(const Eigen::Matrix<double, Dim, Dim + 1> &points) {

    using Matrix = Eigen::Matrix<double, Dim + 1, Dim + 1>;
    Matrix S     = Matrix::Zero(); // Input simplex
    Matrix U     = Matrix::Zero(); // Unit simplex

    S.template block<Dim, Dim + 1>(0, 0) = points;
    U.template block<Dim, Dim>(0, 1).setIdentity();
    S.row(Dim).setConstant(1.0);
    U.row(Dim).setConstant(1.0);

    Matrix from_unit_simplex_matrix = S * U.inverse();
    Matrix to_unit_simplex_matrix   = U * S.inverse();

    from_unit_simplex.A =
        from_unit_simplex_matrix.template block<Dim, Dim>(0, 0);
    from_unit_simplex.b =
        from_unit_simplex_matrix.template block<Dim, 1>(0, Dim);

    to_unit_simplex.A = to_unit_simplex_matrix.template block<Dim, Dim>(0, 0);
    to_unit_simplex.b = to_unit_simplex_matrix.template block<Dim, 1>(0, Dim);
  }

  static affine_map<1, Dim> unit_barycentric_coordinate(const int i) {
    using Vector = Eigen::Matrix<double, 1, Dim>;
    if (i == 0) {
      Vector E = Vector::Constant(-1);
      return {E, 1};
    } else {
      Vector E = Vector::Zero();
      E(i - 1) = 1;
      return {E, 0};
    }
  }

  static Eigen::Matrix<double, Dim + 1, Dim + 1> unit_mass_matrix() {

    Eigen::Matrix<double, Dim + 1, Dim + 1> M;

    auto integrator = unit_simplex_integrator<Dim>(1);

    for (int i = 0; i < Dim + 1; i++) {
      for (int j = i; j < Dim + 1; j++) {

        auto phi_i = unit_barycentric_coordinate(i);
        auto phi_j = unit_barycentric_coordinate(j);

        double value = integrator([&](Eigen::Matrix<double, Dim, 1> x) {
          return phi_i(x) * phi_j(x);
        });

        if (i == j) {
          M(i, j) = value;
        } else {
          M(i, j) = value;
          M(j, i) = value;
        }
      }
    }

    return M;
  }

  Eigen::Matrix<double, Dim, 1> point(const int i) const {
    using Vector = Eigen::Matrix<double, Dim, 1>;

    Vector E = Vector::Zero();
    if (i > 0)
      E(i - 1) = 1;

    return from_unit_simplex(E);
  }

  affine_map<1, Dim> barycentric_coordinate(const int i) const {
    return unit_barycentric_coordinate(i) | to_unit_simplex;
  }

  Integrator<Dim> integrator(const int rule) const {
    return pushforward(from_unit_simplex, unit_simplex_integrator<Dim>(rule));
  }

  double volume() const {
    return abs(from_unit_simplex.A.determinant()) / factorial(Dim);
  }

  std::array<double, Dim + 1> angles() const {
    if constexpr (Dim != 2) {
      std::cout << "Angle computation is supported only in 2D!" << std::endl;
      return {};
    } else {

      auto e0 = (point(2) - point(1)).normalized().eval();
      auto e1 = (point(1) - point(0)).normalized().eval();
      auto e2 = (point(2) - point(0)).normalized().eval();

      return {acos(e1.dot(e2)), acos(e0.dot(-e1)), acos(e2.dot(e0))};
    }
  }

  Eigen::Matrix<double, Dim + 1, Dim + 1> mass_matrix() const {
    return factorial(Dim) * volume() * unit_mass_matrix();
  }

  Eigen::Matrix<double, Dim + 1, Dim + 1> stiffness_matrix() const {
    auto integrate = integrator(0);

    auto phi = std::array{barycentric_coordinate(0), barycentric_coordinate(1),
                          barycentric_coordinate(2)};

    Eigen::Matrix<double, Dim + 1, Dim + 1> S;

    for (int i = 0; i < Dim + 1; i++) {
      for (int j = i; j < Dim + 1; j++) {

        using Vector     = Eigen::Matrix<double, Dim, 1>;
        const double val = integrate([&](Vector const &x) {
          return phi[i].derivative()(x).dot(phi[j].derivative()(x));
        });

        S(i, j) = val;
        if (i != j)
          S(j, i) = val;
      }
    }
    return S;
  }

public:
  affine_map<Dim, Dim> from_unit_simplex;
  affine_map<Dim, Dim> to_unit_simplex;

public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

//  ___       _            _    _        _   ___ _            _
// | __|_ __ | |__  ___ __| |__| |___ __| | / __(_)_ __  _ __| |_____ __
// | _|| '  \| '_ \/ -_) _` / _` / -_) _` | \__ \ | '  \| '_ \ / -_) \ /
// |___|_|_|_|_.__/\___\__,_\__,_\___\__,_| |___/_|_|_|_| .__/_\___/_\_\
//                                                      |_|

template <std::size_t SimplexDim, std::size_t SpaceDim> struct EmbeddedSimplex {

  EmbeddedSimplex(
      const Eigen::Matrix<double, SpaceDim, SimplexDim + 1> &points) {

    using Matrix = Eigen::Matrix<double, SpaceDim, SimplexDim>;
    Matrix M     = points.template block<SpaceDim, SimplexDim>(0, 1);
    M.colwise() -= points.col(0);

    Eigen::JacobiSVD<Matrix> svd(M, Eigen::ComputeFullU | Eigen::ComputeFullV);

    auto   U                                          = svd.matrixU().eval();
    Matrix adjV                                       = Matrix::Zero();
    adjV.template block<SimplexDim, SimplexDim>(0, 0) = svd.matrixV().adjoint();

    Matrix P = U * adjV;

    local_to_global.A = P;
    local_to_global.b = points.col(0);

    global_to_local.A = P.transpose();
    global_to_local.b = -P.transpose() * points.col(0);

    // Transform points to local coordinates
    Eigen::Matrix<double, SimplexDim, SimplexDim + 1>
        local_points; // Points of a simplex in local coordinates
    for (int i = 0; i < SimplexDim + 1; i++) {
      local_points.col(i) = global_to_local(points.col(i));
    }

    local_simplex = Simplex<SimplexDim>{local_points};
  }

public:
  Simplex<SimplexDim>              local_simplex;
  affine_map<SimplexDim, SpaceDim> global_to_local;
  affine_map<SpaceDim, SimplexDim> local_to_global;

public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

} // namespace houfem
