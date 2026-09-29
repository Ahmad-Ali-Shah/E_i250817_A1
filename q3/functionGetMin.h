#pragma once

/////////////////////////////

// Needed Function

///////////////////////////





int getMin(int a , int b ){

    return a < b ? a : b;

} 


int getMin(int a , int b , int c){

    return getMin(getMin(a,b),c);

}


void swap(int & a , int & b){
    int temp = a^b;
    a = temp^b;
    b = temp^a;
}