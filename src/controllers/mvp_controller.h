#ifndef MVP_CONTROLLER_H
#define MVP_CONTROLLER_H

#include "mvpchange.h"

namespace s21 {

class MVPController {
 public:
  MVPController() = delete;
  MVPController(std::shared_ptr<MVPMatrix> model) : model_(model){};
  void RotateX(float angleX) { model_->RotateX(angleX); };
  void RotateY(float angleY) { model_->RotateY(angleY); };
  void RotateZ(float angleZ) { model_->RotateZ(angleZ); };
  void TranslateX(float shiftX) { model_->TranslateX(shiftX); };
  void TranslateY(float shiftY) { model_->TranslateY(shiftY); };
  void TranslateZ(float shiftZ) { model_->TranslateZ(shiftZ); };
  void Scale(float scale_factor) { model_->Scale(scale_factor); };
  void SetOffset(float offsetX, float offsetY, float offsetZ) {
    model_->SetOffset(offsetX, offsetY, offsetZ);
  };
  s21::S21Matrix GetMVPMatrix() { return model_->GetMVPMatrix(); };

 private:
  std::shared_ptr<MVPMatrix> model_;
};
}  // namespace s21
#endif  // MVP_CONTROLLER_H
