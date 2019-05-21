#pragma once

#include <SOP/SOP_Node.h>

namespace cppvex {

// In future, these global variables should be unique for each process, such
// that cppvex functions can be safelly called in parallel
namespace internal {

// Time
float time;

// Geometry
std::vector<const GU_Detail *> input_geometry;
GU_Detail *                    output_geometry;

// Nodes
SOP_Node *current_node;
SOP_Node *execution_node;

} // namespace internal

const GU_Detail *input_geometry(const int i) {
  return internal::input_geometry[i];
}

GU_Detail *output_geometry() { return internal::output_geometry; }

SOP_Node *current_node() { return internal::current_node; }

SOP_Node *execution_node() { return internal::execution_node; }

void set_up_cppvex_context(float time, SOP_Node *_current_node,
                           SOP_Node * _execution_node,
                           GU_Detail *_output_geometry) {
  // Time
  internal::time = time;

  // Nodes
  internal::current_node   = _current_node;
  internal::execution_node = _execution_node;

  // Output Geometry
  internal::output_geometry = _output_geometry;

  // Input Geometry
  int input_num = _current_node->nInputs();
  internal::input_geometry.resize(input_num);
  for (int i = 0; i < input_num; i++) {
    internal::input_geometry[i] = _current_node->getInputLastGeo(i, time);
  }

  // If we run this from CppWrangle, the execution and current nodes are not the
  // same and we have to make sure to re-evaluate when any change happen to the
  // wrangle node
  if (_execution_node != _current_node) {
    _execution_node->addExtraInput(_current_node, OP_INTEREST_DATA);
  }
}

} // namespace cppvex
