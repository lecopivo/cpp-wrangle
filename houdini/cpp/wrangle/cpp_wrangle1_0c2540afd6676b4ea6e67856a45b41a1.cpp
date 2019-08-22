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

  std::vector<int> arr = {1, 2, 3, 4, 5, 6};

  std::cout << "Saving vector: ";
  for (auto const &a : arr)
    std::cout << a << ",";
  std::cout << std::endl;

  auto attr = internal::ensure_blob_attribute_existence(geo, GA_ATTRIB_POINT,
                                                        "test_any");
  auto aif  = attr->getAIFBlob();

  std::cout << "Attribute name: " << attr->getName() << std::endl;
  std::cout << "Attribute full name: " << attr->getFullName() << std::endl;
  std::cout << "Attribute type: " << attr->getType().getTypeName() << std::endl;

  std::any   arr_any = arr;
  const uint hash    = 666; // std::hash<T>()(value);

  GA_BlobRef blob_ptr     = new internal::AnyBlob{arr_any, hash};
  auto       any_blob_ptr = dynamic_cast<internal::AnyBlob *>(blob_ptr.get());
  auto       brr_any      = any_blob_ptr->m_data;

  std::cout << "Type Name in std::any: " << brr_any.type().name() << std::endl;

  auto brr = std::any_cast<std::vector<int>>(brr_any);

  std::cout << "Loading vector: ";
  for (auto const &a : brr)
    std::cout << a << ",";
  std::cout << std::endl;

  std::cout << "hoho" << std::endl;

  // aif->setBlob(attr, new internal::AnyBlob{std::any{}, hash}, 0);

  std::cout << "Attr tuple size: " << aif->getTupleSize(attr) << std::endl;
  aif->setTupleSize(attr, 2);
  // aif->arrayAppendBlob(attr, blob_ptr);
  std::cout << "Attr tuple size: " << aif->getTupleSize(attr) << std::endl;
  aif->getTupleSize(attr);
  //aif->setBlob(attr, blob_ptr, 0);

  std::cout << std::endl;

  // </wrangle>
}
