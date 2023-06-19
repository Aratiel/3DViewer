#ifndef SRC_MODELS_MVPMATRIX_H_
#define SRC_MODELS_MVPMATRIX_H_
#include <math.h>

#include <vector>

#include "s21_matrix_oop.h"

namespace s21 {
class MVPMatrix {
 public:
  MVPMatrix() {
    S21Matrix ind = S21Matrix::GetIdentityMatrix(4);
    result_matrix_ = ind;
    mat_rotX_ = ind;
    mat_rotY_ = ind;
    mat_rotZ_ = ind;
    mat_scale_ = ind;
    mat_offset_ = ind;
    mat_move_ = ind;
  };
  void RotateX(float angleX);
  void RotateY(float angleY);
  void RotateZ(float angleZ);
  void TranslateX(float shiftX);
  void TranslateY(float shiftY);
  void TranslateZ(float shiftZ);
  void Scale(float scale_factor);
  void SetOffset(float offsetX, float offsetY, float offsetZ);
  s21::S21Matrix GetMVPMatrix() { return result_matrix_; };

 private:
  void Transform();
  s21::S21Matrix result_matrix_, mat_rotX_, mat_rotY_, mat_rotZ_, mat_scale_,
      mat_offset_, mat_move_;
};
}  // namespace s21

#endif  // SRC_MODELS_MVPMATRIX_H_
