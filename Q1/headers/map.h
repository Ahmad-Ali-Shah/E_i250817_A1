#pragma once

#include "layer.h"
#include "neuron.h"
#include "synapse.h"
#include <iostream>
#include <fstream>
#include <cstdint>
#include <climits>
#include <cmath>
#include <cstdlib>
#include <cstring>


using namespace std;

////////////////////////////////////////////////////////////////////////////////
// As you mentioned on page 4-5
//
// 1)Class NeuroMesh
//
// 2)Orchestrates the biological sparse neural substrate
//
////////////////////////////////////////////////////////////////////////////////

class NeuroMesh {
private:

    Layer* headLayer;
    Layer* tailLayer;
    int N;
    int indexLayer;

    double pruningThreshold;
    double fusionThreshold;

    char lastInputLetter;
    char lastOutputLetter;
    Neuron* lastForwardEndNeuron;
    bool hasForwardRun;

public:

    NeuroMesh();
    ~NeuroMesh();

    ////////////////////////////////////////////////////////////////////////////
    // Construction Operations
    ////////////////////////////////////////////////////////////////////////////

    void loadFromFile(const std::string& fileName);
    void insertNeuron(int id, double weight);

    ////////////////////////////////////////////////////////////////////////////
    // Deletion Operations
    ////////////////////////////////////////////////////////////////////////////

    void deleteNeuron(int id);
    void deleteLayer(int layerId);

    ////////////////////////////////////////////////////////////////////////////
    // Trigger Operations
    ////////////////////////////////////////////////////////////////////////////

    void pruneNeuron(Neuron* targetNeuron);
    Neuron* mergeNeurons(Synapse* activeSynapse);
    Neuron* mergeColumns(Neuron* columnA, Neuron* columnB);
    Layer* mergeLayers(Layer* layerA, Layer* layerB);
    void removeEmptyLayer(Layer* emptyLayer);

    ////////////////////////////////////////////////////////////////////////////
    // Print and Export Operations
    ////////////////////////////////////////////////////////////////////////////

    void printLayer(int layerIndex);
    void exportMesh(const std::string& fileName);
    void navigateMesh();
    void printNeuron(int id);

    ////////////////////////////////////////////////////////////////////////////
    // Search Operations
    ////////////////////////////////////////////////////////////////////////////

    Neuron* findNeuronById(int id);
    Neuron* findNeuronByPosition(int layerIndex, int row, int col);

    ////////////////////////////////////////////////////////////////////////////
    // Signal Propagation Operations
    ////////////////////////////////////////////////////////////////////////////

    char forwardPropagate(char inputLetter);
    void backwardPropagate(char targetLetter);

    ////////////////////////////////////////////////////////////////////////////
    // Debug Inspection Helpers for Unit Tests
    ////////////////////////////////////////////////////////////////////////////

    void debugDumpLayer(int layerIndex);
    Neuron* debugGetNeuronAt(int layerIndex, int row, int col);

    ////////////////////////////////////////////////////////////////////////////
    // Getters and Setters
    ////////////////////////////////////////////////////////////////////////////

    Layer* getHeadLayer() const { return this->headLayer; }
    void setHeadLayer(Layer* newHead) { this->headLayer = newHead; }

    Layer* getTailLayer() const { return this->tailLayer; }
    void setTailLayer(Layer* newTail) { this->tailLayer = newTail; }

    int getN() const { return this->N; }
    void setN(int dimensionN) { this->N = dimensionN; }

    double getPruningThreshold() const { return this->pruningThreshold; }
    void setPruningThreshold(double newThreshold) { this->pruningThreshold = newThreshold; }

    double getFusionThreshold() const { return this->fusionThreshold; }
    void setFusionThreshold(double newThreshold) { this->fusionThreshold = newThreshold; }

    bool getHasForwardRun() const { return this->hasForwardRun; }
    char getLastInputLetter() const { return this->lastInputLetter; }
    char getLastOutputLetter() const { return this->lastOutputLetter; }
    Neuron* getLastForwardEndNeuron() const { return this->lastForwardEndNeuron; }
    void setSynapseWeight(int id, char dir, double newWeight);

    void enforceRules();

    ////////////////////////////////////////////////////////////////////////////
    // Helper Member Function Declarations
    ////////////////////////////////////////////////////////////////////////////

    Layer* locateLayerOfNeuron(Neuron* targetNeuron);
    int findColumnIndexOfNeuron(Layer* targetLayer, Neuron* targetNeuron);
    void resyncColumnHorizontalLinks(Layer* targetLayer, int columnIndex);

    void pruneOneVerticalNeighbor(Neuron* triggeringNeuron,Neuron* victimNeuron,int verticalDirection,Layer* homeLayer, int triggeringColumnIndex,Layer* previousLayer, bool hasPreviousLayer );

    void pruneOneHorizontalNeighbor(Neuron* triggeringNeuron,Neuron* victimNeuron,int horizontalDirection,Layer* homeLayer,Layer* previousLayer,bool hasPreviousLayer);

    void sortColumns(Layer* targetLayer);
    double getColumnWeight(Neuron* columnTopNeuron);
    int getColumnCount(Layer* targetLayer);
    Neuron* getColumnTop(Layer* targetLayer, int columnIndex);
    void swapColumns(Layer* targetLayer, int columnIndexA, int columnIndexB);

    void rewireAllSynapses();
    void rewireLayerSynapses(Layer* targetLayer);
    void clearLayerOutgoingSynapses(Layer* targetLayer);
    void removeSynapsesTargeting(Neuron* targetNeuron);

    int getNeuronRow(Layer* targetLayer, Neuron* targetNeuron);
    Neuron* getNeuronAt(Layer* targetLayer, int rowIndex, int columnIndex);
    Layer* getLayerByIndex(int layerIndex);
    int getLayerIndex(Layer* targetLayer);
    Layer* getPreviousLayer(Layer* targetLayer);
    void cleanMeshMemory();

};

NeuroMesh::NeuroMesh(){
    headLayer = nullptr;
    tailLayer = nullptr;
    N = 0;
    indexLayer = 0;
    pruningThreshold = 0.0;
    fusionThreshold = 0.0;
    lastInputLetter = ' ';
    lastOutputLetter = ' ';
    lastForwardEndNeuron = nullptr;
    hasForwardRun = false;
}

NeuroMesh::~NeuroMesh(){
    cleanMeshMemory();
}

#include "meshLifecycleManager.h"
#include "propagationEngine.h"
#include "meshInspector.h"
#include "meshFileReader.h"
