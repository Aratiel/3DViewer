#include <gtest/gtest.h>

#include "parser_controller.h"

TEST(parser, test1) {
  s21::Parser P;
  P.Parse("../objs/cube.obj");
  ASSERT_EQ(P.GetVertices().size(), 24);
  ASSERT_EQ(P.GetEdges().size(), 24);
}

TEST(parser, test2) {
  s21::ParserController::GetInstance().Parse("../objs/cube.obj");
  s21::ParserController::GetInstance().GetNormalized();
  ASSERT_EQ(s21::ParserController::GetInstance().GetVertices().size(), 24);
  ASSERT_EQ(s21::ParserController::GetInstance().GetEdges().size(), 24);
}
