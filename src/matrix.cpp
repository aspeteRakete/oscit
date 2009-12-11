#include "oscit/values.h"

#include <vector>

#include "opencv/cv.h"


namespace oscit {

/**
 * cv::Mat is defined in cxcore.hpp:686
 */
struct Matrix::Implementation : public cv::Mat {
  Implementation() {}
  Implementation(int rows, int cols, int type) : cv::Mat(rows, cols, type) {}

  bool operator==(const Matrix::Implementation &other) {
    // FIXME: this is all wrong but who really needs to compare matrices anyway ?
    // This implementation is just here so that we can compare Values.
    std::cerr << __FILE__ << ":" << __LINE__ << " WARNING: comparing matrices is not implemented, it might return wrong results\n";
    return rows == other.rows && cols == other.cols;
  }
};

Matrix::Matrix() {
  impl_ = new Matrix::Implementation();
}

Matrix::~Matrix() {
  delete impl_;
}

Matrix::Matrix(int rows, int cols) {
#if Real == double
  impl_ = new Matrix::Implementation(rows, cols, CV_64FC1 );
#else
  impl_ = new Matrix::Implementation(rows, cols, CV_32FC1 );
#endif
}

Matrix::Matrix(int rows, int cols, int type) {
  impl_ = new Matrix::Implementation(rows, cols, type);
}

Matrix::Matrix(const Matrix &other) {
  impl_ = new Matrix::Implementation(*other.impl_);
}

size_t Matrix::rows() {
  return impl_->rows;
}

size_t Matrix::cols() {
  return impl_->cols;
}

void *Matrix::data() {
  return impl_->data;
}

int Matrix::type() {
  return impl_->type();
}

const cv::Mat *Matrix::cv_mat() {
  return impl_;
}

bool Matrix::operator==(const Matrix &other) const {
  return *impl_ == *other.impl_;
}

} // oscit

// opencv
#include "opencv/cxalloc.cpp"
#include "opencv/cxsystem.cpp"
