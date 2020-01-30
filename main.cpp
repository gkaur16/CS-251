//
// Project 1 - Spam Filter Program 
// Gurleen Kaur
//

#include <iostream>
#include <fstream>
#include <string>

#include "mymatrix.h"

using namespace std;

int  main()
{
    mymatrix<int>  matrix;  // 4x4 matrix, initialized to 0
    mymatrix<int> solution;
    int multiplyBy;
    
    matrix(0, 0) = 123;
    matrix(1, 1) = 456;
    matrix(2, 2) = 789;
    matrix(3, 3) = -99;
    matrix._output();
    
    cout << endl;
    
    mymatrix <int> multiplyMatrix(4,3);
    multiplyMatrix(0, 0) = 1;
    multiplyMatrix(1, 1) = 2;
    multiplyMatrix(2, 2) = 3;
    multiplyMatrix(3, 2) = 4;
    
//     multiplyMatrix._output();
//     cout << endl;
    
    matrix.growcols(1, 8);  // increase # of cols in row 1 to 8
    matrix._output();
        
    cout << endl;
//     solution = matrix * multiplyMatrix;
//     solution._output();
    
    matrix.grow(5, 5);
    matrix._output();
    
    cout << endl;
    
    matrix.growcols(4, 10);
    matrix._output();
    
//     cout << endl << "Enter the scalar number you want to mulitply the metrix by: ";
//     cin >> multiplyBy;
    
    return 0;
}
