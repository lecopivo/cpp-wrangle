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
  auto P  = point<vector3>("P");
  auto Cd = point<vector3>("Cd");
  
  for (int i = 0; i < npoints(); i++) {
    const char *group     = "mygroup"; // group name to add points to
    bool        condition = (P(i).x() > 0) ? true : false; // short form if() test
    
    setpointgroup(group, i, condition);
    Cd.set(i, {1.0f * condition, 0.0, 0.0}); // color if in group
  }
  

  // </wrangle>
}