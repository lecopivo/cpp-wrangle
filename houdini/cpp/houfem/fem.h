#pragma once

#include <Eigen/Sparse>
#include <array>
#include <vector>

//#include "../hougen/hougen.h"
#include "simplex.h"

#include <GA/GA_Detail.h>
#include <GA/GA_Primitive.h>

namespace houfem {

EmbeddedSimplex<2, 3> get_triangle(const GA_Detail *geo,
                                   const GA_Index   prim_index) {

  const GA_Primitive *prim = geo->getPrimitiveByIndex(prim_index);

  // Load points
  Eigen::Matrix3d points;
  for (int j = 0; j < 3; j++) {
    auto posj = prim->getPos3(j);
    for (int i = 0; i < 3; i++) {
      points(i, j) = posj[i];
    }
  }

  return EmbeddedSimplex<2, 3>(points);
}

//  _____      ___      __  __              __  __      _       _
// | _ \ \    / / |    |  \/  |__ _ ______ |  \/  |__ _| |_ _ _(_)_ __
// |  _/\ \/\/ /| |__  | |\/| / _` (_-<_-< | |\/| / _` |  _| '_| \ \ /
// |_|   \_/\_/ |____| |_|  |_\__,_/__/__/ |_|  |_\__,_|\__|_| |_/_\_\

// Standard mass matrix for piece-wise linear basis functions

Eigen::SparseMatrix<double> mass_matrix_pwl(const GA_Detail *geo) {

  using SpMat = Eigen::SparseMatrix<double>;
  using T     = Eigen::Triplet<double>;

  const int N = geo->getNumPoints();

  SpMat          M(N, N);
  std::vector<T> coefficients;
  coefficients.reserve(6 * N);

  Eigen::Matrix<double, 3, 3> unit_mass_matrix = Simplex<2>::unit_mass_matrix();

  // iterate over primitives
  const int nprimitives = geo->getNumPrimitives();
  for (int I = 0; I < nprimitives; I++) {

    const GA_Primitive *prim = geo->getPrimitiveByIndex(I);

    // global indices of primitive points
    auto id = std::array{prim->getPointIndex(0), prim->getPointIndex(1),
                         prim->getPointIndex(2)};

    const double area = prim->calcArea();

    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        coefficients.emplace_back(
            T{(int)id[i], (int)id[j], 2 * area * unit_mass_matrix(i, j)});
      }
    }
  }

  M.setFromTriplets(coefficients.begin(), coefficients.end());
  M.makeCompressed();

  return M;
}

//  _____      _____   __  __              __  __      _       _
// | _ \ \    / / __| |  \/  |__ _ ______ |  \/  |__ _| |_ _ _(_)_ __
// |  _/\ \/\/ / (__  | |\/| / _` (_-<_-< | |\/| / _` |  _| '_| \ \ /
// |_|   \_/\_/ \___| |_|  |_\__,_/__/__/ |_|  |_\__,_|\__|_| |_/_\_\

// Mass matrix for piece-wise constant matrix, i.e. it is just a diagonal matrix
// with triangle areas on the diagonal

Eigen::DiagonalMatrix<double, Eigen::Dynamic>
mass_matrix_pwc(const GA_Detail *geo) {

  const int N = geo->getNumPrimitives();

  Eigen::VectorXd areas(N);

  for (int I = 0; I < N; I++) {

    const GA_Primitive *prim = geo->getPrimitiveByIndex(I);

    areas(I) = prim->calcArea();
  }

  return areas.asDiagonal();
}

//  _____      ___    ___   __  __              __  __      _       _
// | _ \ \    / / |  / __| |  \/  |__ _ ______ |  \/  |__ _| |_ _ _(_)_ __
// |  _/\ \/\/ /| |_| (__  | |\/| / _` (_-<_-< | |\/| / _` |  _| '_| \ \ /
// |_|   \_/\_/ |____\___| |_|  |_\__,_/__/__/ |_|  |_\__,_|\__|_| |_/_\_\

Eigen::SparseMatrix<double> mass_matrix_pwlc(const GA_Detail *geo) {

  using SpMat = Eigen::SparseMatrix<double>;
  using T     = Eigen::Triplet<double>;

  const int npoints     = geo->getNumPoints();
  const int nprimitives = geo->getNumPrimitives();

  SpMat          M(npoints, nprimitives);
  std::vector<T> coefficients;
  coefficients.reserve(3 * nprimitives);

  // iterate over primitives
  for (int I = 0; I < nprimitives; I++) {

    const GA_Primitive *prim = geo->getPrimitiveByIndex(I);

    // global indices of primitive points
    auto id = std::array{prim->getPointIndex(0), prim->getPointIndex(1),
                         prim->getPointIndex(2)};

    const double area = prim->calcArea();

    for (int i = 0; i < 3; i++) {
      coefficients.emplace_back(T{(int)id[i], I, (1.0 / 3.0) * area});
    }
  }

  M.setFromTriplets(coefficients.begin(), coefficients.end());
  M.makeCompressed();

  return M;
}

//  ___ _   _  __  __                  __  __      _       _
// / __| |_(_)/ _|/ _|_ _  ___ ______ |  \/  |__ _| |_ _ _(_)_ __
// \__ \  _| |  _|  _| ' \/ -_|_-<_-< | |\/| / _` |  _| '_| \ \ /
// |___/\__|_|_| |_| |_||_\___/__/__/ |_|  |_\__,_|\__|_| |_/_\_\

Eigen::SparseMatrix<double> stiffness_matrix(const GA_Detail *geo) {

  using SpMat = Eigen::SparseMatrix<double>;
  using T     = Eigen::Triplet<double>;

  const int N = geo->getNumPoints();

  SpMat          S(N, N);
  std::vector<T> coefficients;
  coefficients.reserve(6 * N);

  // iterate over primitives
  const int nprimitives = geo->getNumPrimitives();
  for (int I = 0; I < nprimitives; I++) {

    // global indices of primitive points
    const GA_Primitive *prim = geo->getPrimitiveByIndex(I);
    auto id = std::array{prim->getPointIndex(0), prim->getPointIndex(1),
                         prim->getPointIndex(2)};

    // embeded triangle in 3-space
    const auto  embedded_triangle = get_triangle(geo, I);
    const auto &triangle          = embedded_triangle.local_simplex;

    auto localS = triangle.stiffness_matrix();

    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        coefficients.emplace_back(T{(int)id[i], (int)id[j], localS(i, j)});
      }
    }
  }

  S.setFromTriplets(coefficients.begin(), coefficients.end());
  S.makeCompressed();

  return S;
}

//   ___             _ _         _     __  __      _       _
//  / __|_ _ __ _ __| (_)___ _ _| |_  |  \/  |__ _| |_ _ _(_)__ ___ ___
// | (_ | '_/ _` / _` | / -_) ' \  _| | |\/| / _` |  _| '_| / _/ -_|_-<
//  \___|_| \__,_\__,_|_\___|_||_\__| |_|  |_\__,_|\__|_| |_\__\___/__/

std::array<Eigen::SparseMatrix<double, Eigen::RowMajor>, 3>
gradient_matrices(const GA_Detail *geo) {

  const int N = geo->getNumPrimitives();
  const int M = geo->getNumPoints();

  std::array<Eigen::SparseMatrix<double, Eigen::RowMajor>, 3> D;
  D[0].resize(N, M);
  D[1].resize(N, M);
  D[2].resize(N, M);

  D[0].reserve(Eigen::VectorXi::Constant(N, 3));
  D[1].reserve(Eigen::VectorXi::Constant(N, 3));
  D[2].reserve(Eigen::VectorXi::Constant(N, 3));

  for (int I = 0; I < N; I++) {

    // global indices of primitive points
    const GA_Primitive *prim = geo->getPrimitiveByIndex(I);
    auto id = std::array{prim->getPointIndex(0), prim->getPointIndex(1),
                         prim->getPointIndex(2)};

    // embeded triangle in 3-space
    const auto  embedded_triangle = get_triangle(geo, I);
    const auto &triangle          = embedded_triangle.local_simplex;

    for (int j = 0; j < 3; j++) {

      auto phi = triangle.barycentric_coordinate(j) |
                 embedded_triangle.global_to_local;
      auto grad = phi.derivative().value;

      D[0].insert(I, id[j]) = grad(0);
      D[1].insert(I, id[j]) = grad(1);
      D[2].insert(I, id[j]) = grad(2);
    }
  }

  return D;
}

Eigen::SparseMatrix<double> prim_to_point(const GA_Detail *geo) {

  const int N = geo->getNumPoints();
  const int M = geo->getNumPrimitives();

  Eigen::SparseMatrix<double> A(N, M);
  A.reserve(Eigen::VectorXi::Constant(M, 3));

  const int nprimitives = geo->getNumPrimitives();
  for (int I = 0; I < nprimitives; I++) {

    // global indices of primitive points
    const GA_Primitive *prim = geo->getPrimitiveByIndex(I);
    auto id = std::array{prim->getPointIndex(0), prim->getPointIndex(1),
                         prim->getPointIndex(2)};

    A.insert(id[0], I) = 1.0;
    A.insert(id[1], I) = 1.0;
    A.insert(id[2], I) = 1.0;
  }

  return A;
}
} // namespace houfem
