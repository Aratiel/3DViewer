#include "parser.h"

namespace s21 {
void Parser::Parse(std::string path) {
  std::ifstream file(path);  // файл из которого читаем
  if (!file.is_open())       // если не удалось открыть
    throw std::runtime_error("The file does not exist.");
  std::string line = {};  // считанные строки
  while (std::getline(file, line)) {  // очередную строку в переменную (line)
    if (line.size() > 2) {
      if (line.at(0) == 'v' && line.at(1) == ' ') {  //  если вершины
        ScanVertices(line);
      } else if (line.at(0) == 'f' && line.at(1) == ' ') {
        ScanEdge(line);  //  многоугольник разобрать на рёбра
      }
    }
  }
  file.close();  // закрываем файл
}

void Parser::Clear() {
  vertices_.clear();
  edges_.clear();
  min_max_.clear();
}

void Parser::ScanMinMax(float val, std::pair<float, float> *min_max) {
  if (min_max->first > val) min_max->first = val;
  if (min_max->second < val) min_max->second = val;
}

float Parser::GetNormalized() const {
  float lenX = min_max_.at(0).second - min_max_.at(0).first;
  float lenY = min_max_.at(1).second - min_max_.at(1).first;
  float lenZ = min_max_.at(2).second - min_max_.at(2).first;
  float result = std::fmax(lenX, std::fmax(lenY, lenZ));
  return result;
}

void Parser::ScanVertices(std::string line) {
  float x = 0, y = 0, z = 0;
  sscanf(line.c_str(), "v %f %f %f", &x, &y, &z);
  if (min_max_.empty()) {  //  начальная инициализация лимитов
    min_max_.push_back(std::pair<float, float>(x, x));
    min_max_.push_back(std::pair<float, float>(y, y));
    min_max_.push_back(std::pair<float, float>(z, z));
  }
  vertices_.push_back(x);
  vertices_.push_back(y);
  vertices_.push_back(z);
  ScanMinMax(x, &min_max_.at(0));  //  поиск лимитов
  ScanMinMax(y, &min_max_.at(1));
  ScanMinMax(z, &min_max_.at(2));
}

void Parser::ScanEdge(std::string line) {
  int count = 0;  //  счётчик вершин многоугольника
  int first_vet = 0;  //  Первая вершина многоугольника
  int last_vet = 0;  //  Последняя вершина многоугольника
  std::istringstream iss(line);
  for (std::string token; std::getline(iss, token, ' ');) {
    int ind_tmp = 0;  //  Текущая вершина
    if (sscanf(token.c_str(), "%d", &ind_tmp) == 1) {
      if (count == 0) {
        first_vet = ind_tmp - 1;
        edges_.push_back(first_vet);
        count = 1;
      } else if (count == 1) {
        last_vet = ind_tmp - 1;
        edges_.push_back(last_vet);
        count = 2;
      } else if (count >= 2) {
        edges_.push_back(last_vet);
        last_vet = ind_tmp - 1;
        edges_.push_back(last_vet);
        count = count + 2;
      }
    }
  }
  if (count > 2) {  //  Замыкающее ребро в многоугольник
    edges_.push_back(last_vet);
    edges_.push_back(first_vet);
  }
}
}  // namespace s21
