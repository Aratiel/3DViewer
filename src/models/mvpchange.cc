#include "mvpchange.h"
namespace s21 {
void MVPMatrix::RotateX(float angleX) {
  mat_rotX_(1, 1) = cos(angleX);
  mat_rotX_(2, 2) = cos(angleX);
  mat_rotX_(1, 2) = -sin(angleX);
  mat_rotX_(2, 1) = sin(angleX);
  Transform();
}

void MVPMatrix::RotateY(float angleY) {
  mat_rotY_(0, 0) = cos(angleY);
  mat_rotY_(2, 2) = cos(angleY);
  mat_rotY_(2, 0) = -sin(angleY);
  mat_rotY_(0, 2) = sin(angleY);
  Transform();
}

void MVPMatrix::RotateZ(float angleZ) {
  mat_rotZ_(0, 0) = cos(angleZ);
  mat_rotZ_(1, 1) = cos(angleZ);
  mat_rotZ_(0, 1) = -sin(angleZ);
  mat_rotZ_(1, 0) = sin(angleZ);
  Transform();
}

void MVPMatrix::TranslateX(float shiftX) {
  mat_move_(0, 3) = shiftX;
  Transform();
}

void MVPMatrix::TranslateY(float shiftY) {
  mat_move_(1, 3) = shiftY;
  Transform();
}

void MVPMatrix::TranslateZ(float shiftZ) {
  mat_move_(2, 3) = shiftZ;
  Transform();
}

void MVPMatrix::Scale(float scale_factor) {
  mat_scale_ = S21Matrix::GetIdentityMatrix(4);
  mat_scale_ = mat_scale_ * scale_factor;
  mat_scale_(3, 3) = 1;
  Transform();
}

void MVPMatrix::Transform() {
  result_matrix_ =
      mat_move_ * mat_rotX_ * mat_rotY_ * mat_rotZ_ * mat_offset_ * mat_scale_;
}

void MVPMatrix::SetOffset(float offsetX, float offsetY, float offsetZ) {
  mat_offset_(0, 3) = offsetX;
  mat_offset_(1, 3) = offsetY;
  mat_offset_(2, 3) = offsetZ;
  Transform();
}
}  // namespace s21
