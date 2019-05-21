#pragma once

#include <Eigen/Sparse>

#include "hougen_element.h"

namespace hougen {

// GA_Size  getNumElement(const GA_Detail * geo, const GA_

/*
 * @Brief Projection matrix from the full space of attributes to attributes in
 * or out of a specific group
 *
 * Suppose you have a geometry with N elements and a group "test_group" with M
 * elements. This function retuns the two following matrices:
 *
 * P1 - the first matrix  - M x N     - all elements to group elements
 * P2 - The second matrix - (N-M) x N - all elements to non-group elements
 *
 * If you have a N x 1 vector `V` of some values, to get values only in the
 * specificied group just compute `P1*V`. Values not in the group are computed
 * with `P2*V`
 *
 * The first matrix `P1` defines a group local index of an element.
 * Non-zero element, at location `(i,j)`, of `P1` corresponds to an element
 * with a global index `j` and its group local index is defined as `i`.
 *
 * For non-existent groups:
 * The return matrices are 0 x N matrix and N x N identity matrix.
 *
 * @param geo Geometry to build group projection for.
 * @param group Group of elements
 * @return First - projection to group elements, Second - projection to
 * non-group elements
 */
template <class Real = double>
std::pair<Eigen::SparseMatrix<Real, Eigen::RowMajor>,
          Eigen::SparseMatrix<Real, Eigen::RowMajor>>
group_projection(const GA_Detail *geo, const GA_AttributeOwner owner,
                 const char *group_name) {

  using SpMat = Eigen::SparseMatrix<Real, Eigen::RowMajor>;

  const GA_ElementGroup *group = geo->findElementGroup(owner, group_name);
  const int              N     = getNumElements(geo, owner);

  // If group does not exists
  if (group == nullptr) {
    SpMat P1(0, N);
    SpMat P2(N, N);
    P2.setIdentity();
    return {P1, P2};
  }

  const int M = group->entries();

  SpMat P1(M, N);
  SpMat P2(N - M, N);
  P1.reserve(Eigen::VectorXi::Constant(M, 1));
  P2.reserve(Eigen::VectorXi::Constant(N - M, 1));

  GA_Index group_index     = 0;
  GA_Index non_group_index = 0;
  for (GA_Index j = 0; j < N; j++) {
    if (group->containsIndex(j)) {
      P1.insert(group_index++, j) = 1;
    } else {
      P2.insert(non_group_index++, j) = 1;
    }
  }
  P1.makeCompressed();
  P2.makeCompressed();

  return {P1, P2};
}

template <class Real = double>
Eigen::SparseMatrix<Real> vertex_to_point_projection(const GA_Detail *geo) {

  const int pointNum  = geo->getNumPoints();
  const int vertexNum = geo->getNumVertices();

  Eigen::SparseMatrix<Real> P(pointNum, vertexNum);
  P.reserve(Eigen::VectorXi::Constant(vertexNum, 1));

  for (int j = 0; j < vertexNum; j++) {
    GA_Offset vtx_offset = geo->vertexOffset(j);
    GA_Offset pt_offset  = geo->vertexPoint(vtx_offset);
    const int i          = geo->pointIndex(pt_offset);

    P.insert(i, j) = 1;
  }
  P.makeCompressed();

  return P;
}

template <class Real = double>
Eigen::SparseMatrix<Real> primvertex_to_point_projection(const GA_Detail *geo) {

  const int pointNum  = geo->getNumPoints();
  const int vertexNum = geo->getNumVertices();
  const int primNum   = geo->getNumPrimitives();

  Eigen::SparseMatrix<Real> P(pointNum, vertexNum);
  P.reserve(Eigen::VectorXi::Constant(vertexNum, 1));

  int j = 0;
  for (GA_Index pi = 0; pi < primNum; pi++) {
    const auto *prim     = geo->getPrimitiveByIndex(pi);
    const int   primSize = prim->getVertexCount();
    for (int vi = 0; vi < primSize; vi++) {
      const int i = prim->getPointIndex(vi);

      P.insert(i, j) = 1;
      j++;
    }
  }
  P.makeCompressed();

  return P;
}

} // namespace hougen
