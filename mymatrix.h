/*mymatrix.h*/

// 
// Gurleen Kaur
// U. of Illinois, Chicago
// CS 251: Spring 2020
// Project #02
//

//
// mymatrix
//
// The mymatrix class provides a matrix (2D array) abstraction.
// The size can grow dynamically in both directions (rows and 
// cols).  Also, rows can be "jagged" --- i.e. rows can have 
// different column sizes, and thus the matrix is not necessarily 
// rectangular.  All elements are initialized to the default value
// for the given type T.  Example:
//
//   mymatrix<int>  M;  // 4x4 matrix, initialized to 0
//   
//   M(0, 0) = 123;
//   M(1, 1) = 456;
//   M(2, 2) = 789;
//   M(3, 3) = -99;
//
//   M.growcols(1, 8);  // increase # of cols in row 1 to 8
//
//   for (int r = 0; r < M.numrows(); ++r)
//   {
//      for (int c = 0; c < M.numcols(r); ++c)
//         cout << M(r, c) << " ";
//      cout << endl;
//   }
//
// Output:
//   123 0 0 0
//   0 456 0 0 0 0 0 0
//   0 0 789 0
//   0 0 0 -99
//

#pragma once

#include <iostream>
#include <exception>
#include <stdexcept>

using namespace std;

template<typename T>
class mymatrix
{
private:
  struct ROW
  {
    T*  Cols;     // dynamic array of column elements
    int NumCols;  // total # of columns (0..NumCols-1)
  };
  ROW* Rows;     // dynamic array of ROWs
  int  NumRows;  // total # of rows (0..NumRows-1)

public:
  //
  // default constructor:
  //
  // Called automatically by C++ to construct a 4x4 matrix.  All 
  // elements are initialized to the default value of T.
  //
  mymatrix()
  {
    Rows = new ROW[4];  // an array with 4 ROW structs:
    NumRows = 4;

    // initialize each row to have 4 columns:
    for (int r = 0; r < NumRows; ++r)
    {
        Rows[r].Cols = new T[4];  // an array with 4 elements of type T:
        Rows[r].NumCols = 4;

      // initialize the elements to their default value:
        for (int c = 0; c < Rows[r].NumCols; ++c)
        {
            Rows[r].Cols[c] = T{};  // default value for type T:
        }
    }//for (int
  }//mymatrix()

  //
  // parameterized constructor:
  //
  // Called automatically by C++ to construct a matrix with R rows, 
  // where each row has C columns. All elements are initialized to 
  // the default value of T.
  //
  mymatrix(int R, int C)
  {
    Rows = new ROW[R];  // an array with R ROW structs:
    NumRows = R;

    if (R < 1)
      throw invalid_argument("mymatrix::constructor: # of rows");
    if (C < 1)
      throw invalid_argument("mymatrix::constructor: # of cols");
  
    // initialize each row to have C columns:
    for (int r = 0; r < NumRows; ++r)
    {
        Rows[r].Cols = new T[C];  // an array with R elements of type T:
        Rows[r].NumCols = C;

        // initialize the elements to their default value:
        for (int c = 0; c < Rows[r].NumCols; ++c)
        {
            Rows[r].Cols[c] = T{};  // default value for type T:
        }
     }//for (int
  }//mymatrix(int R


  //
  // copy constructor:
  //
  // Called automatically by C++ to construct a matrix that contains a 
  // copy of an existing matrix.  Example: this occurs when passing 
  // mymatrix as a parameter by value
  //
  //   void somefunction(mymatrix<int> M2)  <--- M2 is a copy:
  //   { ... }
  //
  mymatrix(const mymatrix<T>& other)
  {
    
    Rows = new ROW[other.numrows()];  // an array with R ROW structs:
    NumRows = other.numrows();
    
    for (int r = 0; r < NumRows; ++r)
    {
        Rows[r].Cols = new T[other.numcols(r)];  // an array with R elements of type T:
        Rows[r].NumCols = other.numcols(r);

        // initialize the elements to their default value:
        for (int c = 0; c < Rows[r].NumCols; ++c)
        {
            Rows[r].Cols[c] = other.Rows[r].Cols[c];  // default value for type T:
        }
    }//for (int
  }//mymatrix(const


  //
  // numrows
  //
  // Returns the # of rows in the matrix.  The indices for these rows
  // are 0..numrows-1.
  //
  int numrows() const
  {
    return NumRows;
  }//int numrows
  

  //
  // numcols
  //
  // Returns the # of columns in row r.  The indices for these columns
  // are 0..numcols-1.  Note that the # of columns can be different 
  // row-by-row since "jagged" rows are supported --- matrices are not
  // necessarily rectangular.
  //
  int numcols(int r) const
  {
    if (r < 0 || r >= NumRows)
      throw invalid_argument("mymatrix::numcols: row");

    return this->Rows[r].NumCols;             //return num of cols in rows r
  }//int numcols


  //
  // growcols
  //
  // Grows the # of columns in row r to at least C.  If row r contains 
  // fewer than C columns, then columns are added; the existing elements
  // are retained and new locations are initialized to the default value 
  // for T.  If row r has C or more columns, then all existing columns
  // are retained -- we never reduce the # of columns.
  //
  // Jagged rows are supported, i.e. different rows may have different
  // column capacities -- matrices are not necessarily rectangular.
  //
  void growcols(int r, int C)
  {
    T* newA = new T[C];                          // Creating new array

    if (r < 0 || r >= NumRows)
      throw invalid_argument("mymatrix::growcols: row");
    if (C < 1)
      throw invalid_argument("mymatrix::growcols: columns");
    if (C <= Rows[r].NumCols)                    // testing if Num of cols is smaller than the value of C passed then return
        return;
            
    for (int c = 0; c < Rows[r].NumCols; ++c){   // storing the values of Cols in new array
        newA[c] = Rows[r].Cols[c];
    }
    
    for (int c = Rows[r].NumCols; c < C; ++c)    // setting newA to default value
    {
        newA[c] = T{};                           // default value for type T:
    }
    
    delete[] Rows[r].Cols;                       // delete the old matrix
    Rows[r].Cols = newA;                         // storing the value of newA in old matrix
    Rows[r].NumCols = C;                         // setting the num of cols equal to C
  }//void growcols(


  //
  // grow
  //
  // Grows the size of the matrix so that it contains at least R rows,
  // and every row contains at least C columns.
  // 
  // If the matrix contains fewer than R rows, then rows are added
  // to the matrix; each new row will have C columns initialized to 
  // the default value of T.  If R <= numrows(), then all existing
  // rows are retained -- we never reduce the # of rows.
  //
  // If any row contains fewer than C columns, then columns are added
  // to increase the # of columns to C; existing values are retained
  // and additional columns are initialized to the default value of T.
  // If C <= numcols(r) for any row r, then all existing columns are
  // retained -- we never reduce the # of columns.
  // 
  void grow(int R, int C)
  {
    if (R < 1)
      throw invalid_argument("mymatrix::grow: # of rows");
    if (C < 1)
      throw invalid_argument("mymatrix::grow: # of cols");
    if(R < NumRows){
        R = NumRows;
    }

    mymatrix<T> incRow(R, C);                               // creating new matrix incRow with dimensions R and C

    for(int r = 0; r < this->NumRows; ++r)
    {
        if(incRow.Rows[r].NumCols < Rows[r].NumCols)        // if new matrix Colums are smaller than original matrix
        {
            incRow.Rows[r].Cols = new T[Rows[r].NumCols];   // create new Cols
            incRow.Rows[r].NumCols = Rows[r].NumCols;       // and store the old to incRow
        }
        for (int c = 0; c < Rows[r].NumCols; ++c)           
        {
            incRow.Rows[r].Cols[c] = Rows[r].Cols[c];       // setting cols to new matrix
        }
        for(int c = Rows[r].NumCols; c<C; c++)
            incRow.Rows[r].Cols[c] = T{};                   // setting the rest of cols to default type T
    }
    
    *this = incRow;                                         // setting the incRow to original matrix
    
  }//void grow(


  //
  // size
  //
  // Returns the total # of elements in the matrix.
  //
  int size() const
  {
  
    int numElements = 0;
    
    for(int r = 0; r < NumRows; ++r)
    {
        for (int c = 0; c < Rows[r].NumCols; ++c)
        {
            numElements++;                           // incrementing numElements as it goes through the whole matrix
        }
    }
    
    return numElements;                              // return numElements
  }//int size


  //
  // at
  //
  // Returns a reference to the element at location (r, c); this
  // allows you to access the element or change it:
  //
  //    M.at(r, c) = ...
  //    cout << M.at(r, c) << endl;
  //
  T& at(int r, int c)
  {
    if (r < 0 || r >= NumRows)
      throw invalid_argument("mymatrix::at: row");
    if (c < 0 || c >= Rows[r].NumCols)
      throw invalid_argument("mymatrix::at: col");

    return this->Rows[r].Cols[c];                          // return reference to element at location r,c
  }// T& at


  //
  // ()
  //
  // Returns a reference to the element at location (r, c); this
  // allows you to access the element or change it:
  //
  //    M(r, c) = ...
  //    cout << M(r, c) << endl;
  //
  T& operator()(int r, int c)
  {
    if (r < 0 || r >= NumRows)
      throw invalid_argument("mymatrix::(): row");
    if (c < 0 || c >= Rows[r].NumCols)
      throw invalid_argument("mymatrix::(): col");

    return Rows[r].Cols[c];                             // return element at r,c
  }//T& operator

  //
  // scalar multiplication
  //
  // Multiplies every element of this matrix by the given scalar value,
  // producing a new matrix that is returned.  "This" matrix is not
  // changed.
  //
  // Example:  M2 = M1 * 2;
  //
  mymatrix<T> operator*(T scalar)
  {
    mymatrix<T> solution(*this);                                  // creating new matrix solution by passing original matrix
    
    for(int r = 0; r < this->NumRows; ++r)
    {
        for (int c = 0; c < Rows[r].NumCols; ++c)
        {
            solution.Rows[r].Cols[c] = scalar * Rows[r].Cols[c];    // setting solution matrix equal to scalar times element stored in r,c
        }
    }

    return solution;                                              // return solution matrix
  }//mymatrix<T> operator


  //
  // matrix multiplication
  //
  // Performs matrix multiplication M1 * M2, where M1 is "this" matrix and
  // M2 is the "other" matrix.  This produces a new matrix, which is returned.
  // "This" matrix is not changed, and neither is the "other" matrix.
  //
  // Example:  M3 = M1 * M2;
  //
  // NOTE: M1 and M2 must be rectangular, if not an exception is thrown.  In
  // addition, the sizes of M1 and M2 must be compatible in the following sense:
  // M1 must be of size RxN and M2 must be of size NxC.  In this case, matrix
  // multiplication can be performed, and the resulting matrix is of size RxC.
  //
  mymatrix<T> operator*(const mymatrix<T>& other)
  {
    //mymatrix<T> result;
    int removeZero;
        
    for(int r = 0; r < NumRows; ++r)
    {
        if (Rows[0].NumCols != Rows[r].NumCols)                             // if # of cols at row 0 is not equal to cols at different rows
           throw runtime_error("mymatrix::*: this not rectangular");        // return throw statement
           
        if (this->Rows[0].NumCols != other.numrows())                       // this cols at rows 0 is not equal to other num of rows
           throw runtime_error("mymatrix::*: size mismatch");               // return throw statement
    }
    
    for (int c = 0; c < other.NumRows; ++c)
    {
        if (other.Rows[0].NumCols != other.Rows[c].NumCols)                 // if # of other cols at row 0 is not equal to cols at diff row 
           throw runtime_error("mymatrix::*: other not rectangular");       // return throw statement
    }
    

    for (int c = 0; c < other.NumRows; ++c)
    {
        removeZero = other.numcols(c);                                       // set the reference to values of other array to removeZero
    }
    
    mymatrix<T> solution(NumRows, removeZero);                               // setting new matrix with dimensions NumRowsm removeZero 
    //
    // Okay, we can multiply:
    //
    for(int r = 0; r < this->NumRows; ++r)
    {
        for (int c = 0; c < Rows[0].NumCols; ++c)
        {
            solution.Rows[r].Cols[c] = 0;                                     // initializing the value of solution matrix to 0
            for (int i=0; i< other.NumRows; ++i)
            {
                // multiply other matrix cols with the rows of this matrix and adding it to solution matrix
                solution.Rows[r].Cols[c] += other.Rows[i].Cols[c] * this->Rows[r].Cols[i]; 
            }
        }
    }
    return solution;                                                             // return solution
  }//mymatrix<T> operator*


  //
  // _output
  //
  // Outputs the contents of the matrix; for debugging purposes.
  //
  void _output()
  {
    for (int r = 0; r < this->NumRows; ++r)
    {
        for (int c = 0; c < this->Rows[r].NumCols; ++c)
        {
            cout << this->Rows[r].Cols[c] << " ";
        }
        cout << endl;
    }
  }//void _output

};//class mymatrix
