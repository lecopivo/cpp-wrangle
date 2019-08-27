#include <iostream>

#include <cppvex/cppvex.h>
#include <hougen/hougen.h>

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
  
  

  // </wrangle>
}