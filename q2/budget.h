#pragma once





namespace budget{

    long long opVisits = 0;

    void updateBudget(){
        opVisits++; // call for each and hence act like the static no need of static 
    }




}



namespace bildOne{

    long long buildVisits = 0;
    

    void updateBuild(){
        buildVisits++;
    }
}
