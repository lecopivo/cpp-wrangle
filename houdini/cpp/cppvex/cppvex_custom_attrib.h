#pragma once

#include <GA/GA_AIFBlob.h>
#include <GA/GA_BlobData.h>
#include <GA/GA_Detail.h>
#include <GU/GU_Detail.h>

#include "../RegisteredObjects.h"
#include "cppvex_element.h"

#include <functional>
#include <optional>
#include <variant>

namespace cppvex {

namespace internal {

GA_Attribute *ensure_custom_attribute_existence(GA_Detail *             geo,
                                                const GA_AttributeOwner owner,
                                                const char *attribute_name) {

  auto scope = GA_SCOPE_PUBLIC;

  GA_Attribute *attr = geo->findAttribute(owner, scope, attribute_name);
  if (!attr) {
    attr = geo->createAttribute(owner, scope, attribute_name, nullptr, nullptr,
                                "blob");
  }
  return attr;
}

} // namespace internal

template <typename T>
T &load_object(GA_Detail *geo, const char *attribute_name) {

  auto owner = GA_ATTRIB_DETAIL;

  GA_ROAttributeRef attr =
      internal::ensure_custom_attribute_existence(geo, owner, attribute_name);

  auto aif             = attr->getAIFBlob();
  auto blob_ptr        = aif->getBlob(attr.getAttribute(), 0);
  auto object_blob_ptr = dynamic_cast<ObjectBlob<T> *>(blob_ptr.get());

  return object_blob_ptr->m_data;
}

template <typename T>
void save_object(GA_Detail *geo, const char *attribute_name, T &&value) {

  auto owner = GA_ATTRIB_DETAIL;

  GA_RWAttributeRef attr =
      internal::ensure_custom_attribute_existence(geo, owner, attribute_name);

  auto aif         = attr->getAIFBlob();
  auto object_blob = new_ObjectBlob<std::decay_t<T>>(std::forward<T>(value));

  aif->setBlob(attr.getAttribute(), object_blob, 0);
}
} // namespace cppvex
