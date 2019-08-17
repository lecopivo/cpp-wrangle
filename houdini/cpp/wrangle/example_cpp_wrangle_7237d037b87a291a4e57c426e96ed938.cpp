#include <iostream>

#include <cppvex/cppvex.h>
#include <hougen/hougen.h>
#include <houfem/houfem.h>

#include <GU/GU_Detail.h>
#include <SOP/SOP_Node.h>

using namespace cppvex;

extern "C" void callback(const float time, SOP_Node *node, GU_Detail *geo) {

  // Set up VEX context
  set_up_cppvex_context(time, node, node, geo);
 
  //////////////////////////////////////////////
  ////////////// C++ Wrangle Code //////////////
  
  // <wrangle>
   // Generate FE matrices
  auto M = houfem::mass_matrix_pwl(geo);
  auto CM = houfem::mass_matrix_pwc(geo);
  auto S = houfem::stiffness_matrix(geo);
  auto [DX, DY, DZ] = houfem::gradient_matrices(geo);
  
  // Get projection from full dof space to free dofs
  auto [Fixed, Free] = hougen::group_projection(geo, GA_ATTRIB_POINT, "distance_from");
  const int num_fixed_points = Fixed.rows();
  
  ////////////////////////////////////////////
  ///////// STEP 1 - HEAT FLOW ///////////////
  // Heat flow initial state - Fixed points are initialized to one
  Eigen::VectorXd heat_initial_state(npoints());
  heat_initial_state =  Fixed.transpose() * Eigen::MatrixXd::Constant(num_fixed_points, 1, 1.0);
  
  // Amount of heat flow is proportional to the h^2 /home/tskrivan area of triagles(we are being conservative and take the biggest one)
  const double amount = CM.diagonal().maxCoeff();
  
  // Set up a linear system
  auto A = M + amount * S;
  auto B = M;
  Eigen::SimplicialLDLT<Eigen::SparseMatrix<double>> heat_solver(A);
  
  // Compute the heat flow
  auto heat = heat_solver.solve(M*heat_initial_state).eval();
  
  // save heat
  hougen::setpointattrib("heat", heat);
  
  ////////////////////////////////////////////
  ///////// STEP 2 - COMPUTE X ///////////////
  Eigen::Matrix<double, Eigen::Dynamic, 3> X(nprimitives(), 3);
  
  // compute gradient of the heat
  X.col(0) = DX*heat;
  X.col(1) = DY*heat;
  X.col(2) = DZ*heat;
  
  // normalize the heat gradient
  X.rowwise().normalize();
  
  hougen::setprimattrib("X", X);
  
  ///////////////////////////////////////////////////
  ///////// STEP 3 - COMPUTE DISTANCE ///////////////
  auto divX = - (DX.transpose()*CM*X.col(0) + DY.transpose()*CM*X.col(1) + DZ.transpose()*CM*X.col(2));
  
  // The stiffness matrix S is sandwitched by Free, this removes all fixed points from the system
  Eigen::SimplicialLDLT<Eigen::SparseMatrix<double>> dist_solver(Free*S*Free.transpose());
  
  auto dist = (Free.transpose() * dist_solver.solve(Free*divX)).eval();
  
  hougen::setpointattrib("dist", dist);

  // </wrangle>
}