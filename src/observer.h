// Copyright 2022 <lwolmer, jgerrick, lshiela>
#ifndef SRC_OBSERVER_H_
#define SRC_OBSERVER_H_
#include <algorithm>
#include <vector>

namespace s21 {
class Observer {
 public:
  virtual void ObserverUpdate() = 0;
  virtual ~Observer(){};
};

class Observable {
 public:
  void AddObserver(Observer *observer) { observers_.push_back(observer); }
  void RemoveObserver(Observer *observer) {
    observers_.erase(
        std::remove(observers_.begin(), observers_.end(), observer),
        observers_.end());
  }
  void NotifyUpdate() {
    size_t size = observers_.size();
    for (size_t i = 0; i < size; i++) {
      observers_[i]->ObserverUpdate();
    }
  }

 private:
  std::vector<Observer *> observers_;
};
}  // namespace s21
#endif  // SRC_OBSERVER_H_
