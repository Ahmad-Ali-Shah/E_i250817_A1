#include "budget.h"
#include "build.h"


void ChangeChildernOfParent(int index){

    if(addArray[index] != 0)
    {
         // if you performed the sum or addtiion then its updated iif so we need to update its child too cuz iits too iin that range



        //updating left

        updatedArray[2 * index + 1] += addArray[index];

        // updating right 

        updatedArray[2 * index + 2] += addArray[index];

        // and for them we also stored the sum such that if for them we need to perform the same operation cuz their lower layer also get add by that too

        addArray[2 * index + 1] += addArray[index];

        addArray[2 * index + 2] += addArray[index];


        addArray[index] = 0; // set that index to 0 no further sum need 
          
    }
}


void RangeAdd(int index , int lowerLimit , int higherLimit , int givenLowerRange , int givenHigherRange , long long changingOne[] , long long value ){

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

            changingOne[index] += value;

            addArray[index] += value; // we update the add array too 

            

            return;
       }

      
       /////////////////////////////////////

       // CASE 3 PARTIAL OVERLAP 

       /////////////////////////////////

       
       // here we do 2 changes 
       // 1) update the childern first in that range
       // 2) its parent in max form get updated from the mid concept first update all child then update parent 



       
       ChangeChildernOfParent(index);
    
       // updating parent 


       int mid = (lowerLimit + higherLimit) / 2;

       RangeAdd(2 * index + 1 , lowerLimit , mid , givenLowerRange , givenHigherRange , changingOne , value );

       RangeAdd(2 * index + 2 , mid + 1 , higherLimit , givenLowerRange , givenHigherRange , changingOne , value );


       changingOne[index] = changingOne[2*index+1] > changingOne[2*index+2] ? changingOne[2*index+1]: changingOne[2*index+2]; //updatng parent BASED ON THE UPDATNG CHLDERN 
       
       
}



void rangeAdd(int lowerRange , int upperRange , long long value  ){

      // updatte the budget

   

      budget::updateBudget();

       RangeAdd(0 , 0 , arraySize - 1 , lowerRange , upperRange , updatedArray , value );

}