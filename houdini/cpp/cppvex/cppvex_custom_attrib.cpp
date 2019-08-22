#include "cppvex_custom_attrib.h"

#include <iostream>

#include <UT/UT_MemoryCounter.h>

namespace cppvex {

namespace internal {

GA_Attribute *ensure_blob_attribute_existence(GA_Detail *             geo,
                                              const GA_AttributeOwner owner,
                                              const char *attribute_name) {
  GA_Attribute *attr = geo->findAttribute(owner, attribute_name);
  if (!attr) {
    attr = geo->createAttribute(owner, GA_SCOPE_PRIVATE, attribute_name,
                                nullptr, nullptr, "blob");
  }
  return attr;
}

AnyBlob::AnyBlob()
    : m_data()
    , m_hash(0) {}

AnyBlob::AnyBlob(std::any data, const uint hash)
    : m_data(data)
    , m_hash(hash) {}

AnyBlob::~AnyBlob() {}

uint AnyBlob::hash() const { return m_hash; }

bool AnyBlob::isEqual(const GA_BlobData &blob) const {
  return hash() == blob.hash();
}

int64 AnyBlob::getMemoryUsage(bool inclusive) const {

  std::cout << "Getting memory!" << std::endl;

  int64 mem = inclusive ? sizeof(*this) : 0;
  return mem;
}

void AnyBlob::countMemory(UT_MemoryCounter &counter, bool inclusive) const {

  std::cout << "Counting memory!" << std::endl;
  std::cout << "count unshared: " << counter.mustCountUnshared() << std::endl;
  std::cout << "count shared: " << counter.mustCountShared() << std::endl;

  if(counter.mustCountUnshared()){
    counter.countUnshared(inclusive ? sizeof(*this) : 0);
  }
}

} // namespace internal
} // namespace cppvex
