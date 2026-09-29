#pragma once


namespace AhmadArray {
//////////////////////////////////////


//// Added three functionalities 1) add 2) delete and initialize 

    
/////////////////////////////////////




    template<typename T>
    T* arr;

    template<typename T>
    void initialize(int size) {
        AhmadArray::arr<T> = new T[size];
    }

    template<typename T>
    void deleteArray() {
        delete[] AhmadArray::arr<T>;
        AhmadArray::arr<T> = nullptr;
    }
}
