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
    int userInput = 0;      // initialized userInput to 0
    mymatrix<int>  matrix;  // 4x4 matrix, initialized to 0
    mymatrix<int> solution; // new matrix solution
    int scalar;             // scalar value for multiply to a matrix
    
    while (userInput != 7){
        cout << "1. Enter 1 to display initial default matrix: " << endl
             << "2. Enter 2 to display values in matrix: " << endl
             << "3. Enter 3 to grow columns of matrix: " << endl
             << "4. Enter 4 to grow the whole matrix: " << endl
             << "5. Enter 5 to multiply matrix with a scalar: " << endl
             << "6: Enter 6 to multiply old and new matrices: " << endl
             << "7. Enter 7 to quit... ";
        
        cout << endl;
        cin >> userInput;    
        cout << endl;
        
        if(userInput == 1){
            matrix._output();                  // display the 4X4 with initial values 0
            cout << endl;
        }
        else if(userInput == 2){
            matrix(0, 0) = 123;                // passing the values to matrix
            matrix(1, 1) = 456;
            matrix(2, 2) = 789;
            matrix(3, 3) = -99;
            matrix._output();
    
            cout << endl;
        }
        else if(userInput == 3){
           matrix.growcols(1, 8);              // increase # of cols in row 1 to 8
           matrix._output(); 
           cout << endl;
        }
        else if(userInput == 4){
            matrix.grow(5, 5);                  // increase # of rows and cols by 5
            matrix._output();
            cout << endl;
        }
        else if(userInput == 5){
            cout << "Input the scalar you want to multiply with: ";
            cin >> scalar;
            
            solution = matrix * scalar;          // multiply matrix with a scalar
            solution._output();
            cout << endl;
        }
        else if(userInput == 6){
            cout << "Old matrix: " << endl;     
            matrix._output();
            
            cout << endl;
            
            cout << "New Matrix: " << endl;
            mymatrix <int> multiplyMatrix(4,3);
            multiplyMatrix(0, 0) = 1;
            multiplyMatrix(1, 1) = 2;
            multiplyMatrix(2, 2) = 3;
            multiplyMatrix(3, 2) = 4;
            
            multiplyMatrix._output();
            cout << endl;
            
            solution = matrix * multiplyMatrix;    // multiply two matrices
            solution._output();
            cout << endl;
        }//else if(userInput == 6
        
    }//while (userInput
    
    return 0;
}//int main()
