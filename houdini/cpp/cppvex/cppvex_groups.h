#pragma once

#include "cppvex_context.h"

namespace cppvex {

//  ___     _       _ _   _
// | _ \_ _(_)_ __ (_) |_(_)_ _____
// |  _/ '_| | '  \| |  _| \ V / -_)
// |_| |_| |_|_|_|_|_|\__|_|\_/\___|

//  setprimgroup

void setprimgroup(GA_Detail *geo, const char *group_name,
                  const GA_Index prim_index, const bool value) {

  auto *prim_group = geo->findPrimitiveGroup(group_name);
  if (prim_group == nullptr)
    prim_group = geo->newPrimitiveGroup(group_name);

  const GA_Offset offset = geo->primitiveOffset(prim_index);

  prim_group->setOffset(offset, value);
}

void setprimgroup(const char *group_name, const GA_Index prim_index,
                  const bool value) {
  setprimgroup(output_geometry(), group_name, prim_index, value);
}

// inprimgroup

bool inprimgroup(const GA_Detail *geo, const char *group_name,
                 const GA_Index prim_index) {

  const auto *prim_group = geo->findPrimitiveGroup(group_name);

  if (prim_group == nullptr)
    return false;
  else
    return prim_group->containsIndex(prim_index);
}

bool inprimgroup(const char *group_name, const GA_Index prim_index) {
  return inprimgroup(output_geometry(), group_name, prim_index);
}

bool inprimgroup(const int input_index, const char *group_name,
                 const GA_Index prim_index) {
  return inprimgroup(input_geometry(input_index), group_name, prim_index);
}

// nprimitivesgroup

int nprimitivesgroup(const GA_Detail *geo, const char *group_name) {

  const auto *prim_group = geo->findPrimitiveGroup(group_name);

  if (prim_group == nullptr)
    return 0;

  return prim_group->entries();
}

int nprimitivesgroup(const char *group_name) {
  return nprimitivesgroup(output_geometry(), group_name);
}

int nprimitivesgroup(const int input_index, const char *group_name) {
  return nprimitivesgroup(input_geometry(input_index), group_name);
}

//  ___     _     _
// | _ \___(_)_ _| |_
// |  _/ _ \ | ' \  _|
// |_| \___/_|_||_\__|

// setpoingroup

void setpointgroup(GA_Detail *geo, const char *group_name,
                   const GA_Index point_index, const bool value) {

  auto *point_group = geo->findPointGroup(group_name);
  if (point_group == nullptr)
    point_group = geo->newPointGroup(group_name);

  const GA_Offset offset = geo->pointOffset(point_index);

  point_group->setOffset(offset, value);
}

void setpointgroup(const char *group_name, const GA_Index point_index,
                   const bool value) {
  setpointgroup(output_geometry(), group_name, point_index, value);
}

// inpointgroup

bool inpointgroup(const GA_Detail *geo, const char *group_name,
                  const GA_Index point_index) {

  const auto *point_group = geo->findPointGroup(group_name);

  if (point_group == nullptr)
    return false;
  else
    return point_group->containsIndex(point_index);
}

bool inpointgroup(const char *group_name, const GA_Index point_index) {
  return inpointgroup(output_geometry(), group_name, point_index);
}

bool inpointgroup(const int input_index, const char *group_name,
                  const GA_Index point_index) {
  return inpointgroup(input_geometry(input_index), group_name, point_index);
}

// npointsgroup

int npointsgroup(const GA_Detail *geo, const char *group_name) {

  const auto *point_group = geo->findPointGroup(group_name);

  if (point_group == nullptr)
    return 0;

  return point_group->entries();
}

int npointsgroup(const char *group_name) {
  return npointsgroup(output_geometry(), group_name);
}

int npointsgroup(const int input_index, const char *group_name) {
  return npointsgroup(input_geometry(input_index), group_name);
}

// __   __       _
// \ \ / /__ _ _| |_ _____ __
//  \ V / -_) '_|  _/ -_) \ /
//   \_/\___|_|  \__\___/_\_\

// setvertexgroup

void setvertexgroup(GA_Detail *geo, const char *group_name,
                    const GA_Index vertex_index, const bool value) {

  auto *vertex_group = geo->findVertexGroup(group_name);
  if (vertex_group == nullptr)
    vertex_group = geo->newVertexGroup(group_name);

  const GA_Offset offset = geo->vertexOffset(vertex_index);

  vertex_group->setOffset(offset, value);
}

void setvertexgroup(const char *group_name, const GA_Index vertex_index,
                    const bool value) {
  setvertexgroup(output_geometry(), group_name, vertex_index, value);
}

// invertexgroup

bool invertexgroup(const GA_Detail *geo, const char *group_name,
                   const GA_Index vertex_index) {

  const auto *vertex_group = geo->findVertexGroup(group_name);

  if (vertex_group == nullptr)
    return false;
  else
    return vertex_group->containsIndex(vertex_index);
}

bool invertexgroup(const char *group_name, const GA_Index vertex_index) {
  return invertexgroup(output_geometry(), group_name, vertex_index);
}

bool invertexgroup(const int input_index, const char *group_name,
                   const GA_Index vertex_index) {
  return invertexgroup(input_geometry(input_index), group_name, vertex_index);
}

// nvertexsgroup

int nvertexsgroup(const GA_Detail *geo, const char *group_name) {

  const auto *vertex_group = geo->findVertexGroup(group_name);

  if (vertex_group == nullptr)
    return 0;

  return vertex_group->entries();
}

int nvertexsgroup(const char *group_name) {
  return nvertexsgroup(output_geometry(), group_name);
}

int nvertexsgroup(const int input_index, const char *group_name) {
  return nvertexsgroup(input_geometry(input_index), group_name);
}

} // namespace cppvex
