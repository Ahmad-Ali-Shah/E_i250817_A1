#pragma once
#include "functionGetMin.h"





long long largest_ones_square(const int* m,int R,int C,int& side,int& top,int& left){


    ////////////////////////////


    // EDGE CASES 

    ////////////////////////////

    if(R<=0 || C<=0){

        side = -1;

        top = -1;

        left = -1;

        return 0;

    }



    ///////////////////////////////


    // Declaring an array name D as mentioned
    
    
    // "d[i][j] = side length of the largest all-ones square whose bottom-right corner is (i,j)"


    /////////////////////////////////


    int * d = new int [R*C];



    //////////////////////

     // TRACK LARGEST SIDE AS MENTIONED IN STEP 3 FROM ASSIGNMENT


    //////////////////////

    int maxSide = 0;

    int maxRow = -1;

    int maxCol = -1;



    //////////////////////

    // Traversing the row

    //////////////////////


    for(int i=0 ; i< R ; i++){

        for(int j=0 ; j< C ; j++){



            int TraverseIndex = i*C+j;



            

            
            //  Step 1: 
            //copy pasted from assignment 
            //m[i][j] is 0 then d[i][j] is 0

            


            if(m[TraverseIndex] == 0){

                d[TraverseIndex] = 0;

            }



            //  Step 1: 
            //copy pasted from assignment 
            // d[i][j] = 1 when i == 0 or j == 0

            


            else if(i==0 || j==0){

                d[TraverseIndex] = 1;

            }



                ///////////////////////

                // Mentioned in step 2

                //When m[i][j] is 1
                // and both i > 0 and j > 0,

                //d[i][j] = 1
                // + minimum( d[i-1][j], d[i][j-1], d[i-1][j-1] )


                ///////////////////////////


            else{


                int sizeRowAbove = (i-1)*C+j;

                int SizeLeftSide = i*C+(j-1);

                int Szediagonal = (i-1)*C+(j-1);



                int RowAbovevlue = d[sizeRowAbove];

                int LeftVlue = d[SizeLeftSide];

                int DiagVlue = d[Szediagonal];


                 // same formulea

                d[TraverseIndex] = 1 + getMin(RowAbovevlue , LeftVlue , DiagVlue);

            }

            // simple formulea for the yk 
            // fnd max one
            






            if(d[TraverseIndex] > maxSide){

                maxSide = d[TraverseIndex];

                maxRow = i;

                maxCol = j;

            }

        }

    }



    //////////////////////////

    // Delete dynamic array

    //////////////////////////


    delete[] d;

    d = nullptr;



    //////////////////////////

    // if the image contains no 1 at all
    // it returns 0 and sets all three to -1

    //////////////////////////


    if(maxSide == 0){

        side = -1;

        top = -1;

        left = -1;

        return 0;

    }


            //////////////////////////////////

            // Now here is where game changes


            // ATGC


            //The largest square is the largest value in d
            // and the cell holding it is that square's bottom-right corner
            // so its top-left corner is (i - side + 1, j - side + 1)


            //////////////////////////


    side = maxSide;

    top = maxRow - maxSide + 1;

    left = maxCol - maxSide + 1;



    return (long long)maxSide * maxSide;

}
