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
  
  vector3 direction   = chv("../Settings/direction");
  float   amount      = chf("../Settings/amount");
  float   frequency   = chf("../Settings/frequency");
  int     wave_number = chi("../Settings/wave_number");
  vector3 color       = chv("../Settings/color");
  
  for (int i = 0; i < npoints(); i++) {
    vector3 p     = relbbox(0, P(i));
    float   phase = 2 * M_PI * p.y() * wave_number + frequency * time;
    P.add(i, amount * direction * sin(phase));
    Cd.set(i, color * sin(phase));
  }
  
  

  // </wrangle>
}