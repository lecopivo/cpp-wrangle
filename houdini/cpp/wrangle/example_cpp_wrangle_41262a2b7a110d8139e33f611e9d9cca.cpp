#include <iostream>

#include <cppvex/cppvex.h>
#include <hougen/hougen.h>
#include <houfem/houfem.h>

#include <GU/GU_Detail.h>
#include <SOP/SOP_Node.h>

using namespace cppvex;

extern "C" void callback(const float time, SOP_Node *node, GU_Detail *geo) {

  // Set up VEX context
  set_up_cppvex_context(time, node, node, geo);
 
  //////////////////////////////////////////////
  ////////////// C++ Wrangle Code //////////////
  
  // <wrangle>
  const auto M            = houfem::mass_matrix_pwl(output_geometry());
  const auto LC           = houfem::mass_matrix_pwlc(output_geometry());
  const auto [DX, DY, DZ] = houfem::gradient_matrices(geo);
  
  const Eigen::SimplicialLLT<Eigen::SparseMatrix<double>> solver(M);
  
  // Create three canonical vector fields as operators
  auto X = [&](auto const &x) { return solver.solve(LC * DX * x).eval(); };
  auto Y = [&](auto const &x) { return solver.solve(LC * DY * x).eval(); };
  auto Z = [&](auto const &x) { return solver.solve(LC * DZ * x).eval(); };
  
  // Load position and normal data
  const auto  P = hougen::point<double, 3>("P");
  auto const &x = P.col(0);
  auto const &y = P.col(1);
  auto const &z = P.col(2);
  
  const auto  N  = hougen::point<double, 3>("N");
  auto const &nx = N.col(0);
  auto const &ny = N.col(1);
  auto const &nz = N.col(2);
  
  auto EX = hougen::point<double, 3>("EX");
  auto EY = hougen::point<double, 3>("EY");
  auto EZ = hougen::point<double, 3>("EZ");
  
  EX.col(0) = X(x);
  EX.col(1) = Y(x);
  EX.col(2) = Z(x);
  
  EY.col(0) = X(y);
  EY.col(1) = Y(y);
  EY.col(2) = Z(y);
  
  EZ.col(0) = X(z);
  EZ.col(1) = Y(z);
  EZ.col(2) = Z(z);
  
  hougen::setpointattrib("EX", EX);
  hougen::setpointattrib("EY", EY);
  hougen::setpointattrib("EZ", EZ);
  
  Eigen::Matrix<double, Eigen::Dynamic, 9> G;
  G.resize(npoints(), 9);
  G << X(x), X(y), X(z), Y(x), Y(y), Y(z), Z(x), Z(y), Z(z);
  
  Eigen::Matrix<double, Eigen::Dynamic, 9> J;
  J.resize(npoints(), 9);
  J << X(nx), X(ny), X(nz), Y(nx), Y(ny), Y(nz), Z(nx), Z(ny), Z(nz);
  
  auto K = point<float>("K");
  auto H = point<float>("H");
  
  for (int i = 0; i < npoints(); i++) {
    // Shape operator
    Eigen::Matrix3d S;
    S << J.block<1, 3>(i, 0), J.block<1, 3>(i, 3), J.block<1, 3>(i, 6);
  
    K.set(i, (N.row(i).transpose() * N.row(i) + S).determinant());
    H.set(i, 0.5 * S.trace());
  }

  // </wrangle>
}