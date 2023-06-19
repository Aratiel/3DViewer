// Copyright <lwolmer> 2022
#include "s21_matrix_oop.h"
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <cmath>

namespace s21 {
S21Matrix::S21Matrix() {
    _columns = 4;
    _rows = 4;
    _matrix = AllocateMemory(_rows, _columns);
}

S21Matrix::S21Matrix(unsigned int rows, unsigned int cols) : _rows(rows), _columns(cols) {
    if (rows <= 0 || cols <= 0) {
        throw std::invalid_argument("Can't accept negative or zero dimensions");
    }
    _matrix = AllocateMemory(_rows, _columns);
}

S21Matrix::S21Matrix(double* raw_data, unsigned int rows, unsigned int cols) : S21Matrix(rows, cols) {
    for (unsigned int i = 0; i < _rows; i++) {
        std::copy(&raw_data[i * cols], &raw_data[i * cols + cols], _matrix[i]);
    }
}

double** S21Matrix::AllocateMemory(unsigned int rows, unsigned int cols) const {
    double** memory = new double*[rows]();
    if (memory == NULL)
        throw std::bad_alloc();
    for (unsigned int i = 0; i < rows; i++) {
        memory[i] = new double[cols]{0.0f};
        if (memory[i] == NULL)
            throw std::bad_alloc();
    }
    return memory;
}

S21Matrix::S21Matrix(const S21Matrix& other) {
    _columns = other.getColumns();
    _rows = other.getRows();
    _matrix = AllocateMemory(_rows, _columns);
    for (unsigned int i = 0; i < _rows; i++) {
        std::copy(other._matrix[i], other._matrix[i] + _columns, _matrix[i]);
    }
}


S21Matrix::S21Matrix(S21Matrix&& other) {
    _matrix = other._matrix;
    _columns = other.getColumns();
    _rows = other.getRows();
    other._matrix = NULL;
}


S21Matrix::~S21Matrix() {
    DeleteMatrix();
}

void S21Matrix::DeleteMatrix() {
    if (_matrix) {
        for (unsigned int i = 0; i < _rows; i++) {
            if (_matrix[i])
                delete [] _matrix[i];
        }
        delete [] _matrix;
    }
}

bool S21Matrix::eq_matrix(const S21Matrix& other) {
    if (!(_columns == other._columns && _rows == other._rows))
        return false;
    bool is_equal = true;
    for (unsigned int i = 0; i < _rows && is_equal; i++) {
        for (unsigned int j = 0; j < _columns && is_equal; j++) {
            if (fabs(_matrix[i][j] - other._matrix[i][j]) > CRIPPLE_EPS)
                is_equal = false;
        }
    }
    return is_equal;
}

void S21Matrix::SumSubMatrix(const S21Matrix& other, bool substruct) {
    if (_rows != other._rows || _columns != other._columns) {
        throw std::out_of_range("Rows or Columns are zero or less");
    }
    for (unsigned int i = 0; i < _rows; i++) {
        for (unsigned int j = 0; j < _columns; j++) {
            if (!substruct)
                _matrix[i][j] = _matrix[i][j] + other._matrix[i][j];
            else
                _matrix[i][j] = _matrix[i][j] - other._matrix[i][j];
        }
    }
}


void S21Matrix::sum_matrix(const S21Matrix& other) {
    SumSubMatrix(other, false);
}


void S21Matrix::sub_matrix(const S21Matrix& other) {
    SumSubMatrix(other, true);
}

void S21Matrix::mul_number(const double num) {
    for (unsigned int i = 0; i < _rows; i++) {
        for (unsigned int j = 0; j < _columns; j++) {
            _matrix[i][j] = _matrix[i][j] * num;
        }
    }
}


void S21Matrix::mul_matrix(const S21Matrix& other) {
    if (_columns != other.getRows())
        throw std::runtime_error("First Matrix columns amount is not equal second matrix rows amount");
    _columns = other.getColumns();
    double **new_matrix = AllocateMemory(_rows, _columns);
    for (unsigned int i = 0; i < _rows; i++) {
        for (unsigned int j = 0; j < other.getColumns(); j++) {
            for (unsigned int k = 0; k < other.getRows(); k++) {
                new_matrix[i][j] += _matrix[i][k] * other._matrix[k][j];
            }
        }
    }
    DeleteMatrix();
    _matrix = new_matrix;
}


S21Matrix S21Matrix::transpose() {
    S21Matrix trans = S21Matrix(_columns, _rows);
    for (unsigned int i = 0; i < _rows; i++)
        for (unsigned int j = 0; j < _columns; j++) {
            trans._matrix[j][i] = _matrix[i][j];
    }
    return trans;
}

S21Matrix S21Matrix::GetMinorMatrix(unsigned int row, unsigned int column) {
    S21Matrix minor = S21Matrix(_rows - 1, _columns - 1);
    unsigned int minor_i = 0, minor_j = 0;
    for (unsigned int i = 0; i < _rows; i++) {
        if (i != row) {
            for (unsigned int j = 0; j < _columns; j++) {
                if (j != column) {
                    minor._matrix[minor_i][minor_j] = _matrix[i][j];
                    minor_j++;
                }
            }
            minor_i++;
        }
        minor_j = 0;
    }
    return minor;
}



S21Matrix S21Matrix::calc_complements() {
    if (!IsSquare())
        throw std::runtime_error("Matrix is not square");
    S21Matrix result(_rows, _columns);
    for (unsigned int i = 0; i < _rows; i++) {
        for (unsigned int j = 0; j < _columns; j++) {
            S21Matrix minor_matrix = GetMinorMatrix(i, j);
            result._matrix[i][j] += (((i + j) & 1) ? -1 : 1) * minor_matrix.determinant();
        }
    }
    return result;
}


double S21Matrix::determinant() {
    if (!IsSquare())
        throw std::runtime_error("Matrix is not square");
    double result = 0;
    if (_rows == 1) {
        return _matrix[0][0];
    } else if (_rows == 2) {
        result = _matrix[0][0] * _matrix[1][1] - _matrix[1][0] * _matrix[0][1];
    } else {
        for (unsigned int i = 0; i < _columns; i++) {
            S21Matrix minor_matrix = GetMinorMatrix(0, i);
            result += _matrix[0][i] * ((i & 1) ? -1 : 1) * minor_matrix.determinant();
        }
    }
    return result;
}


S21Matrix S21Matrix::inverse_matrix() {
    double determ = determinant();
    if (determ < CRIPPLE_EPS)
        throw std::runtime_error("Can't calculate inverse matrix, determinant is zero.");

    S21Matrix result = S21Matrix(_rows, _columns);
    if (_rows == 1) {
        result._matrix[0][0] = 1 / determ;
    } else {
        S21Matrix complem = calc_complements();
        S21Matrix trans = complem.transpose();
        trans.mul_number(1 / determ);
        result = trans;
    }
    return result;
}


S21Matrix& S21Matrix::operator=(const S21Matrix& other) {
    _columns = other.getColumns();
    _rows = other.getRows();
    double** new_mat = other.GetResizedMatrix(_rows, _columns);
    DeleteMatrix();
    _matrix = new_mat;
    return *this;
}


S21Matrix S21Matrix::operator+(const S21Matrix& other) {
    S21Matrix result = *this;
    result.sum_matrix(other);
    return result;
}


S21Matrix S21Matrix::operator-(const S21Matrix& other) {
    S21Matrix result = *this;
    result.sub_matrix(other);
    return result;
}


S21Matrix S21Matrix::operator*(const S21Matrix& other) {
    S21Matrix result = *this;
    result.mul_matrix(other);
    return result;
}


S21Matrix S21Matrix::operator*(const double dbl) {
    S21Matrix result = *this;
    result.mul_number(dbl);
    return result;
}


bool S21Matrix::operator==(const S21Matrix& other) {
    return this->eq_matrix(other);
}

bool S21Matrix::operator!=(const S21Matrix& other) {
    return !this->eq_matrix(other);
}

S21Matrix& S21Matrix::operator+=(const S21Matrix& other) {
    this->sum_matrix(other);
    return *this;
}


S21Matrix& S21Matrix::operator-=(const S21Matrix& other) {
    this->sub_matrix(other);
    return *this;
}


S21Matrix& S21Matrix::operator*=(const S21Matrix& other) {
    this->mul_matrix(other);
    return *this;
}


S21Matrix& S21Matrix::operator*=(const double dbl) {
    this->mul_number(dbl);
    return *this;
}


double& S21Matrix::operator() (unsigned row, unsigned column) const {
    return _matrix[row][column];
}

void S21Matrix::setRows(const unsigned int new_rows) {
    double** new_mat = GetResizedMatrix(new_rows, _columns);
    DeleteMatrix();
     _matrix = new_mat;
    _rows = new_rows;
}

void S21Matrix::setColumns(const unsigned int new_columns) {
    double** new_mat = GetResizedMatrix(_rows, new_columns);
    DeleteMatrix();
    _matrix = new_mat;
    _columns = new_columns;
}


bool S21Matrix::IsSquare() {
    return _columns == _rows ? true : false;
}

double** S21Matrix::GetResizedMatrix(unsigned int rows, unsigned int columns) const {
    double** new_mat = AllocateMemory(rows, columns);
    for (unsigned int i = 0; i < _rows && i < rows; i++) {
        for (unsigned int j = 0; j < _columns && i < columns; j++) {
            new_mat[i][j] += _matrix[i][j];
        }
    }
    return new_mat;
}

void S21Matrix::Print() {
    std::cout << std::left;
    for (unsigned int i = 0; i < _rows; i++) {
        for (unsigned int j = 0; j < _columns; j++) {
            std::cout  << std::setw(8) << _matrix[i][j];
        }
        std::cout << std::endl;
    }
}

S21Matrix S21Matrix::GetIdentityMatrix(const unsigned int size) {
    S21Matrix result(size, size);
    for (unsigned int i = 0; i < size; i++) {
        result(i, i) = 1;
    }
    return result;
}

S21Matrix operator*(const double dbl, const S21Matrix& other) {
    S21Matrix result = other;
    result.mul_number(dbl);
    return result;
}
}  // namespace S21
