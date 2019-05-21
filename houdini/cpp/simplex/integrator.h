#pragma once

#include <vector>

#include "affine_map.h"
#include "simplex_gm_rule.h"

namespace simplex{

template <std::size_t Dim> struct Integrator {
  using Vector = Eigen::Matrix<double, Dim, 1>;

  template <class Fun> double operator()(Fun const &fun) {
    double out = 0;
    for (int i = 0; i < weights.size(); i++) {
      out += weights(i) * fun(integration_nodes.col(i).eval());
    }
    return out;
  }

  Eigen::Matrix<double, Eigen::Dynamic, 1>   weights;
  Eigen::Matrix<double, Dim, Eigen::Dynamic> integration_nodes;

public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

template <std::size_t Dim>
Integrator<Dim> pushforward(affine_map<Dim, Dim> const &map,
                            Integrator<Dim>             integrator) {

  auto &weights = integrator.weights;
  auto &nodes   = integrator.integration_nodes;

  weights *= abs(map.A.determinant());

  for (int i = 0; i < nodes.cols(); i++) {
    nodes.col(i) = map(nodes.col(i));
  }

  return integrator;
}

}
