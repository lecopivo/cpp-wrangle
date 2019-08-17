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
  auto CM = houfem::mass_matrix_pwc(output_geometry());
  auto S = houfem::stiffness_matrix(output_geometry());
  auto [DX, DY, DZ] = houfem::gradient_matrices(output_geometry());
  
  // This should yield stifness matrix!
  auto SS = (DX.transpose()*CM*DX + DY.transpose()*CM*DY + DZ.transpose()*CM*DZ).eval();
  
  auto P = hougen::point<double, 3>("P");
  Eigen::Matrix<double, Eigen::Dynamic, 3> dPy(nprimitives(),3);
  
  dPy.col(0) = DX*P.col(1);
  dPy.col(1) = DY*P.col(1);
  dPy.col(2) = DZ*P.col(1);
  
  hougen::setprimattrib("dPy", dPy);
  
  std::cout << "Mass Matrix:\n" << M << "\n\n\n";
  std::cout << "Stiffness Matrix:\n" << S << "\n\n\n";
  std::cout << "Stiffness Matrix computed with single derivatives:\n" << SS << "\n\n\n";
  std::cout << "X Derivative Matrix:\n" << DX << "\n\n\n";
  

  // </wrangle>
}