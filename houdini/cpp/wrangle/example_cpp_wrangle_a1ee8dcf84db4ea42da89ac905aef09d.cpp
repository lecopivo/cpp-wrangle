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
  
  for(int i=0; i<npoints(); i++){
     float condition = (P(i).x() > 0) ? 1.0 : 0.0; 
     Cd.set(i, {condition, 0.0, 0.0});     
  }

  // </wrangle>
}