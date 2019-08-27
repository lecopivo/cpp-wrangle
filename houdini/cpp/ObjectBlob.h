#pragma once

#include <SOP/SOP_Node.h>
#include <UT/UT_MemoryCounter.h>

///////////////// Object Blob //////////////////////////////

template <typename T> class ObjectBlob : public GA_BlobData {
public:
  ObjectBlob() {}

  ObjectBlob(const char *str, T &&t) : myString(str), m_data(std::move(t)) {}
  //ObjectBlob(const char *str, T const &t) : myString(str), m_data(t) {}

  virtual ~ObjectBlob() {}

  virtual uint hash() const { return myString.hash(); }

  virtual bool isEqual(const GA_BlobData &blob) const {
    return hash() == blob.hash();
  }

  virtual int64 getMemoryUsage(bool inclusive) const {
    int64 mem = inclusive ? sizeof(*this) : 0;
    mem += myString.getMemoryUsage();
    return mem;
  }

  virtual void countMemory(UT_MemoryCounter &counter, bool inclusive) const {
    if (counter.mustCountUnshared()) {
      counter.countUnshared(getMemoryUsage(inclusive));
    }
  }

public:
  UT_String myString;
  T         m_data;
};

template <typename T> ObjectBlob<T> *new_ObjectBlob(T t){
  return new ObjectBlob<T>{"hihi", std::move(t)};
}
