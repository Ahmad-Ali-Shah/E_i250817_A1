#pragma once

#include "layer.h"
#include "neuron.h"
#include "synapse.h"
#include <iostream>
#include <fstream>

using namespace std;



void NeuroMesh::printLayer(int layerIndex) {
    Layer* layer = getLayerByIndex(layerIndex);
    if (!layer) {
        cout << "Layer " << layerIndex << " does not exist." << endl;
        return;
    }

    cout << "Layer " << layerIndex << " (currentCount = " 
         << layer->currentCount << " / " << (this->N * this->N) << ")" << endl;

    for (int r = 0; r < this->N; r++) {
        for (int c = 0; c < this->N; c++) {
            Neuron* n = getNeuronAt(layer, r, c);
            if (n != nullptr) {
                cout << "[id=" << n->id << " w=" << n->weight << "] ";
            } else {
                cout << "[empty] ";
            }
        }
        cout << "\n";
    }
}


void NeuroMesh::exportMesh(const std::string& fileName) {
    ofstream outFile(fileName.c_str());
    if (!outFile.is_open()) {
        cout << "Failed to open " << fileName << " for writing." << endl;
        return;
    }

    outFile << "# NeuroMesh Export" << endl;
    outFile << "N " << this->N << endl;
    outFile << "pruningThreshold " << this->pruningThreshold << endl;
    outFile << "fusionThreshold " << this->fusionThreshold << endl;

    int layerIdx = 0;
    Layer* lay = headLayer;
    while (lay != nullptr) {
        outFile << "\nLAYER " << layerIdx << " ID " << lay->layerId 
                << " COUNT " << lay->currentCount << endl;

        for (int r = 0; r < this->N; r++) {
            for (int c = 0; c < this->N; c++) {
                Neuron* n = getNeuronAt(lay, r, c);
                if (n != nullptr) {
                    outFile << "NEURON id=" << n->id << " weight=" << n->weight 
                            << " row=" << r << " col=" << c << endl;

                    Synapse* s = n->headAxon;
                    while (s != nullptr) {
                        outFile << "  SYNAPSE target_id=" 
                                << (s->targetNeuron ? s->targetNeuron->id : -1)
                                << " weight=" << s->weight 
                                << " active=" << (s->isActive ? 1 : 0) << endl;
                        s = s->nextSynapse;
                    }
                }
            }
        }
        lay = lay->next;
        layerIdx++;
    }

    outFile.close();
    cout << "Mesh successfully exported to " << fileName << endl;
}

// -----------------------------------------------------------------
// findNeuronById(id) - Section 9
// -----------------------------------------------------------------
Neuron* NeuroMesh::findNeuronById(int id) {
    Layer* lay = headLayer;
    int layerIdx = 0;

    while (lay != nullptr) {
        for (int r = 0; r < this->N; r++) {
            for (int c = 0; c < this->N; c++) {
                Neuron* n = getNeuronAt(lay, r, c);
                if (n != nullptr && n->id == id) {
                    cout << "Neuron " << id << " found at Layer " << layerIdx 
                         << ", Row " << r << ", Column " << c 
                         << " (weight=" << n->weight << ")" << endl;
                    return n;
                }
            }
        }
        lay = lay->next;
        layerIdx++;
    }

    return nullptr;
}


Neuron* NeuroMesh::findNeuronByPosition(int layerIndex, int row, int col) {

    Layer* lay = getLayerByIndex(layerIndex);

    if (!lay) {

        cout << "Layer " << layerIndex << " does not exist." << endl;

        return nullptr;

    }
    if (row < 0 || row >= this->N || col < 0 || col >= this->N) {

        cout << "Coordinates (" << row << ", " << col << ") out of bounds." << endl;

        return nullptr;

    }

    Neuron* n = getNeuronAt(lay, row, col);

    if (n != nullptr) {

        cout << "Position [Layer " << layerIndex << ", Row " << row  << ", Col " << col << "] holds Neuron id=" << n->id << ", weight=" << n->weight << endl;

    } 
    else {

        cout << "Position [Layer " << layerIndex << ", Row " << row << ", Col " << col << "] is vacant." << endl;

        
    }

    return n;

}


void NeuroMesh::printNeuron(int id) {

    Neuron* n = findNeuronById(id);


    if (!n) {

        cout << "Neuron id " << id << " not found." << endl;

        return;

    }

    Layer* lay = locateLayerOfNeuron(n);

    int layIdx = getLayerIndex(lay);

    int col = findColumnIndexOfNeuron(lay, n);
    
    int row = getNeuronRow(lay, n);


    cout << "========================================" << endl;

    cout << "Neuron ID:     " << n->id << endl;

    cout << "Weight:        " << n->weight << endl;

    cout << "Layer Index:   " << layIdx << endl;

    cout << "Grid Position: (Row " << row << ", Col " << col << ")" << endl;

    cout << "Neighbors:     " << "Up=" << (n->up ? to_string(n->up->id) : "none") << " " << "Down=" << (n->down ? to_string(n->down->id) : "none") << " " << "Left=" << (n->left ? to_string(n->left->id) : "none") << " " << "Right=" << (n->right ? to_string(n->right->id) : "none") << endl;

    cout << "Outgoing Synapses:" << endl;

    Synapse* s = n->headAxon;

    if (!s) {

        cout << "  (none)" << endl;


    }
     else {

        while (s != nullptr) {

            cout << "  -> Target Neuron ID: "  << (s->targetNeuron ? s->targetNeuron->id : -1) << " | Weight: " << s->weight  << " | Active: " << (s->isActive ? "YES" : "NO") << endl;

            s = s->nextSynapse;

        }

    }
    cout << "========================================" << endl;

}


void NeuroMesh::navigateMesh() {

    if (!headLayer) {

        cout << "Mesh is empty. Nothing to navigate." << endl;

        return;

    }

    int curLayer = 0;

    int curRow = 0;

    int curCol = 0;


    // Count layers

    int totalLayers = 0;

    Layer* lay = headLayer;

    while (lay != nullptr) {

        totalLayers++;

        lay = lay->next;

    }

    char cmd = ' ';

    while (cmd != 'q' && cmd != 'Q') {
        
        cout << "\n==============================================" << endl;

        cout << "=== NeuroMesh Interactive Terminal Walk ===" << endl;

        cout << "Current Focus: Layer " << curLayer << " at (" << curRow << ", " << curCol << ")" << endl;

        cout << "Controls: [U]p Layer, [D]own Layer, [W]/[S] Row, [A]/[D] Col, [Q]uit" << endl;

        cout << "==============================================" << endl;


        Layer* l = getLayerByIndex(curLayer);

        if (l != nullptr) {

            for (int r = 0; r < this->N; r++) {

                for (int c = 0; c < this->N; c++) {

                    Neuron* n = getNeuronAt(l, r, c);

                    bool isFocus = (r == curRow && c == curCol);


                    if (isFocus) 
                    cout << ">";

                    if (n != nullptr) {

                        cout << "[id=" << n->id << " w=" << n->weight << "]";

                    } 
                    else {

                        cout << "[empty]";

                    }
                    if (isFocus) 
                    cout << "< ";

                    else 

                    cout << "  ";
                }
                cout << "\n";
            }
        }

        Neuron* curN = getNeuronAt(l, curRow, curCol);

        if (curN != nullptr) {

            cout << "\nFocused Neuron: ID=" << curN->id << " Weight=" << curN->weight << endl;

            cout << "Outgoing axons: ";

            Synapse* s = curN->headAxon;

            
            if (!s)

             cout << "(none)";

            while (s != nullptr) 
            {
                cout << "->[id=" << (s->targetNeuron ? s->targetNeuron->id : -1) << ", w=" << s->weight << (s->isActive ? ", ACTIVE" : "") << "] ";

                s = s->nextSynapse;

            }
            cout << endl;

        } 
        else {

            cout << "\nFocused Slot: VACANT" << endl;

        }

        cout << "\nEnter command (u=prev layer, d=next layer, w=up row, s=down row, a=left col, d=right col, q=exit): ";

        if (!(cin >> cmd)) // cmd = command 
        break;

        if (cmd == 'u' || cmd == 'U') {

            if (curLayer > 0)
             curLayer--;

        }
         else if (cmd == 'd' || cmd == 'D') {

            if (curLayer < totalLayers - 1) 
            curLayer++;

        }
         else if (cmd == 'w' || cmd == 'W') {

            if (curRow > 0)
            curRow--;
        } 
        else if (cmd == 's' || cmd == 'S') {

            if (curRow < this->N - 1) 
            curRow++;
        }
         else if (cmd == 'a' || cmd == 'A')
         {
            
            if (curCol > 0) 
            curCol--;


        } else if (cmd == 'f' || cmd == 'F' || cmd == 'r' || cmd == 'R') {

            if (curCol < this->N - 1) 
            curCol++;

        }
    }
}



void NeuroMesh::debugDumpLayer(int layerIndex){

    Layer* target = headLayer;
    

    for(int i = 0; i < layerIndex; i = i + 1){

        if(target == nullptr){

            cout << "That layer does not exist." << endl;
            return;

        }

        target = target->next;

    }

    if(target == nullptr){

        cout << "That layer does not exist." << endl;
        return;

    }

    cout << "Layer " << target->layerId << "  (currentCount = " << target->currentCount << " / " << (target->N * target->N) << ")" << endl;

    for(int row = 0; row < target->N; row = row + 1){

        for(int col = 0; col < target->N; col = col + 1){

           
            Neuron* colTop = target->topLeft;
            bool colExists = true;

            for(int i = 0; i < col; i = i + 1){

                if(colTop == nullptr){

                    colExists = false;
                    break;

                }

                colTop = colTop->right;

            }

            if(colTop == nullptr){

                colExists = false;

            }

            

            Neuron* cell = nullptr;

            if(colExists){

                cell = colTop;
                bool cellExists = true;

                for(int i = 0; i < row; i = i + 1){

                    if(cell == nullptr){

                        cellExists = false;
                        break;

                    }

                    cell = cell->down;

                }

                if(cell == nullptr){

                    cellExists = false;

                }

                if(!cellExists){

                    cell = nullptr;

                }

            }

            

            if(cell != nullptr){

                cout << "[id=" << cell->id << " w=" << cell->weight << "] ";

            }
            else{

                cout << "[empty] ";

            }

        }

        cout << endl;

    }

}



Neuron* NeuroMesh::debugGetNeuronAt(int layerIndex, int row, int col){

    Layer* target = headLayer;

    for(int i = 0; i < layerIndex; i = i + 1){

        if(target == nullptr){

            return nullptr;

        }

        target = target->next;

    }

    if(target == nullptr){

        return nullptr;

    }

    Neuron* colTop = target->topLeft;

    for(int i = 0; i < col; i = i + 1){

        if(colTop == nullptr){

            return nullptr;

        }

        colTop = colTop->right;

    }

    if(colTop == nullptr){

        return nullptr;

    }

    Neuron* cell = colTop;

    for(int i = 0; i < row; i = i + 1){

        if(cell == nullptr){

            return nullptr;

        }

        cell = cell->down;

    }

    return cell;

}
