#pragma once

namespace cppvex {

//  ___     _       _ _   _
// | _ \_ _(_)_ __ (_) |_(_)_ _____
// |  _/ '_| | '  \| |  _| \ V / -_)
// |_| |_| |_|_|_|_|_|\__|_|\_/\___|

auto setprimgroup(GA_Detail *geo, const char *group_name) {

  auto *prim_group = geo->findPrimitiveGroup(group_name);
  if (prim_group == nullptr)
    prim_group = geo->newPrimitiveGroup(group_name);

  return [prim_group](const GA_Offset prim_offset, const bool value) {
    prim_group->setOffset(prim_offset, value);
  };
}

void setprimgroup(GA_Detail *geo, const char *group_name,
                  const GA_Offset prim_offset, const bool value) {
  setprimgroup(geo, group_name)(prim_offset, value);
}

auto inprimgroup(const GA_Detail *geo, const char *group_name) {

  const auto *prim_group = geo->findPrimitiveGroup(group_name);

  return [prim_group](const GA_Offset prim_offset) -> bool {
    if (prim_group == nullptr)
      return false;
    return prim_group->containsOffset(prim_offset);
  };
}

bool inprimgroup(const GA_Detail *geo, const char *group_name,
                 const GA_Offset prim_offset) {
  return inprimgroup(geo, group_name)(prim_offset);
}

int nprimitivesgroup(const GA_Detail *geo, const char *group_name) {

  const auto *prim_group = geo->findPrimitiveGroup(group_name);

  if (prim_group == nullptr)
    return 0;

  return prim_group->entries();
}

//  ___     _     _
// | _ \___(_)_ _| |_
// |  _/ _ \ | ' \  _|
// |_| \___/_|_||_\__|

auto setpointgroup(GA_Detail *geo, const char *group_name) {
  auto *point_group = geo->findPointGroup(group_name);
  if (point_group == nullptr)
    point_group = geo->newPointGroup(group_name);

  return [point_group](const GA_Offset point_offset, const bool value) {
    point_group->setOffset(point_offset, value);
  };
}

void setpointgroup(GA_Detail *geo, const char *group_name,
                   const GA_Offset point_offset, const bool value) {
  setpointgroup(geo, group_name)(point_offset, value);
}

auto inpointgroup(const GA_Detail *geo, const char *group_name) {

  const auto *point_group = geo->findPointGroup(group_name);

  return [point_group](const GA_Offset point_offset) -> bool {
    if (point_group == nullptr)
      return false;
    return point_group->containsOffset(point_offset);
  };
}

bool inpointgroup(const GA_Detail *geo, const char *group_name,
                  const GA_Offset point_offset) {
  return inpointgroup(geo, group_name)(point_offset);
}

int npointsgroup(const GA_Detail *geo, const char *group_name) {

  const auto *point_group = geo->findPointGroup(group_name);

  if (point_group == nullptr)
    return 0;

  return point_group->entries();
}

// __   __       _
// \ \ / /__ _ _| |_ _____ __
//  \ V / -_) '_|  _/ -_) \ /
//   \_/\___|_|  \__\___/_\_\


auto setvertexgroup(GA_Detail *geo, const char *group_name) {
  auto *vertex_group = geo->findVertexGroup(group_name);
  if (vertex_group == nullptr)
    vertex_group = geo->newVertexGroup(group_name);

  return [vertex_group](const GA_Offset vertex_offset, const bool value) {
    vertex_group->setOffset(vertex_offset, value);
  };
}

void setvertexgroup(GA_Detail *geo, const char *group_name,
                    const GA_Offset vertex_offset, const bool value) {
  setvertexgroup(geo, group_name)(vertex_offset, value);
}

auto invertexgroup(const GA_Detail *geo, const char *group_name) {

  const auto *vertex_group = geo->findVertexGroup(group_name);

  return [vertex_group](const GA_Offset vertex_offset) -> bool {
    if (vertex_group == nullptr)
      return false;
    return vertex_group->containsOffset(vertex_offset);
  };
}

bool invertexgroup(const GA_Detail *geo, const char *group_name,
                   const GA_Offset vertex_offset) {
  return invertexgroup(geo, group_name)(vertex_offset);
}

int nverticesgroup(const GA_Detail *geo, const char *group_name) {

  const auto *vertex_group = geo->findVertexGroup(group_name);

  if (vertex_group == nullptr)
    return 0;

  return vertex_group->entries();
}

} // namespace cppvex
