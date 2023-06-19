#ifndef SRC_MODELS_FIGURE_H_
#define SRC_MODELS_FIGURE_H_
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QVector3D>
#include <iostream>
#include <string>

#include "mvp_controller.h"
#include "mvpchange.h"
#include "observer.h"

namespace s21 {
class Figure : public Observable {
 public:
  Figure(std::string path, QOpenGLFunctions &function);
  Figure() = delete;
  ~Figure() = default;
  void BindData(QOpenGLShaderProgram &shaders, QMatrix4x4 &projection);

  void DrawLines(QOpenGLShaderProgram &shaders, int edges_type,
                 QColor edges_color, int color_location, float edges_thickness);
  void DrawPoints(QOpenGLShaderProgram &shaders, int vertices_type,
                  QColor vertices_color, int color_location,
                  float vertices_size);
  size_t GetEdgesCount() const { return count_edges_ / 2; }
  size_t GetVerticesCount() const { return count_vertices_ / 3; }
  void AllocateIndexBuffer(int *indices, size_t count);
  void AllocateVertexBuffer(float *vertices, size_t count);
  void BindIndexBuffer();
  void BindVertexBuffer(QOpenGLShaderProgram &shaders_);
  void BindMVPBuffer(QOpenGLShaderProgram &shaders, QMatrix4x4 &projection);
  void TranslateX(float shiftX) {
    mvpmatrix_.TranslateX(shiftX);
    NotifyUpdate();
  }
  void TranslateY(float shiftY) {
    mvpmatrix_.TranslateY(shiftY);
    NotifyUpdate();
  }
  void TranslateZ(float shiftZ) {
    mvpmatrix_.TranslateZ(shiftZ);
    NotifyUpdate();
  }
  void RotateX(float rotationX) {
    mvpmatrix_.RotateX(rotationX);
    NotifyUpdate();
  }
  void RotateY(float rotationY) {
    mvpmatrix_.RotateY(rotationY);
    NotifyUpdate();
  }
  void RotateZ(float rotationZ) {
    mvpmatrix_.RotateZ(rotationZ);
    NotifyUpdate();
  }
  void Scale(float scale) {
    mvpmatrix_.Scale(scale);
    NotifyUpdate();
  }
  float GetWindowSize() { return window_size_; }

 private:
  float window_size_;
  size_t count_edges_;
  size_t count_vertices_;
  QOpenGLBuffer vertices_buffer_{QOpenGLBuffer::VertexBuffer};
  QOpenGLBuffer indices_buffer_{QOpenGLBuffer::IndexBuffer};
  MVPController mvpmatrix_;
  QOpenGLFunctions &function_;
};

QMatrix4x4 operator*(const QMatrix4x4 &qmat, s21::MVPMatrix &mvpmat);
}  // namespace s21

#endif  // SRC_MODELS_FIGURE_H_
