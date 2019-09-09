#include <iostream>

#include <cppvex/cppvex.h>
#include <hougen/hougen.h>
#include <houfem/fem.h>

#include <SIM/SIM_Engine.h>
#include <SIM/SIM_Geometry.h>
#include <SIM/SIM_Object.h>
#include <SIM/SIM_ObjectArray.h>
#include <SIMZ/SIM_SopGeometry.h>

#include <RegisteredObjects.h>

using namespace cppvex;
extern "C" void dop_callback(SIM_Engine &engine, SIM_Object &object,
                             SIM_ObjectArray &objArray,
                             const SIM_Time &timestep, bool newobject) {

  //////////////////////////////////////////////
  ////////////// C++ Wrangle Code //////////////
  
  // <wrangle>
  auto geo = static_cast<SIM_SopGeometry *>(object.getNamedSubData("Geometry"))
                 ->getOwnGeometry()
                 .gdpNC();
  
  std::cout << "Diffusing Positions!" << std::endl;
  
  using SolverType = Eigen::SimplicialLLT<Eigen::SparseMatrix<double>>;
  
  auto const &attrs = geo->getAttributes();
  
  for (auto it = attrs.begin(GA_ATTRIB_DETAIL);
       it != attrs.end(GA_ATTRIB_DETAIL); ++it) {
  
    std::cout << "Attribute: " << it.name() << std::endl;
  }
  
  auto const &solver_ptr =
      load_object<std::unique_ptr<SolverType>>(geo, "diffusion_operator");
  
  auto const &solver = *solver_ptr;
  
  auto P = hougen::point<double, 3>(geo, "P");
  P      = solver.solve(P);
  hougen::setpointattrib(geo, "P", P);
  
  
  
  

  // </wrangle>
}