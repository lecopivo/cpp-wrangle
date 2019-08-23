#include <iostream>

#include <cppvex/cppvex.h>
#include <hougen/hougen.h>

#include <GA/GA_AIFBlob.h>
#include <GA/GA_AIFBlobArray.h>
#include <GU/GU_Detail.h>
#include <SOP/SOP_Node.h>

#include "StructBlob.h"

using namespace cppvex;

extern "C" void callback(const float time, SOP_Node *node, GU_Detail *geo) {

  // Set up VEX context
  set_up_cppvex_context(time, node, node, geo);

  //////////////////////////////////////////////
  ////////////// C++ Wrangle Code //////////////

  // <wrangle>

  std::vector<int> arr = {1, 2, 3, 4, 5, 6};

  auto owner          = GA_ATTRIB_DETAIL;
  auto attribute_name = "test";

  auto *attr = geo->findAttribute(owner, attribute_name);
  if (!attr) {
    attr = geo->createAttribute(owner, GA_SCOPE_PUBLIC, attribute_name, nullptr,
                                nullptr, "blobarray");
  }

  auto const &attributes = geo->getAttributes();
  for (auto it = attributes.begin(owner); it != attributes.end(owner); ++it) {
    auto const &a = *it.attrib();
    std::cout << "Attribute name:      " << a.getName() << std::endl;
    std::cout << "Attribute full name: " << a.getFullName() << std::endl;
    std::cout << "Attribute type:      " << a.getType().getTypeName()
              << std::endl;
    // std::cout <<  a.getTypeInfo()
  }

  auto       aif        = attr->getAIFBlobArray();
  GA_BlobRef blob_ptr   = new StructBlob<float>{3.1415};
  auto       blob_array = UT_Array<GA_BlobRef>{blob_ptr};
  // aif->setBlob(attr, blob_array, 0);

  std::cout << std::endl;

  // </wrangle>
}
