// Copyright <lwolmer> 2022
#ifndef SRC_S21_MATRIX_OOP_H_
#define SRC_S21_MATRIX_OOP_H_

#define CRIPPLE_EPS 1e-7

namespace s21 {
class S21Matrix {
 public:
  /**
      @brief A basic constructor that initialises a matrix of some predefined
     dimension (4x4)
  */
  S21Matrix();

  /**
      @brief Parametrized constructor with number of rows and columns
      @param rows Rows in the matrix
      @param cols Columns in the matrix
  */
  S21Matrix(unsigned int rows, unsigned int cols);
  /**
      @brief Construct a new S21Matrix object using raw data from array of
     doubles

      @param raw_data array of doubles with size (rows * columns)
      @param rows number of rows
      @param cols number of columns
  */
  S21Matrix(double* raw_data, unsigned int rows, unsigned int cols);

  /**
      @brief Copy constructor
      @param other Matrix class to copy from
  */
  S21Matrix(const S21Matrix& other);

  /**
      @brief Move constructor
      @param other Matrix class to copy from
  */
  S21Matrix(S21Matrix&& other);

  /**
      @brief Destroy the S21Matrix object
  */
  ~S21Matrix();

  /**
      @brief Checks matrices for equality with each other
  */
  bool eq_matrix(const S21Matrix& other);

  /**
      @brief Adds the second matrix to the current one
      @throw Different matrix dimensions
  */
  void sum_matrix(const S21Matrix& other);

  /**
      @brief Subtracts another matrix from the current one
      @throw Different matrix dimensions
  */
  void sub_matrix(const S21Matrix& other);

  /**
      @brief Multiplies the current matrix by a number
  */
  void mul_number(const double num);

  /**
      @brief Multiplies the current matrix by the second matrix
      @throw The number of columns of the first matrix is not equal to the
     number of rows of the second matrix
  */
  void mul_matrix(const S21Matrix& other);

  /**
      @brief Creates a new transposed matrix from the current one and returns it
  */
  S21Matrix transpose();

  /**
      @brief Calculates the algebraic addition matrix of the current one and
     returns it
      @throw The matrix is not square
  */
  S21Matrix calc_complements();

  /**
      @brief Calculates and returns the determinant of the current matrix
      @throw the matrix is not square
      @return double : Determinant of the matrix
  */
  double determinant();

  /**
      @brief Calculates and returns the inverse matrix
      @throw Matrix determinant is 0
  */
  S21Matrix inverse_matrix();

  /**
      @brief Assignment of values from one matrix to another one

      @param other Matrix to copy from
      @return S21Matrix& Ref to modified object
  */
  S21Matrix& operator=(const S21Matrix& other);

  /**
      @brief Addition of two matrices
      @throw different matrix dimensions

      @param other Other matrix
      @return S21Matrix : result of operation
  */
  S21Matrix operator+(const S21Matrix& other);

  /**
      @brief Subtraction of one matrix from another
      @throw different matrix dimensions

      @param other Other matrix
      @return S21Matrix : result of operation
  */
  S21Matrix operator-(const S21Matrix& other);

  /**
      @brief Matrix multiplication and matrix multiplication by a number
      @throw the number of columns of the first matrix does not equal the number
     of rows of the second matrix
      @param other Other matrix
      @return S21Matrix : result of operation
  */
  S21Matrix operator*(const S21Matrix& other);

  /**
      @brief Matrix multiplication and matrix multiplication by a number
      @param dbl Double to multiply with
      @return S21Matrix : result of operation
  */
  S21Matrix operator*(const double dbl);

  /**
      @brief Checks for matrices equality (eq_matrix)
      @param other Other matrix
      @return bool : True if equal
  */
  bool operator==(const S21Matrix& other);

  /**
      @brief Checks for matrices inequality (eq_matrix)
      @param other Other matrix
      @return bool : True if not equal
  */
  bool operator!=(const S21Matrix& other);

  /**
      @brief Addition assignment (sum_matrix)
      @throw Different matrix dimensions
      @param other Other matrix
      @return S21Matrix& Ref to modified object
  */
  S21Matrix& operator+=(const S21Matrix& other);

  /**
      @brief Addition assignment (sum_matrix)
      @throw different matrix dimensions
      @param other Other matrix
      @return S21Matrix& Ref to modified object
  */
  S21Matrix& operator-=(const S21Matrix& other);

  /**
      @brief Multiplication assignment with matrix
      @throw the number of columns of the first matrix does not equal the number
     of rows of the second matrix
      @param other Other matrix
      @return S21Matrix& Ref to modified object
  */
  S21Matrix& operator*=(const S21Matrix& other);

  /**
      @brief Multiplication assignment with double
      @param dbl Double to multiply with
      @return S21Matrix& Ref to modified object
  */
  S21Matrix& operator*=(const double dbl);

  /**
      @brief Indexation by matrix elements (row, column)
      @throw index is outside the matrix
      @param row index of the row
      @param column index of the column
      @return double& : Resulting value by given index
  */
  double& operator()(unsigned row, unsigned column) const;

  /**
      @brief Sets the rows field and reallocs matrix accordingly
      @param new_rows New amount of rows
  */
  void setRows(const unsigned int new_rows);

  /**
      @brief Sets the columns field and reallocs matrix accordingly

      @param new_columns
  */
  void setColumns(const unsigned int new_columns);

  /**
      @brief Get the rows field
      @return unsigned int : Rows
  */
  unsigned int getRows() const { return _rows; }

  /**
      @brief Get the columns field
      @return unsigned int : Columns
  */
  unsigned int getColumns() const { return _columns; }

  /**
      @brief Checks if matrix is square

      @return true if matrix is square,
      @return false otherwise
  */
  bool IsSquare();
  /**
      @brief Prints matrix

  */
  void Print();

  /**
      @brief Get the Identity Matrix object

      @param size
      @return S21Matrix
  */
  static S21Matrix GetIdentityMatrix(const unsigned int size);

 private:
  double** _matrix = {nullptr};
  unsigned int _rows = {0};
  unsigned int _columns = {0};

  /**
      @brief Allocates memory for matrix

      @param rows amount of rows
      @param cols amount of columns
      @return double** : returns dynamically allocated array of pointers,
      that points to arrays of doubles.
      With total size (sizeof(double*) * rows + (sizeof(double) * columns *
     rows))
  */
  double** AllocateMemory(unsigned int rows, unsigned int cols) const;

  /**
      @brief Helper method for addition and substraction, made to avoid code
     duplication

      @param other Other matrix used in operator
      @param substruct If true the operation will be substraction, otherwise
     addition
  */
  void SumSubMatrix(const S21Matrix& other, bool substruct);

  /**
      @brief Get minor matrix. Removes row and collumn with indices passed to
     method.

      @param row index of row to remove
      @param column index of column to remove
      @return S21Matrix : Result of removal
  */
  S21Matrix GetMinorMatrix(unsigned int row, unsigned int column);

  /**
      @brief Get the Resized matrix memory block

      @param rows new number of rows
      @param columns new number of columns
      @return double** : Dynnamically allocated matrix
  */

  double** GetResizedMatrix(unsigned int rows, unsigned int columns) const;
  /**
      @brief Frees memory used for storing matrix

  */

  void DeleteMatrix();
};

/**
    @brief Matrix multiplication by a number
    @param dbl Double to multiply with
    @param other Matrix to multiply with
    @return S21Matrix result of multiplcation
*/
S21Matrix operator*(const double dbl, const S21Matrix& other);
}  // namespace s21
#endif  // SRC_S21_MATRIX_OOP_H_
