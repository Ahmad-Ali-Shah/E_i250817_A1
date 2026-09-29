#include"build.h"

#include "budget.h"












long long RangeMax(int index , int lowerLimit , int higherLimit , int givenLowerRange , int givenHigherRange){

    /////////////////////////////

    // HERE WE SHALL DO THE SAME 3 CASES AS I MENTIONED EARLEIER
    
    /////////////////////////////


    
    // 1) Range Doesnot exist 


    if(lowerLimit > givenHigherRange || higherLimit < givenLowerRange){

        ///////////////////////////////////////
        // C limits is allowed 
        // its a library that provide us limit value like INT_MAX AND INT_MIN 
        // these are very useful in many purposes 
        // like if the value overflow WE TOOK INT MAX AND SAME FOR THE OTHER

        // HERE SINCE IT WORKS ON THE LONG LONG SO WE RETURN THE LONG LONG MIN

        ///////////////////////////////////



        return LLONG_MIN;
    }


    //2) completely match then that one is max sum of other 



    if(givenLowerRange <= lowerLimit && higherLimit <= givenHigherRange){

        ////////////////
        // gettiing max one 
        //////////////

        return updatedArray[index];
    }


    
    // 3) partial overlap

    int mid = (lowerLimit + higherLimit) / 2;

    // if so same find max for each layer of left and rght  the only difference is in early ones we were just traversing here we are traversng  + returnng value and in each getting max of it 


    long long leftSolMax = RangeMax(  2 * index + 1,lowerLimit, mid, givenLowerRange,givenHigherRange);


    long long rightSolMax = RangeMax( 2 * index + 2, mid + 1,higherLimit,givenLowerRange, givenHigherRange );


    return leftSolMax > rightSolMax ? leftSolMax : rightSolMax; 
}




long long rangeMax(int lowerRange, int upperRange){

    // THAT'S LAPIECE THE TOP ONE BE THE MAX ONE 


    budget::updateBudget();



    long long answer = RangeMax( 0,0, arraySize - 1,lowerRange, upperRange );

    return answer;
}