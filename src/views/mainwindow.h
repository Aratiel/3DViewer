// Copyright 2022 <lwolmer, jgerrick, lshiela>
#ifndef SRC_VIEWS_MAINWINDOW_H_
#define SRC_VIEWS_MAINWINDOW_H_

#include <QColor>
#include <QColorDialog>
#include <QLabel>
#include <QMainWindow>

#include "figure.h"
#include "parser_controller.h"
#include "viewer_settings.h"
#include "viewer_widget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT
  friend s21::Figure;

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private:
  void SaveSettings();
  void LoadSettings();
  void ApplySettings();
  void ConnectToSignals();
  void OpenFile(QString path);
  void InitValidators();
  void SetLabelPixmap(QLabel *label, QColor color);
  void OnChange(QLineEdit *line, QSlider *slider,
                void (ViewerWidget::*ptrMoveFunc)(float));
  void LoadDefaultFigure();
  void SetDefaultSliders();

  ViewerWidget *gl_widget_ = nullptr;
  QColor bg_color_ = {};
  QColor edges_color_ = {};
  QColor vertices_color_ = {};
  ViewerSettings *settings_ = nullptr;
  Ui::MainWindow *ui;

 private slots:
  void OnWidgetLoad();
  void OnOpenFilePressed();
  void OnProjectionTypeChange();
  void OnBGColorPressed();
  void OnBGColorChange();
  void OnEdgeTypeChange();
  void OnEdgeThicknessChange();
  void OnEdgeColorPressed();
  void OnEdgeColorChange();
  void OnVertexTypeChange();
  void OnVertexColorPressed();
  void OnVertexColorChange();
  void OnVertexSizeChange();
  void OnMakeScreenshootPressed();
  void OnMakeGifPressed();
};
#endif  // SRC_VIEWS_MAINWINDOW_H_
