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
  auto P     = hougen::point<float, 3>("P");
  auto shift = hougen::point<float, 3>("shift");
  auto color = hougen::prim<float, 3>("Cd");
  
  shift = 0.01 * Eigen::MatrixXf::Random(npoints(), 3);
  color = Eigen::MatrixXf::Random(nprimitives(), 3);
  
  hougen::setpointattrib("P", P + shift);
  hougen::setpointattrib("shift", shift);
  hougen::setprimattrib("Cd", color);
  
  

  // </wrangle>
}