#include <iostream>

#include <cppvex/cppvex.h>
#include <hougen/hougen.h>

#include <GU/GU_Detail.h>
#include <SOP/SOP_Node.h>

using namespace cppvex;

extern "C" void callback(const float time, SOP_Node *node, GU_Detail *geo) {

  // Set up VEX context
  set_up_cppvex_context(time, node, node, geo);
 
  //////////////////////////////////////////////
  ////////////// C++ Wrangle Code //////////////
  
  // <wrangle>
  // Load points of the first input
  auto P1 = hougen::point<float, 3>(1, "P");
  
  // Compute mean
  auto center = P1.colwise().mean();
  
  // Center data around mean
  auto X = P1.rowwise() - center;
  
  // compute the covariance matrix
  auto Cov = X.adjoint()*X/(npoints(1)-1);
  
  // Preform eigenvalue decomposition
  Eigen::SelfAdjointEigenSolver<Eigen::Matrix3f> solver(Cov);
  
  // Get eigen decomposition matrices
  auto D = solver.eigenvalues().array().sqrt().matrix().asDiagonal();
  auto V = solver.eigenvectors();
  
  // Transform points of the sphere
  auto P = hougen::point<float, 3>("P");
  auto TP = sqrt(3.0)*P*V*D*V.transpose(); // transform
  auto Q  = TP.rowwise() + center; // move to the center
  hougen::setpointattrib("P", Q);
  
  

  // </wrangle>
}