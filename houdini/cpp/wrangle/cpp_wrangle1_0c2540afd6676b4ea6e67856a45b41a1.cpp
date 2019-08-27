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
  std::vector<float> vec = {1,2,3,4,5,6};
  
  set_object_attr<std::vector<float>>(geo, "vec_attr", std::move(vec));
  
  std::cout << "Saving Custom Object!" << std::endl;
  
  auto S = houfem::stiffness_matrix(geo);
  auto M = houfem::mass_matrix_pwl(geo);
  
  using SolverType = Eigen::SimplicialLLT<Eigen::SparseMatrix<double>>;
  
  auto solver_ptr = std::make_unique<SolverType>(M);
  
  set_object_attr<Eigen::SparseMatrix<double>>(geo, "stiffness_matrix", std::move(S));
  set_object_attr<std::unique_ptr<SolverType>>(geo, "solver_ptr", std::move(solver_ptr));
  
  

  // </wrangle>
}