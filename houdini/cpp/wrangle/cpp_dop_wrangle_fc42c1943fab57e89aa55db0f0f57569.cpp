#include <iostream>

#include <cppvex/cppvex.h>
#include <hougen/hougen.h>

#include <SIM/SIM_Engine.h>
#include <SIM/SIM_Geometry.h>
#include <SIM/SIM_Object.h>
#include <SIM/SIM_ObjectArray.h>
#include <SIMZ/SIM_SopGeometry.h>

extern "C" void dop_callback(SIM_Engine &engine, SIM_Object &object,
                             SIM_ObjectArray &objArray,
                             const SIM_Time &timestep, bool newobject) {

  //////////////////////////////////////////////
  ////////////// C++ Wrangle Code //////////////
  
  // <wrangle>
  std::cout << "Hello World!" << std::endl;
  
  auto geo = static_cast<SIM_SopGeometry *>(object.getNamedSubData("Geometry"))
                 ->getOwnGeometry()
                 .gdpNC();
  
  const int npoints = geo->getNumPoints();
  for (int i = 0; i < npoints; i++) {
    using namespace cppvex;
    auto p = point<vector3>(geo, "P", i);
    p.y() += 0.01;
  
    std::cout << "Point " << i << " " << p << std::endl;
  
    setpointattrib(geo, "P", i, p);
  }
  
  std::cout << std::endl;
  
  

  // </wrangle>
}