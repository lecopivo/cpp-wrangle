#include <iostream>

#include <cppvex/cppvex.h>
#include <hougen/hougen.h>

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
  auto owner          = GA_ATTRIB_DETAIL;
  auto scope          = GA_SCOPE_PUBLIC;
  auto attribute_name = "laplace_solver";

  GA_RWAttributeRef blob_gah = geo->findAttribute(owner, scope, attribute_name);

  // create blob
  GA_Attribute *    attr = blob_gah.getAttribute();
  const GA_AIFBlob *aif  = attr->getAIFBlob();

  GA_BlobRef blob = aif->getBlob(attr, 0);

  using SolverType = Eigen::SimplicialLLT<Eigen::SparseMatrix<double>>;

  auto object_blob = dynamic_cast<ObjectBlob<std::unique_ptr<SolverType>> *>(blob.get());
  
  auto const& solver = *(object_blob->m_data);
  
  //auto P = 

  // </wrangle>
}
