#include <gtest/gtest.h>

#include "mvp_controller.h"

TEST(mvp, moove) {
  s21::MVPMatrix matrix_mvp;
  matrix_mvp.TranslateX(2);
  matrix_mvp.TranslateY(3);
  matrix_mvp.TranslateZ(1);
  matrix_mvp.TranslateX(1);
  double mat_raw4x4[] = {1, 0, 0, 1, 0, 1, 0, 3, 0, 0, 1, 1, 0, 0, 0, 1};
  s21::S21Matrix matrix_test(mat_raw4x4, 4, 4);
  ASSERT_TRUE(matrix_mvp.GetMVPMatrix() == matrix_test);
}

TEST(mvp, multi_rotate) {
  s21::MVPMatrix matrix_mvp;
  matrix_mvp.RotateX(1);
  matrix_mvp.RotateY(1);
  matrix_mvp.RotateZ(1);
  matrix_mvp.SetOffset(0, 1, 0);
  matrix_mvp.Scale(2);
  double mat_raw4x4[] = {0.5838531, -0.9092973, 1.6829419,  -0.4546487,
                         1.6744447, -0.6077933, -0.9092973, -0.3038966,
                         0.9248513, 1.6744447,  0.5838531,  0.8372223,
                         0.000000,  0.000000,   0.000000,   1.000000};
  s21::S21Matrix matrix_test(mat_raw4x4, 4, 4);
  ASSERT_TRUE(matrix_mvp.GetMVPMatrix() == matrix_test);
}

TEST(mvp, another_way) {
  s21::MVPController c_mvpmatrix(std::make_shared<s21::MVPMatrix>());
  s21::MVPMatrix matrix_mvp;

  matrix_mvp.RotateX(1);
  matrix_mvp.RotateY(1);
  matrix_mvp.RotateZ(1);
  matrix_mvp.SetOffset(0, 1, 0);
  matrix_mvp.Scale(2);
  //  не зависимо от порядка и количества преобразований можно получить
  //  идентичный результат
  c_mvpmatrix.RotateX(3);
  c_mvpmatrix.RotateZ(1);
  c_mvpmatrix.SetOffset(0, 1, 0);
  c_mvpmatrix.Scale(2);
  c_mvpmatrix.RotateY(1);
  c_mvpmatrix.RotateX(1);

  ASSERT_TRUE(matrix_mvp.GetMVPMatrix() == c_mvpmatrix.GetMVPMatrix());
}
