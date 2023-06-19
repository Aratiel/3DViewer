// Copyright 2022 <lwolmer, jgerrick, lshiela>
#ifndef SRC_VIEWS_VIEWER_SETTINGS_H
#define SRC_VIEWS_VIEWER_SETTINGS_H

#include <QColor>
#include <QComboBox>
#include <QLineEdit>
#include <QSettings>
#include <QSlider>

class ViewerSettings : public QSettings {
 public:
  using QSettings::QSettings;
  void SaveElement(QLineEdit &elem);
  void SaveElement(QSlider &elem);
  void SaveElement(QComboBox &elem);
  void SaveVariable(QString name, QColor &var);

  void LoadElement(QLineEdit *elem);
  void LoadElement(QSlider *elem);
  void LoadElement(QComboBox *elem);
  void LoadVariable(QString name, QColor *var);
};
#endif  // SRC_VIEWS_VIEWER_SETTINGS_H
