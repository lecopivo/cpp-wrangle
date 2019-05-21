#pragma once

#include <set>
#include <string>
#include <tuple>

#include <SOP/SOP_Node.h>

#include "cppvex_types.h"

namespace cppvex {

namespace internal {

float     time;
SOP_Node *current_node;
SOP_Node *execution_node;

std::string full_path(const char *parm) {

  if (UT_String(parm).isAbsolutePath())
    return parm;

  UT_String current_node_path;
  current_node->getPathFromParent(current_node_path, nullptr);

  return current_node_path.toStdString() + "/" + parm;
}

std::tuple<OP_Node *, std::string> parm_node_and_name(const char *parm) {
  UT_String path(full_path(parm));

  UT_String node_path, parm_name;
  path.splitPath(node_path, parm_name);

  OP_Node *node = OPgetDirector()->findNode(node_path);

  return {node, parm_name.toStdString()};
}

void register_parm(OP_Node *pnode, const char *pname) {
  int parm_idx = pnode->getParmIndex(pname);
  execution_node->addExtraInput(*pnode, parm_idx, -1);
}

} // namespace internal

float chf(const char *parm, const int component = 0) {
  auto[node, name] = internal::parm_node_and_name(parm);
  internal::register_parm(node, name.c_str());
  return node->evalFloat(name, component, internal::time);
}

int chi(const char *parm, const int component = 0) {
  auto[node, name] = internal::parm_node_and_name(parm);
  internal::register_parm(node, name.c_str());
  return node->evalInt(name, component, internal::time);
}

std::string chs(const char *parm) {
  auto[node, name] = internal::parm_node_and_name(parm);
  internal::register_parm(node, name.c_str());
  UT_String val;
  node->evalString(val, name, 0, internal::time);
  return val.toStdString();
}

vector2 chu(const char *parm) { return {chf(parm, 0), chf(parm, 1)}; }

vector3 chv(const char *parm) {
  return {chf(parm, 0), chf(parm, 1), chf(parm, 2)};
}

vector4 chp(const char *parm) {
  return {chf(parm, 0), chf(parm, 1), chf(parm, 2), chf(parm, 3)};
}

matrix2 ch2(const char *parm) {
  matrix2   out;
  const int dim = 2;
  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++) {
      int idx   = dim * i + j;
      out(i, j) = chf(parm, idx);
    }
  }
  return out;
}

matrix3 ch3(const char *parm) {
  matrix3   out;
  const int dim = 3;
  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++) {
      int idx   = dim * i + j;
      out(i, j) = chf(parm, idx);
    }
  }
  return out;
}

matrix4 ch4(const char *parm) {
  matrix4   out;
  const int dim = 4;
  for (int i = 0; i < dim; i++) {
    for (int j = 0; j < dim; j++) {
      int idx   = dim * i + j;
      out(i, j) = chf(parm, idx);
    }
  }
  return out;
}

} // namespace cppvex
