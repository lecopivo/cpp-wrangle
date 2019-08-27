#pragma once

#include "ObjectBlob.h"

#ifdef GENERATE_OBJECT_BLOB_CODE

#define REGISTER_OBJECT(T)                                                     \
  template class ObjectBlob<T>;                                                \
  template ObjectBlob<T> *new_ObjectBlob<T>(T t);

#else

#define REGISTER_OBJECT(T)                                                     \
  extern template ObjectBlob<T> *new_ObjectBlob<T>(T t);                       \
  extern template class ObjectBlob<T>;

#endif

#include <memory>
#include <vector>

#include <Eigen/Sparse>

REGISTER_OBJECT(int)
REGISTER_OBJECT(float)
REGISTER_OBJECT(double)

REGISTER_OBJECT(std::vector<int>)
REGISTER_OBJECT(std::vector<float>)
REGISTER_OBJECT(std::vector<double>)

REGISTER_OBJECT(Eigen::SparseMatrix<float>)
REGISTER_OBJECT(Eigen::SparseMatrix<double>)
using SparseMatrixFloatRowMajor = Eigen::SparseMatrix<float, Eigen::RowMajor>;
using SparseMatrixDoubleRowMajor = Eigen::SparseMatrix<double, Eigen::RowMajor>;
REGISTER_OBJECT(SparseMatrixFloatRowMajor);
REGISTER_OBJECT(SparseMatrixDoubleRowMajor);

REGISTER_OBJECT(
    std::unique_ptr<Eigen::SimplicialLLT<Eigen::SparseMatrix<float>>>)
REGISTER_OBJECT(
    std::unique_ptr<Eigen::SimplicialLLT<Eigen::SparseMatrix<double>>>)
REGISTER_OBJECT(
    std::unique_ptr<Eigen::SparseLU<Eigen::SparseMatrix<float>>>)
REGISTER_OBJECT(
    std::unique_ptr<Eigen::SparseLU<Eigen::SparseMatrix<double>>>)
