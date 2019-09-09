#include <iostream>

#include <cppvex/cppvex.h>
#include <hougen/hougen.h>
#include <houfem/fem.h>

#include <GU/GU_Detail.h>
#include <SOP/SOP_Node.h>

#include <RegisteredObjects.h>

using namespace cppvex;

extern "C" void callback(const float time, SOP_Node *node, GU_Detail *geo) {

  // Set up VEX context
  set_up_cppvex_context(time, node, node, geo);
 
  //////////////////////////////////////////////
  ////////////// C++ Wrangle Code //////////////
  
  // <wrangle>
  std::cout << "Computing Laplace Operator!" << std::endl;
  
  Eigen::SparseMatrix<double> I(npoints(), npoints());
  I.setIdentity();
  auto S = houfem::stiffness_matrix(geo);
  auto M = houfem::mass_matrix_pwl(geo);
  auto [Fixed, Free] = hougen::group_projection(geo, GA_ATTRIB_POINT, "fixed");
  
  auto P = (Free.transpose()*Free).eval();
  
  double diffusion_rate = chf("../Settings/diffusion_rate");
  
  using SolverType = Eigen::SimplicialLLT<Eigen::SparseMatrix<double>>;
  
  Eigen::SparseMatrix<double> A = (M+diffusion_rate*S).eval();
  
  auto solver_ptr = std::make_unique<SolverType>(A);
  
  save_object<std::unique_ptr<SolverType>>(geo, "diffusion_operator", std::move(solver_ptr));
  
  

  // </wrangle>
}