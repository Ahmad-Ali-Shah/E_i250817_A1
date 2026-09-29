#pragma once

#include "neuron.h"
#include "synapse.h"

////////////////////////////////////////////////////////////////////////////////
// As you mentioned in the assignement on the page2
//
// 1)Class Layer
//
// 2)Represents a single grid layer in the vertical doubly linked list
//
// 3)Every layer holds a fixed N x N grid of neuron slots
//
////////////////////////////////////////////////////////////////////////////////

class Layer {
public:

    int N;
    int currentCount;
    int layerId;

    Neuron* topLeft;
    Layer* next;

    ////////////////////////////////////////////////////////////////////////////
    // Constructors
    ////////////////////////////////////////////////////////////////////////////

    Layer() {

        this->N = 0;
        this->currentCount = 0;
        this->layerId = 0;
        this->topLeft = nullptr;
        this->next = nullptr;

    }

    Layer(int dimensionSize, int layerIdentifier) {

        this->N = dimensionSize;
        this->currentCount = 0;
        this->layerId = layerIdentifier;
        this->topLeft = nullptr;
        this->next = nullptr;

    }

    ////////////////////////////////////////////////////////////////////////////
    // Getters and Setters
    ////////////////////////////////////////////////////////////////////////////

    bool isFilled() const {

        return (this->currentCount >= (this->N * this->N));

    }

    int getCapacity() const {

        return (this->N * this->N);

    }

    int getCurrentCount() const {

        return this->currentCount;

    }

    void setCurrentCount(int newCount) {

        this->currentCount = newCount;

    }

    int getLayerId() const {

        return this->layerId;

    }

    void setLayerId(int newIdentifier) {

        this->layerId = newIdentifier;

    }

    int getN() const {

        return this->N;

    }

    void setN(int newDimension) {

        this->N = newDimension;

    }

    Neuron* getTopLeft() const {

        return this->topLeft;

    }

    void setTopLeft(Neuron* newTopLeft) {

        this->topLeft = newTopLeft;

    }

    Layer* getNext() const {

        return this->next;

    }

    void setNext(Layer* newNextLayer) {

        this->next = newNextLayer;

    }

};
