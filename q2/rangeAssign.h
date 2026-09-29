#include "build.h"



/////////////////////////////////

//SAME LAYER LOGIC JUST A LITTLE PART GET CHANGED

//////////////////////////////


void UpdateArrayChildern(int index){

    if(hasAssign[index]){

        ///////////////////////////

        // LIKE IN PREVOUS CASE WE ADD VALUE TO CHILDERN SAME GOES HERE 

        // HERE YOU ASSIGN CHILDREN OF UPDATED ARRAY 

        // THEN ASSIGN IT TO THE CHILD OF THE UPDATED ARRAY AND THE ASSIGNED ARRAY THAT IS YK 0 WE INITIALIZED IN BUILD .H 

        ///////////////////////////////

        updatedArray[2 * index + 1] = assignArray[index];

        updatedArray[2 * index + 2] = assignArray[index];


        ////////////////////////////////////
        

        // update the childern too 


        ///////////////////////////////////



        assignArray[2 * index + 1] = assignArray[index];

        assignArray[2 * index + 2] = assignArray[index];


        ///////////////////////////////

        // update the has assign to its childern to true 

        /////////////////////////////




        hasAssign[2 * index + 1] = true;

        hasAssign[2 * index + 2] = true;


        ///////////////////

        // and then no need to update the parent 


        ////////////////////



        hasAssign[index] = false;

    }

}


void RangeAssign(int index , int lowerLimit , int higherLimit ,int givenLowerRange , int givenHigherRange ,long long changingOne[] , long long value ){

       ///////////////////////


       // Here we have 3 scienrios


       ///////////////////

       // 1)  not there 

       // 2)  completely match 

       // 3) partial match 

       ///////////////////



       // case 1


       // there are again two condition 

       //1) if My current range ends BEFORE the query range starts.
       //2) My current range starts AFTER the query range ends.

       if(lowerLimit > givenHigherRange || higherLimit < givenLowerRange){

        // nothing to change

        return ;
       }


      


      

       // case 2 

       // perfectly found


       //it has again two cases

       //1) the higher current range is before gven high range

       //2) higher limit is < or equal to the start of it 



       if(givenLowerRange <= lowerLimit && higherLimit <= givenHigherRange){

            changingOne[index] = value;// PLACE VALUE IN PARENT

            assignArray[index] = value;// PLACE IN CORRENSPONDING TOO

            hasAssign[index] = true;// MAKE THE ASSIGNMENT TRUE AKA SET ALL PARMETERS 

            return;
       }

      
       /////////////////////////////////////

       // CASE 3 PARTIAL OVERLAP 

       /////////////////////////////////

       
       ///////////////////////
       // ONLY THAT LOGIC CHANGES
       //////////////////////////

       UpdateArrayChildern(index);

       // GO TO MID SAME TRICK AND TO END INDEXES AND THEN ADD IT 

       int mid = (lowerLimit + higherLimit) / 2;

       RangeAssign(2 * index + 1 , lowerLimit , mid , givenLowerRange , givenHigherRange , changingOne , value );

       RangeAssign(2 * index + 2 , mid + 1 , higherLimit ,givenLowerRange , givenHigherRange , changingOne , value );


       changingOne[index] = changingOne[2*index+1] > changingOne[2*index+2]? changingOne[2*index+1] : changingOne[2*index+2];
       
}


void rangeAssign(int lowerLimit , int higherLimit , long long value){

    budget::updateBudget();

    RangeAssign(0 , 0 , arraySize - 1 , lowerLimit , higherLimit , updatedArray , value);

}