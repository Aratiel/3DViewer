#ifndef SRC_MODELS_PARSER_H_
#define SRC_MODELS_PARSER_H_

#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace s21 {
class Parser {
 public:
  void Parse(std::string path);
  std::vector<float> GetVertices() const { return vertices_; }
  std::vector<int> GetEdges() const { return edges_; }
  float GetOffsetX() const {
    return (min_max_.at(0).first + min_max_.at(0).second) / -2;
  }
  float GetOffsetY() const {
    return (min_max_.at(1).first + min_max_.at(1).second) / -2;
  }
  float GetOffsetZ() const {
    return (min_max_.at(2).first + min_max_.at(2).second) / -2;
  }
  float GetNormalized() const;
  void Clear();

 private:
  std::vector<float> vertices_ = {};
  std::vector<int> edges_ = {};
  // first is min, second is max
  std::vector<std::pair<float, float>> min_max_ = {};

  void ScanMinMax(float val, std::pair<float, float>* min_max);
  void ScanVertices(std::string line);
  void ScanEdge(std::string line);
};
}  // namespace s21

#endif  // SRC_MODELS_PARSER_H_
