#pragma once

#include <climits>
#include "budget.h"


//////////////////////////////////


//// My build function logic 



////////////////////////////



/////////////////////////////////

// FOR EACH CREATED A LONG LONG ARRAY AND INITIALIZE IT SO NO NEED TO DO IT
// FOR THE OTHER 

// UPDATED FOR BUILD 

// ADD FOR THE ADDIING ELEMENT 

// ASSIGN FOR THE ASSIGNIING EACH ONE 

// HAS ASSIGN NORMALLY WHEN YOU HAVE DONE SOME OPERATION THEN AT END WE NEED TO MAINTAIN HIRARCHY SAY WE DID A+5 THEN ITS SUM BE S  = A+5+B NOT THAT OUTDATED A+B 



///////////////////////////////

long long * updatedArray;
long long * addArray;
long long * assignArray;
bool * hasAssign;

int arraySize;


void myPlacement(int index , int lowerLimit , int higherLimit , int arr[] , long long updatedOne[] ){
    // arr changed from long long[] to int[] : the required test harness
    // in main.cpp allocates "int *a = new int[n]" exactly as given in the
    // assignment spec, so this must accept int to match and actually compile.
    // updatedOne stays long long since stored maxima can exceed int range
    // once rangeAdd starts applying deltas.

    // Count every entry into the recursive build function
    bildOne::updateBuild();


    ////////////////////////////////

    // Hirarchy 

    // lower layer -> hold the array elements 

    // upper one hold their maximum 

    //////////////////////////////////


    /////////////////////////////////////

    // Base Case


    //////////////////////////////////


    if(lowerLimit == higherLimit){

        ///////////////////////////
        // we reach to the bottom layer 
        // in the bottem layer we will set elements 
        ////////////////////////////

        updatedOne[index] = arr[lowerLimit];

        return;
    }



    // now vvvvvv important 


    // what if i am at a and b , c its belowest element then how to reach it 

    // we need to check each one but its in b->c interval so for that we need to reach b and c thereform one be lower point of b and MID THAT ONE IS VVVVV IMPORTANT SINCE MID is in between two conzective element of layer hence we are going to find the mid 


    int mid = (lowerLimit + higherLimit ) / 2;



    // then move recursively one for the left move total left until you reach to base layer then its right  THEN FIND MAX OF BOTH AND ASSIGNED TO THE RESULTING ONE 


    // it lie in the limit of l -> mid 

    myPlacement( 2 * index + 1, lowerLimit, mid, arr,updatedOne );


    // it lie in the limit of mid -> high 

    myPlacement( 2 * index + 2, mid + 1, higherLimit, arr,updatedOne);


    // aftr that it be at top one ie for 3,2,1 first it move to 2 then 1 then move out and pass throgh THAT COMMENTED LINE BEST TIME TO READ THE VALUE and implement what need ie max


    updatedOne[index] = updatedOne[2 * index + 1] > updatedOne[2 * index + 2]? updatedOne[2 * index + 1]: updatedOne[2 * index + 2];

}


void build(int arr[] , int size ){
    // same fix as myPlacement above: int[] to match main.cpp's "int *a"

    //////////////////////////////

    // update the build one 


    /////////////////////////////


    //////////////////////////////
    // NOW IINITIALIZING ALL 
    /////////////////////////////


    // WHY 4* SIZE 


    // 1) ITS SAFE AND USE AS STANDERED FOR EVERY QUESTION WHERE YOU STORE AND ELEMENT BY MEANS OF RANGE OR SEPRATION IITS BEST

    // 2) IT IS USE TO CATER ALL 2* SIZE CAN BUT IT CAN'T HANDLE THE EDGE CASE 


    updatedArray = new long long[4 * size];

    addArray = new long long[4 * size];

    assignArray = new long long[4 * size];


    hasAssign = new bool[4 * size];

    arraySize = size;


    for(int i = 0 ; i < 4 * size ; i++){

        addArray[i] = 0;

        assignArray[i] = 0;

        hasAssign[i] = false;

    }


    // saving the array in the form of the interval wise 

    ////////////////////////////////////////

    // here my rules of building
    
    // 1) here i am arranging the array in such way such that maximum of b and c is stored in a
    
    // 2) by doing that i just make the layer in array upper layer hold the maximum of below two then their also have the same division 
    
    // 3) now when ever i need to perform operation i need to traverse through the layers and it can be 1->2 then that 2->4 since division is BINAEY NOT SINGULAR SO 1->2->4->8->...................... 2^k hence for worst case ALL OPERATION TAKES O(LOG N)

    ////////////////////////////////////////


    

    // now saving it 
   
    myPlacement(0,0,size - 1, arr,updatedArray);

}


//////////////////////////////////////////////////////////////

// That's the beauty of .h file you need to just make an solve
// now they all got over rid in the result one 
// i am going to explain each and every thing in detail


//////////////////////////////////////////////////////////////

void applyAssign(int index, long long val){
    ///////////////////////
    // Assign functionalty 
    // complexty : o(1)
    // use in read Assign functon  
    /////////////////////


    // here four functionality 

    // 1) place value  to the current node of tree

    // 2) also update the assigned array value 

    // 3) since for that index we assigend not add so add be 0


    updatedArray[index] = val;

    assignArray[index] = val;

    hasAssign[index] = true;


    addArray[index] = 0;

}

void applyAdd(int index, long long AddingFactor){

    // again o(1) update the current array updated 
    // just we add to parent 
    // if we need to assign to like both at the tiime extra feature then add that too

    updatedArray[index] += AddingFactor;


    if(hasAssign[index])
    {

        assignArray[index] += AddingFactor;

    } 
    else {

        addArray[index] += AddingFactor; // else we shall update it as always add array 
    }
}

void pushDown(int index){
/////////////////
// i did so ii used or call for the multiple functiions 
///////////////////


    if(hasAssign[index]){
        // for example see
        // here I ASSIGEN FOR THE RESULT OF DAUGHER OF PARENT THEN PARENT GOT UPDATED IN LOOP 
        applyAssign(2 * index + 1, assignArray[index]);

        applyAssign(2 * index + 2, assignArray[index]);

        hasAssign[index] = false; // ASSIIGENMENT GO FALSE 

        assignArray[index] = 0; // YOU SEE THAT'S WHY I USED THE ARRAY ASSIGN CUZ IT DEAL BOTH 

    }

    if(addArray[index] != 0){

        // SAME ADD DAUGHETER WTH THE FACTOR AND THAT'S FACTOR IIS PUSH DOWN MEANS WE UPDATE DOWN AND UP GOT UPDATED TOP 


        applyAdd(2 * index + 1, addArray[index]);


        applyAdd(2 * index + 2, addArray[index]);


        addArray[index] = 0;
        

    }
}


////////////////////////////////////


// When we allocate it we need to de allocate it too


//////////////////////////////


void freeBuild(){
    delete[] updatedArray;
    delete[] addArray;
    delete[] assignArray;
    delete[] hasAssign;

    updatedArray = nullptr;
    addArray = nullptr;
    assignArray = nullptr;
    hasAssign = nullptr;
}