#include <iostream>

#include <cppvex/cppvex.h>
#include <hougen/hougen.h>
#include <houfem/fem.h>

#include <GU/GU_Detail.h>
#include <SOP/SOP_Node.h>
#include <GA/GA_AttributeRef.h>

#include "RegisteredObjects.h"
using namespace cppvex;

extern "C" void callback(const float time, SOP_Node *node, GU_Detail *geo) {

  // Set up VEX context
  set_up_cppvex_context(time, node, node, geo);
 
  //////////////////////////////////////////////
  ////////////// C++ Wrangle Code //////////////
  
  // <wrangle>
  std::cout << "Computing Laplace Operatorrr!" << std::endl;
  
  Eigen::SparseMatrix<double> I(npoints(), npoints());
  I.setIdentity();
  auto S = houfem::stiffness_matrix(geo);
  auto M = houfem::mass_matrix_pwl(geo);
  auto [Fixed, Free] = hougen::group_projection(geo, GA_ATTRIB_POINT, "fixed");
  
  // Eigen::SparseMatrix<double> Fr = Free;
  //auto F = (Free.transpose()*Free).eval();
  
  double diffusion_rate = chf("../Settings/diffusion_rate");
  
  using SolverType = Eigen::SimplicialLLT<Eigen::SparseMatrix<double>>;
  
  auto solver_ptr = std::make_unique<SolverType>(Free*(M+diffusion_rate*S)*Free.transpose());
  
  auto P = hougen::point<double,3>("P");
  P = Free.transpose()*solver_ptr->solve(Free*M*Free.transpose()*Free*P - diffusion_rate*Free*S*Fixed.transpose()*Fixed*P) + Fixed.transpose()*Fixed*P;
  hougen::setpointattrib("P", P);
  
  save_object<std::unique_ptr<SolverType>>(geo, "diffusion_operator", std::move(solver_ptr));
  
  
  
  
  

  // </wrangle>
}