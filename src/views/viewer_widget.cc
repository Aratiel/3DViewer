// Copyright 2022 <lwolmer, jgerrick, lshiela>
#include "viewer_widget.h"

#include <QImage>
#include <QImageWriter>
#include <QMouseEvent>
#include <cmath>
#include <memory>

#include "figure.h"

ViewerWidget::~ViewerWidget() { figures_.clear(); }

ViewerWidget::ViewerWidget(QWidget *parent, Qt::WindowFlags f)
    : QOpenGLWidget(parent, f) {}

void ViewerWidget::initializeGL() {
  initializeOpenGLFunctions();
  glClearColor(0.0, 0.0, 1.0, 0.0);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  initShaders();
}

void ViewerWidget::GifCreation() {
  if (is_recording_) {
    if (gif_frame_count_ < 50) {
      QImage screen = this->grabFramebuffer();
      gif_.addFrame(screen);
      gif_frame_count_++;
    } else {
      is_recording_ = false;
      gif_.setDefaultDelay(100);
      QString fileName = QFileDialog::getSaveFileName(
          this, tr("Save File"), QDir::homePath() + "/animation.gif",
          tr("Gif (*.gif)"));
      if (fileName.length()) gif_.save(fileName);
      gif_ = QGifImage(QSize(640, 480));
      gif_frame_count_ = 0;
      emit GifRecorded();
    }
  }
}

void ViewerWidget::TakeScreenshot() {
  QImage screen = this->grabFramebuffer();
  QString fileName = QFileDialog::getSaveFileName(
      this, tr("Save File"), QDir::homePath() + "/screenshoot.jpg",
      tr("JPEG (*.jpg *.jpeg);;BMP (*.bmp)"));
  if (fileName.length()) screen.save(fileName);
}

size_t ViewerWidget::GetFigureEdgesCount(size_t index) const {
  return figures_.at(index).get()->GetEdgesCount();
}
size_t ViewerWidget::GetFigureVerticesCount(size_t index) const {
  return figures_.at(index).get()->GetVerticesCount();
}

void ViewerWidget::TranslateFigureX(float translateX) {
  figures_.at(0)->TranslateX(translateX);
}

void ViewerWidget::TranslateFigureY(float translateY) {
  figures_.at(0)->TranslateY(translateY);
}

void ViewerWidget::TranslateFigureZ(float translateZ) {
  figures_.at(0)->TranslateZ(translateZ);
}

void ViewerWidget::RotateFigureX(float rotateX) {
  figures_.at(0)->RotateX(qDegreesToRadians(rotateX));
}

void ViewerWidget::RotateFigureY(float rotateY) {
  figures_.at(0)->RotateY(qDegreesToRadians(rotateY));
}

void ViewerWidget::RotateFigureZ(float rotateZ) {
  figures_.at(0)->RotateZ(qDegreesToRadians(rotateZ));
}

void ViewerWidget::ScaleFigure(float scale) { figures_.at(0)->Scale(scale); }

void ViewerWidget::SetupProjection(int width, int height) {
  float aspect = static_cast<float>(qreal(width) / qreal(height ? height : 1));

  // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
  const float zNear = 0.1f, zFar = 10000.0, fov = 70.0;
  // Reset projection
  projection_.setToIdentity();

  // Set projection
  if (projection_type_ == 0) {
    projection_.perspective(fov, aspect, zNear, zFar);
  } else {
    float window = figures_.at(0)->GetWindowSize();
    projection_.ortho(-aspect * window, aspect * window, -window, window,
                      -window * 10, window * 10);
  }
}

void ViewerWidget::initShaders() {
  // Compile vertex shader
  if (!shaders_.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                        ":vertex_shader"))
    qFatal("No vertex shader");

  // Compile fragment shader
  if (!shaders_.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                        ":fragment_shader"))
    qFatal("No fragment shader");

  // Link shader pipeline
  if (!shaders_.link()) close();

  // Bind shader pipeline for use
  if (!shaders_.bind()) close();
}

void ViewerWidget::resizeGL(int w, int h) { SetupProjection(w, h); }

void ViewerWidget::ObserverUpdate() { update(); };

void ViewerWidget::paintGL() {
  glClearColor(background_color_.redF(), background_color_.greenF(),
               background_color_.blueF(), background_color_.alphaF());
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  int color_location = shaders_.uniformLocation("color");
  Q_ASSERT(color_location != -1);
  for (auto &figure : figures_) {
    figure.get()->BindData(shaders_, projection_);
    figure.get()->DrawLines(shaders_, edges_type_, edges_color_, color_location,
                            edges_thickness_);
    figure.get()->DrawPoints(shaders_, vertices_type_, vertices_color_,
                             color_location, vertices_size_);
  }

  GifCreation();
}

void ViewerWidget::LoadModel(QString path) {
  if (path.length()) {
    figures_.clear();
    figures_.shrink_to_fit();
    figures_.push_back(std::make_unique<s21::Figure>(
        path.toStdString(), *dynamic_cast<QOpenGLFunctions *>(this)));
    figures_.at(figures_.size() - 1).get()->AddObserver(this);
  }
}
