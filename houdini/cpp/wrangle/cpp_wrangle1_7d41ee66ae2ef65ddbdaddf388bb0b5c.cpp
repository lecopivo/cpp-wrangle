#include <iostream>

#include <cppvex/cppvex.h>
#include <hougen/hougen.h>
#include <houfem/fem.h>

#include <GU/GU_Detail.h>
#include <SOP/SOP_Node.h>

#include <RegisteredObjects.h>

using namespace cppvex;

extern "C" void callback(const float time, SOP_Node *node, GU_Detail *geo) {

  // Set up VEX context
  set_up_cppvex_context(time, node, node, geo);
 
  //////////////////////////////////////////////
  ////////////// C++ Wrangle Code //////////////
  
  // <wrangle>
  std::cout << "Computing Laplace Operator!" << std::endl;
  
  auto M = houfem::mass_matrix_pwl(geo);
  auto S = houfem::stiffness_matrix(geo);
  
  auto [Fixed, Free] = hougen::group_projection(geo, GA_ATTRIB_POINT, "fixed");
  
  using SolverType = Eigen::SimplicialLLT<Eigen::SparseMatrix<double>>;
  
  auto solver_ptr = std::make_unique<SolverType>(Free*S*Free.transpose());
  
  auto owner          = GA_ATTRIB_DETAIL;
  auto scope          = GA_SCOPE_PUBLIC;
  auto attribute_name = "laplace_solver";
  
  GA_RWAttributeRef blob_gah = geo->createAttribute(
      owner, scope, attribute_name, nullptr, nullptr, "blob");
  
  // create blob
  GA_Attribute *    attr     = blob_gah.getAttribute();
  const GA_AIFBlob *aif      = attr->getAIFBlob();
  
  //GA_BlobRef        blob_ptr = new StructBlob<float>{"StructBlob data!", 3.1415f};
  //GA_BlobRef        blob_ptr = new_StructBlob<float>(3.1415f);
  GA_BlobRef        blob_ptr = new_ObjectBlob<std::unique_ptr<SolverType>>(std::move(solver_ptr));
  
  aif->setBlob(attr, blob_ptr, 0);

  // </wrangle>
}
