#pragma once

#include "functionGetMin.h"


int checkSquareCells(const int* m,int C,int row,int column,int side,int currentRow,int currentColumn){

    if(currentRow == row + side){

        return 1;

    }


    if(currentColumn == column + side){

        return checkSquareCells(m,C,row,column,side,currentRow + 1,column);

    }


    int TraverseIndex = currentRow * C + currentColumn;


    int currentValue = 1;


    if(m[TraverseIndex] == 0){

        currentValue = 0;

    }


    int remainingCells = checkSquareCells(
        m,C,row,column,side,currentRow,currentColumn + 1
    );


    if(currentValue == 0){

        return 0;

    }


    return remainingCells;

}


long long largest_ones_square_brute(const int* m,int R,int C){


    long long largestArea = 0;


    ////////////////////////////

    // traverse

    ////////////////////////////


    for(int i = 0 ; i < R ; i++){

        for(int j = 0 ; j < C ; j++){


            ////////////////////////////

            // Try every possible square size

            ////////////////////////////


            for(int k = 1 ; i + k <= R && j + k <= C ; k++){


                ////////////////////////////

                // Recursively verify ALL k^2 cells

                ////////////////////////////


                int AllValuesOne = checkSquareCells(
                    m,C,i,j,k,i,j
                );


                ////////////////////////////

                // If the complete square
                // contains only 1s

                ////////////////////////////


                if(AllValuesOne == 1){

                    long long area = (long long)k * k;


                    if(area > largestArea){

                        largestArea = area;

                    }

                }

            }

        }

    }


    return largestArea;

}