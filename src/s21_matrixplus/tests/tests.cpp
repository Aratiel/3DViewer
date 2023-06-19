// Copyright <lwolmer> 2022
#include <gtest/gtest.h>
#include <gtest/gtest-spi.h>
#include <stdexcept>
#include "s21_matrix_oop.h"

using s21::S21Matrix;

void ASSERT_MATRIX(S21Matrix const &mat1, S21Matrix const &mat2) {
    ASSERT_EQ(mat1.getColumns(), mat2.getColumns());
    ASSERT_EQ(mat1.getRows(), mat2.getRows());
    unsigned int columns = static_cast<unsigned>(mat1.getColumns());
    unsigned int rows = static_cast<unsigned>(mat1.getRows());
    for (unsigned int i = 0; i < rows; i++) {
        for (unsigned int j = 0; j < columns; j++) {
            ASSERT_DOUBLE_EQ(mat1(i, j), mat2(i, j));
        }
    }
}

double mat_raw5x5[] = {1, 2, 3, 4, -5,
                    4.24, 121, 2, 5, 5,
                    -1, 23, 51, 52, -7,
                    1, 5, 79, 32, 14,
                    67, 5.42, -545, 4, 8};

double mat_raw5x5_2[] = {5, 3, 3, 9 , 5,
                        3.4, -121, 2, 5, 5,
                        1, 4, 51, 3, -7,
                        53, -5, 79.5, 32, 14,
                        62, 4.422, -9, 4, 5};

double mat_raw2x6[] = {5, 3,
                    3.4, -121,
                    1, 4,
                    53, -5,
                    62, 4.422,
                    5.67, 45.8};

double mat_raw2x6_2[] = {7.5, 555,
                    -25, 4,
                    6, -5.9,
                    5, 99,
                    4126.5, 0.55,
                    12.6, -11};

double mat_raw3x3[] = {12, 32, 35,
                        47, 85, 62,
                        74, 68, 98};

double mat_raw3x3_2[] = {11, 12, 9,
                        -15, 16, 8,
                        4, 5, 6};

double mat_raw3x3_3[] = {0, -2, 0,
                        3, 2, -1,
                        5, 1, 0};

double mat_raw3x3_4[] = {1, 2, 3,
                        0, 2, 2,
                        1, 4, 5};

double mat_raw3x5[] = {1, 2, 3, 4, -5,
                    4.24, 121, 2, 5, 5,
                    -1, 23, 51, 52, -7};

double raw_5x5_sum[] = {6, 5, 6, 13, 0,
                        7.64, 0, 4, 10, 10,
                        0, 27, 102, 55, -14,
                        54, 0, 158.5, 64, 28,
                        129, 9.842, -554, 8, 13};

double raw_2x6_sub[] = {-2.5, -552,
                        28.4, -125,
                        -5, 9.9,
                        48, -104,
                        -4064.5, 3.872,
                        -6.93, 56.8};

double raw_5x5_mult_by_5[] = {5, 10, 15, 20, -25,
                            21.2, 605, 10, 25, 25,
                            -5, 115, 255, 260, -35,
                            5, 25, 395, 160, 70,
                            335, 27.1, -2725, 20, 40};

double raw_5x5_mult[] = {-83.2, -269.11, 523, 136, 25,
                            1009.6, -14623.17, 709.22, 829.16, 707.2,
                            2446.2, -2872.954, 6841, 1895, 446,
                            2665, -384.092, 6460, 1351, -5,
                            516.428, -2619.444, -27337.16, -844.9, 4273.1};

double raw_5x3_transpose[] = {1, 4.24, -1,
                            2, 121, 23,
                            3, 2, 51,
                            4, 5, 52,
                            -5, 5, -7};

double raw_3x3_compl[] = {4114, -18, -3094,
                        -756, -1414, 1552,
                        -991, 901, -484};

double raw_3x3_inverse[] = {0.1, 0, 0.2,
                            -0.5, 0, 0,
                            -0.7, -1, 0.6};

#define DEFAULT_ROW_COL_SIZE 4

TEST(CREATION, CONSTRUCTORS) {
    S21Matrix matrix = S21Matrix();
    ASSERT_EQ(matrix.getColumns(), DEFAULT_ROW_COL_SIZE);
    ASSERT_EQ(matrix.getRows(), DEFAULT_ROW_COL_SIZE);
    S21Matrix matrix_with_size(3, 3);
    ASSERT_EQ(matrix_with_size.getColumns(), 3);
    ASSERT_EQ(matrix_with_size.getRows(), 3);
    matrix_with_size(0, 0) = 1;
    matrix_with_size(2, 2) = 1;
    S21Matrix matrix_from_matrix(matrix_with_size);
    ASSERT_MATRIX(matrix_with_size, matrix_from_matrix);
}

TEST(CREATION, CONSTRUCTORS_MOVE) {
    S21Matrix matrix1(mat_raw5x5, 5, 5);
    S21Matrix matrix2(std::move(matrix1));
    S21Matrix result(mat_raw5x5, 5, 5);
    ASSERT_MATRIX(matrix2, result);
}

TEST(CREATION, CONSTRUCTORS_EXCEPTION_2) {
    EXPECT_THROW({
        S21Matrix matrix(1, 0);
    }, std::invalid_argument);
}

TEST(CREATION, DESTRUCTOR) {
    S21Matrix *matrix = new S21Matrix(5, 5);
    ASSERT_EQ(matrix->getColumns(), 5);
    ASSERT_EQ(matrix->getRows(), 5);
    delete matrix;
}

TEST(COMPARSION, EQUAL5X5) {
    S21Matrix matrix1(mat_raw5x5, 5, 5);
    S21Matrix matrix2(mat_raw5x5, 5, 5);
    ASSERT_MATRIX(matrix1, matrix2);
    ASSERT_TRUE(matrix1 == matrix2);
}

TEST(COMPARSION, EQUAL5X5_2) {
    S21Matrix matrix1(mat_raw5x5_2, 5, 5);
    S21Matrix matrix2(mat_raw5x5_2, 5, 5);
    ASSERT_MATRIX(matrix1, matrix2);
    ASSERT_TRUE(matrix1 == matrix2);
    ASSERT_FALSE(matrix1 != matrix2);
}

TEST(COMPARSION, EQUAL2X6) {
    S21Matrix matrix1(mat_raw2x6, 2, 6);
    S21Matrix matrix2(mat_raw2x6, 2, 6);
    ASSERT_MATRIX(matrix1, matrix2);
    ASSERT_TRUE(matrix1 == matrix2);
    ASSERT_FALSE(matrix1 != matrix2);
}

TEST(COMPARSION, EQUAL2X6_2) {
    S21Matrix matrix1(mat_raw2x6_2, 2, 6);
    S21Matrix matrix2(mat_raw2x6_2, 2, 6);
    ASSERT_MATRIX(matrix1, matrix2);
    ASSERT_TRUE(matrix1 == matrix2);
    ASSERT_FALSE(matrix1 != matrix2);
}

TEST(COMPARSION, EQUAL_EQUAL_ZEROED) {
    S21Matrix matrix1(2, 2);
    S21Matrix matrix2(2, 2);
    ASSERT_TRUE(matrix1 == matrix2);
    ASSERT_FALSE(matrix1 != matrix2);
}

TEST(COMPARSION, EQUAL_NONEQUAL_ZEROED) {
    S21Matrix matrix1(5, 5);
    S21Matrix matrix2(5, 6);
    ASSERT_TRUE(matrix1 != matrix2);
    ASSERT_FALSE(matrix1 == matrix2);
}

TEST(COMPARSION, EQUAL5X5_NONEQUAL) {
    S21Matrix matrix1(mat_raw5x5, 5, 5);
    S21Matrix matrix2(mat_raw5x5_2, 5, 5);
    ASSERT_TRUE(matrix1 != matrix2);
    ASSERT_FALSE(matrix1 == matrix2);
}

TEST(ARITHMETICS, ADDITION_NORMAL) {
    S21Matrix matrix1(mat_raw5x5, 5, 5);
    S21Matrix matrix2(mat_raw5x5_2, 5, 5);

    S21Matrix correct(raw_5x5_sum, 5, 5);
    S21Matrix result  = matrix1 + matrix2;
    ASSERT_TRUE(result == correct);
}

TEST(ARITHMETICS, ADDITION_NORMAL_ASSIGN) {
    S21Matrix matrix1(mat_raw5x5, 5, 5);
    S21Matrix matrix2(mat_raw5x5_2, 5, 5);

    S21Matrix correct(raw_5x5_sum, 5, 5);
    matrix1 += matrix2;
    ASSERT_TRUE(matrix1 == correct);
}

TEST(ARITHMETICS, ADDITION_EXCEPTION) {
    S21Matrix matrix1(mat_raw5x5, 5, 5);
    S21Matrix matrix2(mat_raw2x6, 2, 6);

    EXPECT_THROW({
        S21Matrix result  = matrix1 + matrix2;
    }, std::out_of_range);
}

TEST(ARITHMETICS, SUBSTRACTION_NORMAL) {
    S21Matrix matrix1(mat_raw2x6, 2, 6);
    S21Matrix matrix2(mat_raw2x6_2, 2, 6);

    S21Matrix correct(raw_2x6_sub, 2, 6);
    S21Matrix result  = matrix1 - matrix2;
    ASSERT_TRUE(result == correct);
}

TEST(ARITHMETICS, SUBSTRACTION_NORMAL_ASSIGN) {
    S21Matrix matrix1(mat_raw2x6, 2, 6);
    S21Matrix matrix2(mat_raw2x6_2, 2, 6);

    S21Matrix correct(raw_2x6_sub, 2, 6);
    matrix1 -= matrix2;
    ASSERT_TRUE(matrix1 == correct);
}

TEST(ARITHMETICS, SUBSTRACTION_EXCEPTION) {
    S21Matrix matrix1(mat_raw5x5, 5, 5);
    S21Matrix matrix2(mat_raw2x6, 2, 6);

    EXPECT_THROW({
        S21Matrix result  = matrix1 - matrix2;
    }, std::out_of_range);
}

TEST(ARITHMETICS, MULTIPLICATION_NUMBER_NORMAL) {
    S21Matrix matrix1(mat_raw5x5, 5, 5);

    S21Matrix correct(raw_5x5_mult_by_5, 5, 5);
    S21Matrix result  = matrix1 * 5;
    ASSERT_TRUE(result == correct);
    result  = 5 * matrix1;
    ASSERT_TRUE(result == correct);
}

TEST(ARITHMETICS, MULTIPLICATION_NUMBER_NORMAL_ASSIGN) {
    S21Matrix matrix1(mat_raw5x5, 5, 5);

    S21Matrix correct(raw_5x5_mult_by_5, 5, 5);
    matrix1 *= 5;
    ASSERT_TRUE(matrix1 == correct);
}

TEST(ARITHMETICS, MULTIPLICATION_NUMBER_ZERO) {
    S21Matrix matrix1(mat_raw5x5, 5, 5);
    S21Matrix correct(5, 5);

    S21Matrix result  = matrix1 * 0;
    ASSERT_TRUE(result == correct);
}

TEST(ARITHMETICS, MULTIPLICATION_NORMAL) {
    S21Matrix matrix1(mat_raw5x5, 5, 5);
    S21Matrix matrix2(mat_raw5x5_2, 5, 5);

    S21Matrix correct(raw_5x5_mult, 5, 5);
    S21Matrix result  = matrix1 * matrix2;
    ASSERT_TRUE(result == correct);
}

TEST(ARITHMETICS, MULTIPLICATION_NORMAL_ASSIGN) {
    S21Matrix matrix1(mat_raw5x5, 5, 5);
    S21Matrix matrix2(mat_raw5x5_2, 5, 5);

    S21Matrix correct(raw_5x5_mult, 5, 5);
    matrix1 *= matrix2;
    ASSERT_TRUE(matrix1 == correct);
}

TEST(ARITHMETICS, MULTIPLICATION_ZERO) {
    S21Matrix matrix1(mat_raw5x5, 5, 5);
    S21Matrix zero(5, 5);
    S21Matrix correct(5, 5);

    S21Matrix result  = matrix1 * zero;
    ASSERT_TRUE(result == correct);
}

TEST(ARITHMETICS, MULTIPLICATION_EXCEPTION) {
    S21Matrix matrix1(5, 3);
    S21Matrix matrix2(4, 5);

    EXPECT_THROW({
        S21Matrix result  = matrix1 * matrix2;
    }, std::runtime_error);
}

TEST(MISC, TRANSPOSE_NORMAL) {
    S21Matrix matrix1(mat_raw3x5, 3, 5);
    S21Matrix correct(raw_5x3_transpose, 5, 3);

    S21Matrix result = matrix1.transpose();
    ASSERT_TRUE(result == correct);
}

TEST(MISC, COMPLEMENT_NORMAL) {
    S21Matrix matrix1(mat_raw3x3, 3, 3);
    S21Matrix correct(raw_3x3_compl, 3, 3);

    S21Matrix result = matrix1.calc_complements();
    ASSERT_TRUE(result == correct);
}

TEST(MISC, COMPLEMENT_EXCEPTION) {
    S21Matrix matrix1(mat_raw2x6, 2, 6);

    EXPECT_THROW({
        S21Matrix result = matrix1.calc_complements();
    }, std::runtime_error);
}

TEST(MISC, DETERMINANT_NORMAL) {
    S21Matrix matrix1(mat_raw3x3_2, 3, 3);
    ASSERT_DOUBLE_EQ(matrix1.determinant(), 829);
}

TEST(MISC, DETERMINANT_SINGLE) {
    double arr[] = {5};
    S21Matrix matrix1(arr, 1, 1);
    ASSERT_DOUBLE_EQ(matrix1.determinant(), 5);
}

TEST(MISC, DETERMINANT_EXCEPTION) {
    S21Matrix matrix1(mat_raw2x6, 2, 6);

    EXPECT_THROW({
        matrix1.determinant();
    }, std::runtime_error);
}

TEST(MISC, INVERSE_NORMAL) {
    S21Matrix matrix1(mat_raw3x3_3, 3, 3);
    S21Matrix correct(raw_3x3_inverse, 3, 3);

    S21Matrix result = matrix1.inverse_matrix();
    ASSERT_TRUE(result == correct);
}

TEST(MISC, INVERSE_SINGLE) {
    double arr[] = {5};
    S21Matrix matrix1(arr, 1, 1);
    double arr2[] = {0.2};
    S21Matrix correct(arr2, 1, 1);
    S21Matrix result = matrix1.inverse_matrix();
    ASSERT_TRUE(result == correct);
}

TEST(MISC, INVERSE_EXCEPTION) {
    S21Matrix matrix1(mat_raw3x3_4, 3, 3);

    EXPECT_THROW({
        S21Matrix result = matrix1.inverse_matrix();
    }, std::runtime_error);
}

TEST(MISC, SET_ROWS) {
    S21Matrix matrix1(3, 5);
    matrix1.setRows(5);
    ASSERT_EQ(matrix1.getRows(), 5);
}

TEST(MISC, SET_COLUMNS) {
    S21Matrix matrix1(5, 3);
    matrix1.setColumns(5);
    ASSERT_EQ(matrix1.getColumns(), 5);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
