#pragma once



struct Neuron;

class Synapse {
public:

    double weight;
    bool isActive;

    Neuron* targetNeuron;
    Synapse* nextSynapse;

    ////////////////////////////////////////////////////////////////////////////
    // Constructors
    ////////////////////////////////////////////////////////////////////////////

    Synapse() {

        this->weight = 0.0;
        this->isActive = false;
        this->targetNeuron = nullptr;
        this->nextSynapse = nullptr;

    }

    Synapse(double initialWeight, Neuron* destinationNeuron) {

        this->weight = initialWeight;
        this->isActive = false;
        this->targetNeuron = destinationNeuron;
        this->nextSynapse = nullptr;

    }

    Synapse(double initialWeight, Neuron* destinationNeuron, Synapse* subsequentSynapse) {

        this->weight = initialWeight;
        this->isActive = false;
        this->targetNeuron = destinationNeuron;
        this->nextSynapse = subsequentSynapse;

    }

    ////////////////////////////////////////////////////////////////////////////
    // Getters and Setters
    ////////////////////////////////////////////////////////////////////////////

    double getWeight() const {

        return this->weight;

    }

    void setWeight(double newWeight) {

        this->weight = newWeight;

    }

    bool getIsActive() const {

        return this->isActive;

    }

    void setIsActive(bool activeFlag) {

        this->isActive = activeFlag;

    }

    Neuron* getTargetNeuron() const {

        return this->targetNeuron;

    }

    void setTargetNeuron(Neuron* newTargetNeuron) {

        this->targetNeuron = newTargetNeuron;

    }

    Synapse* getNextSynapse() const {

        return this->nextSynapse;

    }

    void setNextSynapse(Synapse* newNextSynapse) {

        this->nextSynapse = newNextSynapse;

    }

};
