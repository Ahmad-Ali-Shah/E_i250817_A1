#pragma once

#include "functionGetMin.h"





long long count_ones_squares(const int* m,int R,int C){


    ////////////////////////////


    // EDGE CASES 

    ////////////////////////////

    if(R<=0 || C<=0){

        return 0;

    }



    ///////////////////////////////


    // Declaring an array name D as mentioned
    
    
    // "d[i][j] = side length of the largest all-ones square whose bottom-right corner is (i,j)"


    /////////////////////////////////


    int * d = new int [R*C];



    //////////////////////

     // TOTAL COUNT TAKING SAME NAME AS MENTIONED IN ASSIIGNMENT 


    //////////////////////

    long long totalCount = 0;



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



            //////////////////////////////////

            // Now here is where game changes


            // ATGC


            //The total count is 
            //the plain sum of every entry of d, 
            //because a cell whose value is 3 
            //is the bottom-right corner of exactly three all-ones squares,
            // of sides 1, 2 and 3.
            // WHICH is nothiing not total counter += Each element plain meaning 
            // which s same as provided formulea




            //////////////////////////


            totalCount = totalCount + d[TraverseIndex];

        }

    }



    //////////////////////////

    // Delete dynamic array

    //////////////////////////


    delete[] d;

    d = nullptr;



    return totalCount;

}