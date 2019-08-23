#pragma once

#include <GA/GA_BlobData.h>
#include <UT/UT_MemoryCounter.h>

#include <functional>
#include <iostream>

template <typename T> class StructBlob : public GA_BlobData {
public:
  StructBlob() {
  std::cout << "Calling `StructBlob()`" << std::endl;}
  StructBlob(T const &t) : m_data(t){
  std::cout << "Calling `StructBlob(T const& t)`" << std::endl;
  };
  virtual ~StructBlob() { std::cout << "Calling `~StructBlob`" << std::endl; }

  virtual uint hash() const {
    std::cout << "Calling `hash`" << std::endl;
    return std::hash<T>()(m_data);
  }

  virtual bool isEqual(const GA_BlobData &blob) const {
    std::cout << "Calling `isEqual`" << std::endl;
    return blob.hash() == hash();
  }

  virtual int64 getMemoryUsage(bool inclusive) const {
    std::cout << "Calling `getMemoryUsage`" << std::endl;
    return inclusive ? sizeof(*this) : 0;
    //return sizeof(*this);
  }
  virtual void countMemory(UT_MemoryCounter &counter, bool inclusive) const {
    std::cout << "Calling `countMemory`" << std::endl;
    if (counter.mustCountUnshared()) {
      counter.countUnshared(inclusive ? sizeof(*this) : 0);
    }
    if (counter.mustCountShared()) {
      counter.countShared(0, 0, nullptr);
    }
  }

public:
  T m_data;
};
