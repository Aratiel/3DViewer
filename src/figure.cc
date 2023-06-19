#include "figure.h"

#include "viewer_widget.h"

namespace s21 {

QMatrix4x4 operator*(const QMatrix4x4 &qmat, s21::MVPController &mvpmat) {
  QMatrix4x4 newmvp;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      newmvp(i, j) = static_cast<float>(mvpmat.GetMVPMatrix()(
          static_cast<unsigned int>(i), static_cast<unsigned int>(j)));
    }
  }
  return qmat * newmvp;
}

Figure::Figure(std::string path, QOpenGLFunctions &function)
    : mvpmatrix_(std::make_shared<MVPMatrix>()), function_(function) {
  function_ = function;
  ParserController &parser_controller_ = ParserController::GetInstance();
  parser_controller_.Parse(path);
  Q_ASSERT(vertices_buffer_.create() == true);
  Q_ASSERT(indices_buffer_.create() == true);
  AllocateIndexBuffer(parser_controller_.GetEdges().data(),
                      parser_controller_.GetEdges().size());
  AllocateVertexBuffer(parser_controller_.GetVertices().data(),
                       parser_controller_.GetVertices().size());
  mvpmatrix_.SetOffset(parser_controller_.GetOffsetX(),
                       parser_controller_.GetOffsetY(),
                       parser_controller_.GetOffsetZ());
  window_size_ = parser_controller_.GetNormalized() + 1;
}

void Figure::AllocateIndexBuffer(int *indices, size_t count) {
  Q_ASSERT(indices_buffer_.bind() == true);
  indices_buffer_.allocate(indices, static_cast<int>(count * sizeof(int)));
  count_edges_ = count;
}

void Figure::AllocateVertexBuffer(float *vertices, size_t count) {
  Q_ASSERT(vertices_buffer_.bind() == true);
  vertices_buffer_.allocate(vertices, static_cast<int>(count * sizeof(float)));
  count_vertices_ = count;
}

void Figure::BindData(QOpenGLShaderProgram &shaders, QMatrix4x4 &projection) {
  BindMVPBuffer(shaders, projection);
  BindIndexBuffer();
  BindVertexBuffer(shaders);
}

void Figure::DrawLines(QOpenGLShaderProgram &shaders, int edges_type,
                       QColor edges_color, int color_location,
                       float edges_thickness) {
  glLineWidth(edges_thickness);
  if (edges_type == 0) {
    glDisable(GL_LINE_STIPPLE);
    glEnable(GL_LINES);
  } else if (edges_type == 1) {
    glLineStipple(5, 0x00FF);
    glEnable(GL_LINE_STIPPLE);
  }
  shaders.setUniformValue(color_location,
                          QVector4D(edges_color.redF(), edges_color.greenF(),
                                    edges_color.blueF(), edges_color.alphaF()));
  glDrawElements(GL_LINES, static_cast<int>(count_edges_), GL_UNSIGNED_INT,
                 nullptr);
}

void Figure::DrawPoints(QOpenGLShaderProgram &shaders, int vertices_type,
                        QColor vertices_color, int color_location,
                        float vertices_size) {
  glPointSize(vertices_size);
  if (vertices_type == 0) {
    glDisable(GL_POINT_SMOOTH);
  } else {
    if (vertices_type == 1) {
      glEnable(GL_POINT_SMOOTH);
    } else if (vertices_type == 2) {
      glDisable(GL_POINT_SMOOTH);
    }
    shaders.setUniformValue(
        color_location,
        QVector4D(vertices_color.redF(), vertices_color.greenF(),
                  vertices_color.blueF(), vertices_color.alphaF()));

    glDrawElements(GL_POINTS, static_cast<int>(count_edges_), GL_UNSIGNED_INT,
                   nullptr);
  }
}

void Figure::BindIndexBuffer() { Q_ASSERT(indices_buffer_.bind() == true); }

void Figure::BindVertexBuffer(QOpenGLShaderProgram &shaders) {
  Q_ASSERT(vertices_buffer_.bind() == true);
  int vertexLocation = shaders.attributeLocation("vertex_pos");
  Q_ASSERT(vertexLocation != -1);
  shaders.setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(float) * 3);
  shaders.enableAttributeArray(vertexLocation);
}

void Figure::BindMVPBuffer(QOpenGLShaderProgram &shaders,
                           QMatrix4x4 &projection) {
  int mvp_location = shaders.uniformLocation("mvp_matrix");
  Q_ASSERT(mvp_location != -1);
  shaders.setUniformValue(mvp_location, projection * mvpmatrix_);
}

}  // namespace s21
