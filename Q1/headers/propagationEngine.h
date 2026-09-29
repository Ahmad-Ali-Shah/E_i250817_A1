#pragma once

#include "layer.h"
#include "neuron.h"
#include "synapse.h"
#include <iostream>
#include <cmath>

using namespace std;

 char toUpperChar(char c) {

    return (c >= 'a' && c <= 'z') ? (c - 'a' + 'A') : c;
    
}



char NeuroMesh::forwardPropagate(char inputLetter) {
    if (!headLayer || !headLayer->topLeft) {
        return inputLetter;
    }

    int n = toUpperChar(inputLetter) - 'A';

    n = ((n % 26) + 26) % 26;


    // Clear all active synapses across mesh before new forward pass

    Layer* lay = headLayer;

    while (lay != nullptr) {

        Neuron* colTop = lay->topLeft;

        while (colTop != nullptr) {

            Neuron* curr = colTop;

            while (curr != nullptr) {

                Synapse* syn = curr->headAxon;

                while (syn != nullptr) {

                    syn->isActive = false;

                    syn = syn->nextSynapse;

                }
                curr = curr->down;

            }
            colTop = colTop->right;

        }
        lay = lay->next;

    }

    Neuron* current = headLayer->topLeft;


    while (current != nullptr && current->headAxon != nullptr) {

        Synapse* best = nullptr;

        double maxW = -1e18;

        int bestPriority = 999;

        Synapse* s = current->headAxon;


        Layer* currLayer = locateLayerOfNeuron(current);

        Layer* nextLayer = (currLayer ? currLayer->next : nullptr);

        int cr = (currLayer ? getNeuronRow(currLayer, current) : -1); // DEFAULT WAY REPLACED BY THE INDEX LOGIC 
        

        int cc = (currLayer ? findColumnIndexOfNeuron(currLayer, current) : -1);

        while (s != nullptr) {

            int priority = 999;

            if (nextLayer && s->targetNeuron) {

                /////////////////////////////////////////////
                //  CHECKING WHICH ONE CONDITION IS IT REFERING TO
                ///////////////////////////////////////////////


                int tr = getNeuronRow(nextLayer, s->targetNeuron);

                int tc = findColumnIndexOfNeuron(nextLayer, s->targetNeuron);

                if (tr == cr && tc == cc - 1)
                 priority = 1;      // Left
                else if (tr == cr && tc == cc + 1)
                 priority = 2; // Right
                else if (tr == cr - 1 && tc == cc)
                 priority = 3; // Up
                else if (tr == cr + 1 && tc == cc) 
                
                priority = 4; // Down
            }

            if (s->weight > maxW + 1e-9) {
                maxW = s->weight;
                best = s;
                bestPriority = priority;
            } else if (fabs(s->weight - maxW) <= 1e-9) {
                if (priority < bestPriority) {
                    best = s;
                    bestPriority = priority;
                }
            }
            // Clear active flag on all synapses on this neuron
            s->isActive = false;
            s = s->nextSynapse;
        }

        if (best == nullptr || best->targetNeuron == nullptr) {

            break;

        }

        best->isActive = true;

        n = ((n + (int)fmod(best->weight, 26.0)) % 26 + 26) % 26;

        current = best->targetNeuron;

    }

    lastInputLetter = inputLetter;

    lastOutputLetter = (char)('A' + n);

    lastForwardEndNeuron = current;

    hasForwardRun = true;

    return lastOutputLetter;
}


void NeuroMesh::backwardPropagate(char targetLetter) {

    if (!hasForwardRun || !lastForwardEndNeuron) {

        cout << "Error: forwardPropagate must be run before backwardPropagate." << endl;

        return;

    }

    int targetNum = toUpperChar(targetLetter) - 'A';

    int actualNum = toUpperChar(lastOutputLetter) - 'A';

    double error = (double)(targetNum - actualNum);


    Neuron* current = lastForwardEndNeuron;

    Layer* currLayer = locateLayerOfNeuron(current);


    while (currLayer != nullptr && currLayer != headLayer) {

        Layer* prevLayer = getPreviousLayer(currLayer);

        if (!prevLayer)
         break;


        // Search prevLayer for neuron whose currently active synapse targets current
        Neuron* prevNeuron = nullptr;

        Synapse* activeSyn = nullptr;


        Neuron* colTop = prevLayer->topLeft;

        while (colTop != nullptr && !prevNeuron) {

            Neuron* cand = colTop;

            while (cand != nullptr && !prevNeuron) {

                Synapse* s = cand->headAxon;

                while (s != nullptr) {

                    if (s->isActive && s->targetNeuron == current) {

                        prevNeuron = cand;
                        
                        activeSyn = s;

                        break;
                        
                    }

                    s = s->nextSynapse;

                }

                cand = cand->down;

            }

            colTop = colTop->right;
        }

        if (activeSyn != nullptr) {

            activeSyn->weight += error;

            current = prevNeuron;

            currLayer = prevLayer;

        } 
        else {

            break;

        }
    }

    
    enforceRules();
}
