#pragma once

#include "synapse.h"



class Neuron {
public:

    int id;
    double weight;

    Neuron* up;
    Neuron* down;
    Neuron* left;
    Neuron* right;

    Synapse* headAxon;

    ////////////////////////////////////////////////////////////////////////////
    // Constructors
    ////////////////////////////////////////////////////////////////////////////

    Neuron() {

        this->id = 0;
        this->weight = 0.0;
        this->up = nullptr;
        this->down = nullptr;
        this->left = nullptr;
        this->right = nullptr;
        this->headAxon = nullptr;

    }

    Neuron(int neuronIdentifier, double neuronInitialWeight) {

        this->id = neuronIdentifier;
        this->weight = neuronInitialWeight;
        this->up = nullptr;
        this->down = nullptr;
        this->left = nullptr;
        this->right = nullptr;
        this->headAxon = nullptr;

    }

    ////////////////////////////////////////////////////////////////////////////
    // Proper Object-Oriented Getters and Setters
    ////////////////////////////////////////////////////////////////////////////

    int getId() const {

        return this->id;

    }

    void setId(int newIdentifier) {

        this->id = newIdentifier;

    }

    double getWeight() const {

        return this->weight;

    }

    void setWeight(double newWeight) {

        this->weight = newWeight;

    }

    Neuron* getUp() const {

        return this->up;

    }

    void setUp(Neuron* newUpNeuron) {

        this->up = newUpNeuron;

    }

    Neuron* getDown() const {

        return this->down;

    }

    void setDown(Neuron* newDownNeuron) {

        this->down = newDownNeuron;

    }

    Neuron* getLeft() const {

        return this->left;

    }

    void setLeft(Neuron* newLeftNeuron) {

        this->left = newLeftNeuron;

    }

    Neuron* getRight() const {

        return this->right;

    }

    void setRight(Neuron* newRightNeuron) {

        this->right = newRightNeuron;

    }

    Synapse* getHeadAxon() const {

        return this->headAxon;

    }

    void setHeadAxon(Synapse* newHeadAxon) {

        this->headAxon = newHeadAxon;

    }

};
