#include <iostream>
#include <random>

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
  auto Cd = point<vector3>("Cd");
  
  std::default_random_engine generator;
  std::uniform_real_distribution<float> distribution(0.0,1.0);
  auto random = [&](){ return distribution(generator); };
  
  for(int i=0;i<npoints();i++){
     vector3 color = {random(), random(), random()};
  
     Cd.set(i, color);
  }
  

  // </wrangle>
}