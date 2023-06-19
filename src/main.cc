// Copyright 2022 <lwolmer, jgerrick, lshiela>
#include <QApplication>
#include <QLabel>
#include <QSurfaceFormat>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  MainWindow w;
  w.show();
  return app.exec();
}
