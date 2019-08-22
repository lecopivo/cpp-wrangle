#pragma once

#include <GA/GA_AIFBlob.h>
#include <GA/GA_BlobData.h>
#include <GA/GA_Detail.h>
#include <GU/GU_Detail.h>

#include "cppvex_element.h"

#include <any>
#include <functional>
#include <optional>

namespace cppvex {

namespace internal {

GA_Attribute *ensure_blob_attribute_existence(GA_Detail *             geo,
                                              const GA_AttributeOwner owner,
                                              const char *attribute_name);

//    _            ___ _     _
//   /_\  _ _ _  _| _ ) |___| |__
//  / _ \| ' \ || | _ \ / _ \ '_ \
// /_/ \_\_||_\_, |___/_\___/_.__/
//            |__/

class AnyBlob : public GA_BlobData {
public:
  AnyBlob();
  AnyBlob(std::any data, const uint hash);
  virtual ~AnyBlob();

  virtual uint hash() const;
  virtual bool isEqual(const GA_BlobData &blob) const;

  virtual int64 getMemoryUsage(bool inclusive) const;
  virtual void  countMemory(UT_MemoryCounter &counter, bool inclusive) const;

public:
  std::any   m_data;
  const uint m_hash;
};
} // namespace internal

template <typename T>
//std::optional<T const &>
T const&
getblobattrib(GA_Detail *geo, const GA_AttributeOwner owner,
              const char *attribute_name, const GA_Index index) {

  auto attr =
      internal::ensure_blob_attribute_existence(geo, owner, attribute_name);
  auto aif = attr->getAIFBlob();

  const GA_Offset offset = elementOffset(geo, owner, index);

  auto any_blob_ptr = aif->getBlob(attr, offset);
  if (any_blob_ptr) {

    internal::AnyBlob &any_blob =
        *dynamic_cast<internal::AnyBlob *>(any_blob_ptr.get());

    if (any_blob.m_data.has_value()) {
      return std::any_cast<T const &>(any_blob);
    }
  }
  return T{};
}

template <typename T>
void setblobattrib(GA_Detail *geo, const GA_AttributeOwner owner,
                   const char *attribute_name, const GA_Index index, T&& value) {
  auto attr =
      internal::ensure_blob_attribute_existence(geo, owner, attribute_name);
  auto aif = attr->getAIFBlob();

  const uint hash         = 666;//std::hash<T>()(value);
  GA_BlobRef any_blob_ptr = new internal::AnyBlob{std::forward<T>(value), hash};

  const GA_Offset offset = elementOffset(geo, owner, index);

  aif->setBlob(attr, any_blob_ptr, offset);
}
} // namespace cppvex
