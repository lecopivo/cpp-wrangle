#include <iostream>
#include <complex>

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
  auto P = point<vector3>("P");
  
  float p = chi("../Settings/p");
  float r = 2 * sqrt(2 * p - 1) / (p - 1);
  
  auto power = [](auto x, int a) {
    auto result = x;
    for (int i = 1; i < a; i++)
      result *= x;
    return result;
  };
  
  for (int i = 0; i < npoints(); i++) {
  
    vector3 pt = P(i);
    auto z = std::complex<float>{pt.x() / (1 - pt.z()), pt.y() / (1 - pt.z())};
    auto I = std::complex<float>{0, 1};
  
    auto zp     = pow(z, p);
    auto z2p    = zp * zp;
    auto factor = I / (z2p + r * zp - 1.0f);
    auto phi    = vector3{(factor * (z2p / z - z)).real(),
                       (factor * (-I * (z2p / z + z))).real(),
                       (factor * ((p - 1.0f) / p * (z2p + 1.0f))).real()};
  
    P.set(i, phi);
  } 

  // </wrangle>
}