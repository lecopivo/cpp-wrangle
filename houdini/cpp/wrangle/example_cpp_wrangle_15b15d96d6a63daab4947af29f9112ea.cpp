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
  auto M = houfem::mass_matrix_pwl(output_geometry());
  auto S = houfem::stiffness_matrix(output_geometry());
  
  auto [Fixed, Free] = hougen::group_projection(geo, GA_ATTRIB_POINT, "fixed");
  
  Eigen::SimplicialLLT<Eigen::SparseMatrix<double>> laplace_solver(Free*S*Free.transpose());
  
  auto P = hougen::point<double, 3>("Q");
  
  auto rhs = - Free*S*Fixed.transpose()*Fixed*P;
  auto Q = (Free.transpose() * laplace_solver.solve(rhs)) + Fixed.transpose()*Fixed*P;
  
  hougen::setpointattrib("P", Q);
  
  
  

  // </wrangle>
}