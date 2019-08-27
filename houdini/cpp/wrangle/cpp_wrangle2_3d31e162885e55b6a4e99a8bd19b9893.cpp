#include <iostream>

#include <cppvex/cppvex.h>
#include <houfem/fem.h>
#include <hougen/hougen.h>

#include <GA/GA_AttributeRef.h>
#include <GU/GU_Detail.h>
#include <SOP/SOP_Node.h>

#include "RegisteredObjects.h"
using namespace cppvex;

extern "C" void callback(const float time, SOP_Node *node, GU_Detail *geo) {

  // Set up VEX context
  set_up_cppvex_context(time, node, node, geo);

  //////////////////////////////////////////////
  ////////////// C++ Wrangle Code //////////////

  // <wrangle>
  std::cout << "Hello World!" << std::endl;

  auto const &vec = get_object_attr<std::vector<float>>(geo, "vec_attr");

  auto const &S =
      get_object_attr<Eigen::SparseMatrix<double>>(geo, "stiffness_matrix");
  auto const &solver = *get_object_attr<
      std::unique_ptr<Eigen::SimplicialLLT<Eigen::SparseMatrix<double>>>>(
      geo, "solver_ptr");

  auto P = hougen::point<double, 3>("P");
  P      = solver.solve(P);
  hougen::setpointattrib("P", P);

  std::cout << "Recovered data: ";

  for (auto const &v : vec)
    std::cout << v << " ";
  std::cout << std::endl;

  std::cout << "Recovered Stiffness Matrix: \n" << S << std::endl << std::endl;

  // </wrangle>
}
