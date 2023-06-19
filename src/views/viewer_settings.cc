// Copyright 2022 <lwolmer, jgerrick, lshiela>
#include "viewer_settings.h"

void ViewerSettings::SaveElement(QLineEdit &elem) {
  setValue(elem.objectName(), elem.text());
}

void ViewerSettings::SaveElement(QSlider &elem) {
  setValue(elem.objectName(), elem.value());
}

void ViewerSettings::SaveElement(QComboBox &elem) {
  setValue(elem.objectName(), elem.currentIndex());
}

void ViewerSettings::SaveVariable(QString name, QColor &var) {
  setValue(name, var.name());
}

void ViewerSettings::LoadElement(QLineEdit *elem) {
  elem->setText(value(elem->objectName()).toString());
}

void ViewerSettings::LoadElement(QSlider *elem) {
  elem->setValue(value(elem->objectName()).toInt());
}

void ViewerSettings::LoadElement(QComboBox *elem) {
  elem->setCurrentIndex(value(elem->objectName()).toInt());
}

void ViewerSettings::LoadVariable(QString name, QColor *var) {
  if (contains(name)) *var = QColor(value(name).toString());
}
