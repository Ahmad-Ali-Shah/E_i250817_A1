#pragma once

#include "layer.h"
#include "neuron.h"
#include "synapse.h"
#include <iostream>
#include <fstream>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// As per assgnement 
//
// Encapsulates single-pass text file parsing for network configuration and weights.
////////////////////////////////////////////////////////////////////////////////

void NeuroMesh::loadFromFile(const std::string &fileName)
{

    ////////////////////////////////////////////////////////////
    // opening the file, only once, as the assignment requires
    //////////////////////////////////////////////////////////

    fstream ReadOnce;

    ReadOnce.open(fileName.c_str(), ios::in);// static_cast<const std::string >(flename )

    if (!ReadOnce.is_open()) {
        std::string fallback = "../" + fileName;
        ReadOnce.open(fallback.c_str(), ios::in);
    }

    if(ReadOnce.is_open()){

        

    }
    else{

        cout << "File not found :(";

        exit(0); // stop the program read in oop as general discusson 

    }




    // the assignment says this first line has exactly 3 numbers space separated in this exact order

    ReadOnce >> this->N;
    ReadOnce >> this->pruningThreshold;
    ReadOnce >> this->fusionThreshold;


  
    //  one neuron id and weight per line,
    // continuing until the file runs out
   

    int IdNeuron;
    double WeightNeuron;

    while(ReadOnce >> IdNeuron >> WeightNeuron){

        insertNeuron(IdNeuron, WeightNeuron);

    }

    ReadOnce.close(); // reading is done, only closed once


  }