#pragma once

#include "layer.h"
#include "neuron.h"
#include "synapse.h"
#include <iostream>
#include <cmath>

using namespace std;



void NeuroMesh::cleanMeshMemory(){
    Layer* currLayer = headLayer;

    while(currLayer != nullptr){

        Layer* nextLayer = currLayer->next;


        

        Neuron* colTop = currLayer->topLeft;

        while(colTop != nullptr){

            Neuron* nextColTop = colTop->right;

            Neuron* currN = colTop;

            while(currN != nullptr){

                Neuron* nextDown = currN->down;


                

                Synapse* s = currN->headAxon;

                while(s != nullptr){

                    Synapse* ns = s->nextSynapse;

                    delete s;

                    s = ns;

                }
                delete currN;

                currN = nextDown;

            }
            colTop = nextColTop;

        }

        delete currLayer;

        currLayer = nextLayer;

    }

    headLayer = nullptr;

    tailLayer = nullptr;


}




void NeuroMesh::insertNeuron(int id, double weight){


    Layer* traverser = nullptr;


    if(!headLayer){

        
        

        headLayer = new Layer();

        headLayer->N = this->N;

        headLayer->currentCount = 0;

        headLayer->layerId = 0;

        headLayer->topLeft = nullptr;

        headLayer->next = nullptr;


        tailLayer = headLayer;
        
        traverser = headLayer;
        
    }
   
    else{

        
        

        traverser = headLayer;

        int nextLayerId = 0;

        while(traverser != nullptr){

            if(traverser->currentCount < (this->N * this->N)){

                

                break;

            }

            

            nextLayerId = nextLayerId + 1;

            traverser = traverser->next;

        }

        if(traverser == nullptr){

            
          

            Layer* freshLayer = new Layer();

            freshLayer->N = this->N;

            freshLayer->currentCount = 0;

            freshLayer->layerId = nextLayerId;

            freshLayer->topLeft = nullptr;

            freshLayer->next = nullptr;


            tailLayer->next = freshLayer;

            tailLayer = freshLayer;


            traverser = freshLayer;

        }

    }

    int bestCol = -1;

    double bestTotal = 0;

    int bestCount = 0;


    Neuron* columnTopPointer = traverser->topLeft;


    for(int col = 0; col < this->N; col = col + 1)
    {


        double total = 0;

        int count = 0;


        if(columnTopPointer == nullptr)
        {

            
            


        }
        else{

            

            Neuron* walker = columnTopPointer;

            while(walker != nullptr){

                total = total + walker->weight;

                count = count + 1;

                walker = walker->down;

            }

        }

        

        if(count < this->N){

            if(bestCol == -1){

                

                bestCol = col;
                bestTotal = total;
                bestCount = count;

            }
            else if(total < bestTotal){

                

                bestCol = col;
                bestTotal = total;
                bestCount = count;

            }

            
            
            
            
            

            


        }

        if(columnTopPointer == nullptr){

            

            


            break;

        }


        

        columnTopPointer = columnTopPointer->right;


    }



    

    Neuron* fresh = new Neuron();

    fresh->id = id;

    fresh->weight = weight;

    fresh->up = nullptr;

    fresh->down = nullptr;

    fresh->left = nullptr;

    fresh->right = nullptr;

    fresh->headAxon = nullptr;




    Neuron* colTop = traverser->topLeft;

    for(int i = 0; i < bestCol; i = i + 1){

        colTop = colTop->right;

    }

    if(bestCount == 0){

       

        if(bestCol == 0){

           

            traverser->topLeft = fresh;

        }

        
    }
    else{

      

        Neuron* lastInCol = colTop;

        for(int i = 0; i < bestCount - 1; i = i + 1){

            lastInCol = lastInCol->down;

        }

        lastInCol->down = fresh;
        fresh->up = lastInCol;

    }


   
    

    if(bestCol > 0){


        Neuron* leftColTop = traverser->topLeft;

        for(int i = 0; i < bestCol - 1; i = i + 1){

            leftColTop = leftColTop->right;

        }

       

        Neuron* candidate = leftColTop;
        bool found = true;

        for(int i = 0; i < bestCount; i = i + 1){

            if(candidate == nullptr){

                found = false;
                break;

            }

            candidate = candidate->down;

        }

        if(found && candidate != nullptr){

            fresh->left = candidate;
            candidate->right = fresh;

        }

    }

    

    if(bestCol < this->N - 1){

       

        Neuron* rightColTop = traverser->topLeft;
        bool rightColumnExists = true;

        for(int i = 0; i < bestCol + 1; i = i + 1){

            if(rightColTop == nullptr){

                

                rightColumnExists = false;
                break;

            }

            rightColTop = rightColTop->right;

        }

        if(rightColumnExists){

            Neuron* candidate = rightColTop;
            bool found = true;

            for(int i = 0; i < bestCount; i = i + 1){

                if(candidate == nullptr){

                    found = false;
                    break;

                }

                candidate = candidate->down;

            }

            if(found && candidate != nullptr){

                fresh->right = candidate;
                candidate->left = fresh;

            }

        }

    }




    traverser->currentCount = traverser->currentCount + 1;


  

    int freshRow = bestCount;
    int freshCol = bestCol;

    if(traverser->layerId > 0){

        
        

        Layer* prevLayer = headLayer;

        for(int i = 0; i < traverser->layerId - 1; i = i + 1){

            prevLayer = prevLayer->next;

        }

        
        
        int candidateRows[4];
        int candidateCols[4];

        candidateRows[0] = freshRow - 1; 
        candidateCols[0] = freshCol;     
        candidateRows[1] = freshRow + 1;
         candidateCols[1] = freshCol;     
        candidateRows[2] = freshRow;   
          candidateCols[2] = freshCol - 1; 
        candidateRows[3] = freshRow;  
           candidateCols[3] = freshCol + 1; 

        for(int k = 0; k < 4; k = k + 1){

            int r = candidateRows[k];

            int c = candidateCols[k];


            
            

            if(r < 0 || r >= this->N || c < 0 || c >= this->N){

                continue;

            }

            

            Neuron* colTop = prevLayer->topLeft;
            bool colExists = true;

            for(int i = 0; i < c; i = i + 1){

                if(colTop == nullptr){

                    colExists = false;
                    break;

                }

                colTop = colTop->right;

            }

            if(colTop == nullptr){

                colExists = false;

            }

            if(!colExists){

                continue;

            }

            

            Neuron* candidate = colTop;
            bool candidateExists = true;

            for(int i = 0; i < r; i = i + 1){

                if(candidate == nullptr){

                    candidateExists = false;
                    break;

                }

                candidate = candidate->down;

            }

            if(candidate == nullptr){

                candidateExists = false;

            }

            if(!candidateExists){

                continue;

            }

            
            

            Synapse* newSynapse = new Synapse();

            newSynapse->weight = (candidate->weight + fresh->weight) / 4.0;

            newSynapse->isActive = false;

            newSynapse->targetNeuron = fresh;

            newSynapse->nextSynapse = nullptr;


            if(candidate->headAxon == nullptr){

                candidate->headAxon = newSynapse;

            }
            else{

                Synapse* tail = candidate->headAxon;

                while(tail->nextSynapse != nullptr){

                    tail = tail->nextSynapse;

                }

                tail->nextSynapse = newSynapse;

            }

        }

    }


    if (this->pruningThreshold > 0.0 && fresh->weight > this->pruningThreshold) {

        pruneNeuron(fresh);

    }

}

void NeuroMesh::pruneNeuron(Neuron* n){

    if(n == nullptr){

        return;

    }

    

    Layer* homeLayer = locateLayerOfNeuron(n);

    if(homeLayer == nullptr){

        

        return;

    }

    
    
    

    bool hasPreviousLayer = (homeLayer != headLayer);

    Layer* previousLayer = nullptr;

    if(hasPreviousLayer){

        previousLayer = headLayer;

        for(int i = 0; i < homeLayer->layerId - 1; i = i + 1){

            previousLayer = previousLayer->next;

        }

    }

    

    int nColumnIndex = findColumnIndexOfNeuron(homeLayer, n);

   

    Neuron* victimUp = n->up;
    Neuron* victimDown = n->down;
    Neuron* victimLeft = n->left;
    Neuron* victimRight = n->right;

    

    

    for(int verticalDirection = 0; verticalDirection < 2; verticalDirection = verticalDirection + 1){

        Neuron* victim = nullptr;

        if(verticalDirection == 0){

            victim = victimUp;

        }
        else{

            victim = victimDown;

        }

        if(victim != nullptr){

            pruneOneVerticalNeighbor( n, victim, verticalDirection, homeLayer,  nColumnIndex,  previousLayer, hasPreviousLayer );

        }

    }

    

    for(int horizontalDirection = 0; horizontalDirection < 2; horizontalDirection = horizontalDirection + 1){

        Neuron* victim = nullptr;

        if(horizontalDirection == 0){

            victim = victimLeft;

        }
        else{

            victim = victimRight;

        }

        if(victim != nullptr){

            pruneOneHorizontalNeighbor( n, victim, horizontalDirection, homeLayer, previousLayer,hasPreviousLayer );

        }

    }



    int nColumnIndexAfter = findColumnIndexOfNeuron(homeLayer, n);

    if(nColumnIndexAfter != -1){

        resyncColumnHorizontalLinks(homeLayer, nColumnIndexAfter);

    }


    n->weight = n->weight / 2.0;

}




void NeuroMesh::pruneOneVerticalNeighbor(
    Neuron* n,
    Neuron* victim,
    int verticalDirection,   
    Layer* homeLayer,
    int nColumnIndex,
    Layer* previousLayer,
    bool hasPreviousLayer
){

   

    Synapse* axonWalker = victim->headAxon;

    while(axonWalker != nullptr){

        Synapse* nextOne = axonWalker->nextSynapse;

        delete axonWalker;

        axonWalker = nextOne;

    }

    victim->headAxon = nullptr;



    if(hasPreviousLayer){

        Neuron* prevColTop = previousLayer->topLeft;

        for(int col = 0; col < previousLayer->N; col = col + 1){

            if(prevColTop == nullptr){

                break;

            }

            Neuron* prevCell = prevColTop;

            while(prevCell != nullptr){

                Synapse* prevAxon = prevCell->headAxon;
                Synapse* trailing = nullptr;

                while(prevAxon != nullptr){

                    Synapse* afterThis = prevAxon->nextSynapse;

                    if(prevAxon->targetNeuron == victim){

                        if(trailing == nullptr){

                            prevCell->headAxon = afterThis;

                        }
                        else{

                            trailing->nextSynapse = afterThis;

                        }

                        delete prevAxon;

                    }
                    else{

                        trailing = prevAxon;

                    }

                    prevAxon = afterThis;

                }

                prevCell = prevCell->down;

            }

            prevColTop = prevColTop->right;

        }

    }

   

    bool victimWasColumnTop = false;

    if(verticalDirection == 0){

        

        victimWasColumnTop = (victim->up == nullptr);

        n->up = victim->up;

        if(victim->up != nullptr){

            victim->up->down = n;

        }

    }
    else{

  
        n->down = victim->down;

        if(victim->down != nullptr){

            victim->down->up = n;

        }

    }

  

    if(victimWasColumnTop){

        n->left = victim->left;
        n->right = victim->right;

        if(victim->left != nullptr){

            victim->left->right = n;

        }

        if(victim->right != nullptr){

            victim->right->left = n;

        }

        if(nColumnIndex == 0){

            homeLayer->topLeft = n;

        }

    }

    delete victim;

    homeLayer->currentCount = homeLayer->currentCount - 1;

   

    resyncColumnHorizontalLinks(homeLayer, nColumnIndex);

}



void NeuroMesh::pruneOneHorizontalNeighbor(Neuron* n, Neuron* victim, int horizontalDirection,   Layer* homeLayer, Layer* previousLayer, bool hasPreviousLayer){

 

    Synapse* axonWalker = victim->headAxon;

    while(axonWalker != nullptr){

        Synapse* nextOne = axonWalker->nextSynapse;

        delete axonWalker;

        axonWalker = nextOne;

    }

    victim->headAxon = nullptr;

    if(hasPreviousLayer){

        Neuron* prevColTop = previousLayer->topLeft;

        for(int col = 0; col < previousLayer->N; col = col + 1){

            if(prevColTop == nullptr){

                break;

            }

            Neuron* prevCell = prevColTop;

            while(prevCell != nullptr){

                Synapse* prevAxon = prevCell->headAxon;
                Synapse* trailing = nullptr;

                while(prevAxon != nullptr){

                    Synapse* afterThis = prevAxon->nextSynapse;

                    if(prevAxon->targetNeuron == victim){

                        if(trailing == nullptr){

                            prevCell->headAxon = afterThis;

                        }
                        else{

                            trailing->nextSynapse = afterThis;

                        }

                        delete prevAxon;

                    }
                    else{

                        trailing = prevAxon;

                    }

                    prevAxon = afterThis;

                }

                prevCell = prevCell->down;

            }

            prevColTop = prevColTop->right;

        }

    }



    int victimColumnIndex = findColumnIndexOfNeuron(homeLayer, victim);
    
    bool victimWasColumnTop = (victim->up == nullptr);

    Neuron* victimColumnReplacement = victim->down;


    bool nIsStillAdjacentToVictim = false;

    if(horizontalDirection == 0){

        nIsStillAdjacentToVictim = (n->left == victim);

    }
    else{

        nIsStillAdjacentToVictim = (n->right == victim);

    }

    if(nIsStillAdjacentToVictim){

        if(victimWasColumnTop && victimColumnReplacement != nullptr){

            if(horizontalDirection == 0){

                n->left = victimColumnReplacement;
                victimColumnReplacement->right = n;

                victimColumnReplacement->left = victim->left;

                if(victim->left != nullptr){

                    victim->left->right = victimColumnReplacement;

                }

            }
            else{

                n->right = victimColumnReplacement;
                victimColumnReplacement->left = n;

                victimColumnReplacement->right = victim->right;

                if(victim->right != nullptr){

                    victim->right->left = victimColumnReplacement;

                }

            }

        }
        else{

            

            if(horizontalDirection == 0){

                n->left = victim->left;

                if(victim->left != nullptr){

                    victim->left->right = n;

                }

            }
            else{

                n->right = victim->right;

                if(victim->right != nullptr){

                    victim->right->left = n;

                }

            }

        }

    }
    else{


        if(horizontalDirection == 0){

            if(victim->left != nullptr){

                victim->left->right = nullptr;

            }

        }
        else{

            if(victim->right != nullptr){

                victim->right->left = nullptr;

            }

        }

    }



    if(victim->up != nullptr){

        victim->up->down = victim->down;

    }

    if(victim->down != nullptr){

        victim->down->up = victim->up;

    }

    if(victimWasColumnTop && victimColumnIndex == 0){

        homeLayer->topLeft = victimColumnReplacement;

    }

    delete victim;

    homeLayer->currentCount = homeLayer->currentCount - 1;

 

    if(victimColumnIndex != -1){

        resyncColumnHorizontalLinks(homeLayer, victimColumnIndex);

    }

}



Layer* NeuroMesh::locateLayerOfNeuron(Neuron* target){

    Layer* layerWalker = headLayer;

    while(layerWalker != nullptr){

        Neuron* colTop = layerWalker->topLeft;

        for(int col = 0; col < layerWalker->N; col = col + 1){

            if(colTop == nullptr){

                break; 

            }

            Neuron* cell = colTop;

            while(cell != nullptr){

                if(cell == target){

                    return layerWalker;

                }

                cell = cell->down;

            }

            colTop = colTop->right;

        }

        layerWalker = layerWalker->next;

    }

    return nullptr; 

}



int NeuroMesh::findColumnIndexOfNeuron(Layer* layer, Neuron* target){

    Neuron* colTop = layer->topLeft;

    for(int col = 0; col < layer->N; col = col + 1){

        if(colTop == nullptr){

            return -1; 

        }

        Neuron* cell = colTop;

        while(cell != nullptr){

            if(cell == target){

                return col;

            }

            cell = cell->down;

        }

        colTop = colTop->right;

    }

    return -1;

}



void NeuroMesh::resyncColumnHorizontalLinks(Layer* layer, int colIndex){

    

    Neuron* thisColTop = layer->topLeft;

    for(int i = 0; i < colIndex; i = i + 1){

        if(thisColTop == nullptr){

            return; 

        }

        thisColTop = thisColTop->right;

    }

    

    int occupiedRows = 0;

    Neuron* counter = thisColTop;

    while(counter != nullptr){

        occupiedRows = occupiedRows + 1;
        counter = counter->down;

    }

    

    Neuron* cell = thisColTop;

    for(int row = 0; row < layer->N; row = row + 1){

        
        
        

        Neuron* correctLeft = nullptr;

        if(colIndex > 0){

            Neuron* leftColTop = layer->topLeft;
            bool leftColExists = true;

            for(int i = 0; i < colIndex - 1; i = i + 1){

                if(leftColTop == nullptr){

                    leftColExists = false;
                    break;

                }

                leftColTop = leftColTop->right;

            }

            if(leftColTop == nullptr){

                leftColExists = false;

            }

            if(leftColExists){

                Neuron* walker = leftColTop;
                bool walkerOk = true;

                for(int i = 0; i < row; i = i + 1){

                    if(walker == nullptr){

                        walkerOk = false;
                        break;

                    }

                    walker = walker->down;

                }

                if(walker == nullptr){

                    walkerOk = false;

                }

                if(walkerOk){

                    correctLeft = walker;

                }

            }

        }

        

        Neuron* correctRight = nullptr;

        if(colIndex < layer->N - 1){

            Neuron* rightColTop = layer->topLeft;
            bool rightColExists = true;

            for(int i = 0; i < colIndex + 1; i = i + 1){

                if(rightColTop == nullptr){

                    rightColExists = false;
                    break;

                }

                rightColTop = rightColTop->right;

            }

            if(rightColTop == nullptr){

                rightColExists = false;

            }

            if(rightColExists){

                Neuron* walker = rightColTop;
                bool walkerOk = true;

                for(int i = 0; i < row; i = i + 1){

                    if(walker == nullptr){

                        walkerOk = false;
                        break;

                    }

                    walker = walker->down;

                }

                if(walker == nullptr){

                    walkerOk = false;

                }

                if(walkerOk){

                    correctRight = walker;

                }

            }

        }

        if(row < occupiedRows){

            
            
            

            cell->left = correctLeft;
            cell->right = correctRight;

            if(correctLeft != nullptr){

                correctLeft->right = cell;

            }

            if(correctRight != nullptr){

                correctRight->left = cell;

            }

            cell = cell->down;

        }
        else{

            
            
            
            

            if(correctLeft != nullptr){

                correctLeft->right = nullptr;

            }

            if(correctRight != nullptr){

                correctRight->left = nullptr;

            }

        }

    }

}

void NeuroMesh::removeSynapsesTargeting(Neuron* target) {

    if (!target) 
    return;
    Layer* lay = headLayer;

    while (lay != nullptr) {

        Neuron* colTop = lay->topLeft;

        while (colTop != nullptr) {

            Neuron* curr = colTop;

            while (curr != nullptr) {

                Synapse* prevSyn = nullptr;

                Synapse* syn = curr->headAxon;

                while (syn != nullptr) {

                    if (syn->targetNeuron == target) {

                        Synapse* toDelete = syn;

                        if (prevSyn == nullptr) {

                            curr->headAxon = syn->nextSynapse;

                            syn = curr->headAxon;

                        } 
                        else {

                            prevSyn->nextSynapse = syn->nextSynapse;

                            syn = prevSyn->nextSynapse;

                        }

                        delete toDelete;

                    } 
                    else {
                        prevSyn = syn;
                        syn = syn->nextSynapse;
                    }

                }

                curr = curr->down;

            }
            colTop = colTop->right;

        }
        lay = lay->next;

    }
}


void NeuroMesh::clearLayerOutgoingSynapses(Layer* layer) {
    if (!layer)
     return;
    Neuron* colTop = layer->topLeft;
    while (colTop != nullptr) {

        Neuron* curr = colTop;

        while (curr != nullptr) {

            Synapse* s = curr->headAxon;


            while (s != nullptr) {

                Synapse* nextS = s->nextSynapse;

                delete s;

                s = nextS;

            }
            curr->headAxon = nullptr;

            curr = curr->down;

        }
        colTop = colTop->right;
    }
}


void NeuroMesh::deleteNeuron(int id) {

    Neuron* target = findNeuronById(id);

    if (!target) {

        cout << "Neuron id " << id << " not found." << endl;

        return;

    }

    Layer* layer = locateLayerOfNeuron(target);

    if (!layer)
     return;

    int colIdx = findColumnIndexOfNeuron(layer, target);

    

    removeSynapsesTargeting(target);

    

    Synapse* s = target->headAxon;

    while (s != nullptr) {

        Synapse* nextS = s->nextSynapse;

        delete s;

        s = nextS;

    }


    target->headAxon = nullptr;

    
    Neuron* above = target->up;

    Neuron* below = target->down;


    if (below != nullptr) {
        

        below->up = above;

        if (above != nullptr) {
            
            above->down = below;

        }
         else {
            
            below->left = target->left;
            
            below->right = target->right;

            if (target->left != nullptr) 

            target->left->right = below;

            if (target->right != nullptr)

             target->right->left = below;

            if (colIdx == 0)

             layer->topLeft = below;

        }

    } 
    else {
        
        if (above != nullptr) {

            above->down = nullptr;
            
        } else {
            
            if (target->left != nullptr) 

            target->left->right = target->right;

            if (target->right != nullptr) 

            target->right->left = target->left;

            if (colIdx == 0)

             layer->topLeft = target->right;

        }
    }

    delete target;

    layer->currentCount--;
    

    
    if (colIdx >= 0) 
    {

        resyncColumnHorizontalLinks(layer, colIdx);

    }

    
    rewireLayerSynapses(getPreviousLayer(layer));

    rewireLayerSynapses(layer);

    
    if (layer->currentCount <= 0) {

        removeEmptyLayer(layer);

    } 
    else {

        enforceRules();

    }
}



void NeuroMesh::deleteLayer(int layerId) {
    Layer* target = headLayer;
    while (target != nullptr && target->layerId != layerId) {
        target = target->next;
    }
    if (!target) {
        cout << "Layer " << layerId << " does not exist." << endl;
        return;
    }

    Layer* prevLayer = getPreviousLayer(target);
    Layer* nextLayer = target->next;

    if (prevLayer) {
        clearLayerOutgoingSynapses(prevLayer);
    }

    Neuron* colTop = target->topLeft;
    while (colTop != nullptr) {
        Neuron* nextColTop = colTop->right;
        Neuron* curr = colTop;
        while (curr != nullptr) {
            Neuron* nextDown = curr->down;
            Synapse* syn = curr->headAxon;
            while (syn != nullptr) {
                Synapse* ns = syn->nextSynapse;
                delete syn;
                syn = ns;
            }
            delete curr;
            curr = nextDown;
        }
        colTop = nextColTop;
    }
    target->topLeft = nullptr;
    target->currentCount = 0;

    if (target == headLayer) {
        headLayer = nextLayer;
    } else if (prevLayer) {
        prevLayer->next = nextLayer;
    }

    if (target == tailLayer) {
        tailLayer = prevLayer;
    }

    delete target;

    if (prevLayer && nextLayer) {
        rewireLayerSynapses(prevLayer);
    }

    enforceRules();
}




void NeuroMesh::removeEmptyLayer(Layer* l) {
    if (!l) return;
    Layer* prevLayer = getPreviousLayer(l);
    Layer* nextLayer = l->next;

    if (prevLayer) {
        clearLayerOutgoingSynapses(prevLayer);
    }

    if (l == headLayer) {
        headLayer = nextLayer;
    } else if (prevLayer) {
        prevLayer->next = nextLayer;
    }

    if (l == tailLayer) {
        tailLayer = prevLayer;
    }

    delete l;

    if (prevLayer && nextLayer) {
        rewireLayerSynapses(prevLayer);
    }
}




Layer* NeuroMesh::getPreviousLayer(Layer* layer) {
    if (!layer || layer == headLayer) return nullptr;
    Layer* curr = headLayer;
    while (curr != nullptr && curr->next != layer) {
        curr = curr->next;
    }
    return curr;
}




int NeuroMesh::getLayerIndex(Layer* layer) {
    int idx = 0;
    Layer* curr = headLayer;
    while (curr != nullptr) {
        if (curr == layer) return idx;
        curr = curr->next;
        idx++;
    }
    return -1;
}




Layer* NeuroMesh::getLayerByIndex(int layerIndex) {
    int idx = 0;
    Layer* curr = headLayer;
    while (curr != nullptr) {
        if (idx == layerIndex) return curr;
        curr = curr->next;
        idx++;
    }
    return nullptr;
}




int NeuroMesh::getColumnCount(Layer* layer) {
    if (!layer || !layer->topLeft) return 0;
    int count = 0;
    Neuron* colTop = layer->topLeft;
    while (colTop != nullptr) {
        count++;
        colTop = colTop->right;
    }
    return count;
}




Neuron* NeuroMesh::getColumnTop(Layer* layer, int colIndex) {
    if (!layer || colIndex < 0) return nullptr;
    Neuron* curr = layer->topLeft;
    for (int i = 0; i < colIndex && curr != nullptr; i++) {
        curr = curr->right;
    }
    return curr;
}




double NeuroMesh::getColumnWeight(Neuron* colTop) {
    double total = 0.0;
    Neuron* curr = colTop;
    while (curr != nullptr) {
        total += curr->weight;
        curr = curr->down;
    }
    return total;
}




Neuron* NeuroMesh::getNeuronAt(Layer* layer, int row, int col) {
    if (!layer || row < 0 || col < 0 || row >= this->N || col >= this->N) return nullptr;
    Neuron* colTop = getColumnTop(layer, col);
    if (!colTop) return nullptr;
    Neuron* curr = colTop;
    for (int r = 0; r < row && curr != nullptr; r++) {
        curr = curr->down;
    }
    return curr;
}




int NeuroMesh::getNeuronRow(Layer* layer, Neuron* target) {
    if (!layer || !target) return -1;
    int col = findColumnIndexOfNeuron(layer, target);
    if (col < 0) return -1;
    Neuron* curr = getColumnTop(layer, col);
    int r = 0;
    while (curr != nullptr) {
        if (curr == target) return r;
        curr = curr->down;
        r++;
    }
    return -1;
}




void NeuroMesh::swapColumns(Layer* layer, int colA, int colB) {
    if (!layer || colA == colB) return;
    if (colA > colB) { int tmp = colA; colA = colB; colB = tmp; }
    if (colB != colA + 1) return;

    
    Neuron* colANeurons[6];
    Neuron* colBNeurons[6];
    for (int r = 0; r < this->N; r++) {
        colANeurons[r] = nullptr;
        colBNeurons[r] = nullptr;
    }

    Neuron* currA = getColumnTop(layer, colA);
    int rA = 0;
    while (currA != nullptr && rA < this->N) {
        colANeurons[rA++] = currA;
        currA = currA->down;
    }

    Neuron* currB = getColumnTop(layer, colB);
    int rB = 0;
    while (currB != nullptr && rB < this->N) {
        colBNeurons[rB++] = currB;
        currB = currB->down;
    }

    
    Neuron* leftNeurons[6];
    Neuron* rightNeurons[6];
    for (int r = 0; r < this->N; r++) {
        leftNeurons[r] = (colA > 0) ? getNeuronAt(layer, r, colA - 1) : nullptr;
        rightNeurons[r] = getNeuronAt(layer, r, colB + 1);
    }

    
    for (int r = 0; r < this->N; r++) {
        Neuron* l = leftNeurons[r];
        Neuron* b = colBNeurons[r];
        Neuron* a = colANeurons[r];
        Neuron* rt = rightNeurons[r];

        if (l) l->right = b ? b : a ? a : rt;
        if (b) {
            b->left = l;
            b->right = a ? a : rt;
        }
        if (a) {
            a->left = b ? b : l;
            a->right = rt;
        }
        if (rt) rt->left = a ? a : b ? b : l;
    }

    if (colA == 0) {
        layer->topLeft = colBNeurons[0] ? colBNeurons[0] : colANeurons[0];
    }

    
    for (int c = 0; c < getColumnCount(layer); c++) {
        resyncColumnHorizontalLinks(layer, c);
    }

    rewireLayerSynapses(getPreviousLayer(layer));
    rewireLayerSynapses(layer);
}




void NeuroMesh::rewireLayerSynapses(Layer* layer) {
    if (!layer || !layer->next) return;
    Layer* nextLay = layer->next;

    Neuron* colTop = layer->topLeft;
    int c = 0;
    while (colTop != nullptr) {
        Neuron* curr = colTop;
        int r = 0;
        while (curr != nullptr) {
            
            Synapse* s = curr->headAxon;
            while (s != nullptr) {
                Synapse* ns = s->nextSynapse;
                delete s;
                s = ns;
            }
            curr->headAxon = nullptr;

            
            
            int candRows[4] = { r, r, r - 1, r + 1 };
            int candCols[4] = { c - 1, c + 1, c, c };

            Synapse* tail = nullptr;
            for (int k = 0; k < 4; k++) {
                int tr = candRows[k];
                int tc = candCols[k];
                if (tr >= 0 && tr < this->N && tc >= 0 && tc < this->N) {
                    Neuron* target = getNeuronAt(nextLay, tr, tc);
                    if (target != nullptr) {
                        Synapse* newS = new Synapse();
                        newS->weight = (curr->weight + target->weight) / 4.0;
                        newS->isActive = false;
                        newS->targetNeuron = target;
                        newS->nextSynapse = nullptr;

                        if (curr->headAxon == nullptr) {
                            curr->headAxon = newS;
                        } else {
                            tail->nextSynapse = newS;
                        }
                        tail = newS;
                    }
                }
            }

            curr = curr->down;
            r++;
        }
        colTop = colTop->right;
        c++;
    }
}




void NeuroMesh::rewireAllSynapses() {
    Layer* lay = headLayer;
    while (lay != nullptr) {
        rewireLayerSynapses(lay);
        lay = lay->next;
    }
}





void NeuroMesh::sortColumns(Layer* layer) {
    if (!layer || !layer->topLeft) return;

    bool modified = true;
    while (modified) {
        modified = false;
        int cols = getColumnCount(layer);
        if (cols <= 1) break;

        
        for (int j = 0; j < cols - 1; j++) {
            Neuron* topA = getColumnTop(layer, j);
            Neuron* topB = getColumnTop(layer, j + 1);
            if (!topA || !topB) continue;

            double wA = getColumnWeight(topA);
            double wB = getColumnWeight(topB);

            if (fabs(wA - wB) < 1e-9) {
                mergeColumns(topA, topB);
                modified = true;
                break;
            }
        }
        if (modified) continue;

        
        for (int j = 0; j < cols - 1; j++) {
            Neuron* topA = getColumnTop(layer, j);
            Neuron* topB = getColumnTop(layer, j + 1);
            if (!topA || !topB) continue;

            double wA = getColumnWeight(topA);
            double wB = getColumnWeight(topB);

            if (wA < wB) {
                swapColumns(layer, j, j + 1);
                modified = true;
                break;
            }
        }
    }
}






Neuron* NeuroMesh::mergeColumns(Neuron* columnA, Neuron* columnB) {
    if (!columnA || !columnB) return nullptr;
    Layer* layer = locateLayerOfNeuron(columnA);
    if (!layer) return nullptr;

    int colA = findColumnIndexOfNeuron(layer, columnA);
    int colB = findColumnIndexOfNeuron(layer, columnB);
    if (colA < 0 || colB < 0) return nullptr;

    if (colA > colB) {
        int tmp = colA; colA = colB; colB = tmp;
    }
    
    if (colB != colA + 1) return nullptr;

    
    int totalCols = 0;
    Neuron* initialColTops[6];
    Neuron* walkTop = layer->topLeft;
    while (walkTop != nullptr && totalCols < this->N) {
        initialColTops[totalCols++] = walkTop;
        walkTop = walkTop->right;
    }

    
    Neuron* nA[6];
    Neuron* nB[6];
    for (int r = 0; r < this->N; r++) {
        nA[r] = getNeuronAt(layer, r, colA);
        nB[r] = getNeuronAt(layer, r, colB);
    }

    
    for (int r = 0; r < this->N; r++) {
        if (nA[r] != nullptr && nB[r] != nullptr) {
            
            nA[r]->id = (nA[r]->id < nB[r]->id) ? nA[r]->id : nB[r]->id;
            
            nA[r]->weight = floor((nA[r]->weight + nB[r]->weight) / 2.0);

            
            removeSynapsesTargeting(nB[r]);
            Synapse* s = nB[r]->headAxon;
            while (s != nullptr) {
                Synapse* ns = s->nextSynapse;
                delete s;
                s = ns;
            }
            delete nB[r];
            layer->currentCount--;
            nB[r] = nullptr;
        } else if (nA[r] == nullptr && nB[r] != nullptr) {
            
            nA[r] = nB[r];
            nB[r] = nullptr;
        }
    }

    
    Neuron* prev = nullptr;
    Neuron* newColTop = nullptr;
    for (int r = 0; r < this->N; r++) {
        if (nA[r] != nullptr) {
            if (newColTop == nullptr) newColTop = nA[r];
            nA[r]->up = prev;
            if (prev != nullptr) prev->down = nA[r];
            prev = nA[r];
        }
    }
    if (prev != nullptr) prev->down = nullptr;
    if (newColTop != nullptr) newColTop->up = nullptr;

    
    Neuron* activeTops[6];
    int topCount = 0;
    for (int c = 0; c < totalCols; c++) {
        if (c == colA) {
            if (newColTop != nullptr) activeTops[topCount++] = newColTop;
        } else if (c == colB) {
            
            continue;
        } else {
            activeTops[topCount++] = initialColTops[c];
        }
    }

    if (topCount > 0) {
        layer->topLeft = activeTops[0];
        for (int i = 0; i < topCount; i++) {
            activeTops[i]->left = (i > 0) ? activeTops[i - 1] : nullptr;
            activeTops[i]->right = (i < topCount - 1) ? activeTops[i + 1] : nullptr;
        }
    } else {
        layer->topLeft = nullptr;
    }

    
    for (int c = 0; c < topCount; c++) {
        resyncColumnHorizontalLinks(layer, c);
    }

    
    rewireLayerSynapses(getPreviousLayer(layer));
    rewireLayerSynapses(layer);

    return newColTop;
}






Layer* NeuroMesh::mergeLayers(Layer* a, Layer* b) {
    if (!a || !b || a->next != b) return a;

    
    Neuron* gridA[6][6];
    Neuron* gridB[6][6];
    for (int r = 0; r < this->N; r++) {
        for (int c = 0; c < this->N; c++) {
            gridA[r][c] = getNeuronAt(a, r, c);
            gridB[r][c] = getNeuronAt(b, r, c);
        }
    }

    
    for (int r = 0; r < this->N; r++) {
        for (int c = 0; c < this->N; c++) {
            Neuron* na = gridA[r][c];
            Neuron* nb = gridB[r][c];

            if (na != nullptr && nb != nullptr) {
                na->id = (na->id < nb->id) ? na->id : nb->id;
                na->weight = floor((na->weight + nb->weight) / 2.0);

                removeSynapsesTargeting(nb);
                Synapse* s = nb->headAxon;
                while (s != nullptr) {
                    Synapse* ns = s->nextSynapse;
                    delete s;
                    s = ns;
                }
                delete nb;
            } else if (na == nullptr && nb != nullptr) {
                
                nb->up = nullptr;
                nb->down = nullptr;
                nb->left = nullptr;
                nb->right = nullptr;

                
                Neuron* colTop = getColumnTop(a, c);
                if (colTop == nullptr) {
                    if (a->topLeft == nullptr) {
                        a->topLeft = nb;
                    } else {
                        Neuron* lastTop = a->topLeft;
                        while (lastTop->right != nullptr) lastTop = lastTop->right;
                        lastTop->right = nb;
                        nb->left = lastTop;
                    }
                } else {
                    Neuron* curr = colTop;
                    while (curr->down != nullptr) curr = curr->down;
                    curr->down = nb;
                    nb->up = curr;
                }
                a->currentCount++;
            }
        }
    }

    
    a->next = b->next;
    if (tailLayer == b) {
        tailLayer = a;
    }
    delete b;

    
    int cols = getColumnCount(a);
    for (int c = 0; c < cols; c++) {
        resyncColumnHorizontalLinks(a, c);
    }

    rewireLayerSynapses(getPreviousLayer(a));
    rewireLayerSynapses(a);

    return a;
}








Neuron* NeuroMesh::mergeNeurons(Synapse* s) {
    if (!s || !s->targetNeuron) return nullptr;

    Neuron* v = s->targetNeuron;
    Layer* layV = locateLayerOfNeuron(v);
    if (!layV) return nullptr;

    Layer* layU = getPreviousLayer(layV);
    if (!layU) return nullptr;

    
    Neuron* u = nullptr;
    Neuron* colTop = layU->topLeft;
    while (colTop != nullptr && !u) {
        Neuron* curr = colTop;
        while (curr != nullptr && !u) {
            Synapse* syn = curr->headAxon;
            while (syn != nullptr) {
                if (syn == s) {
                    u = curr;
                    break;
                }
                syn = syn->nextSynapse;
            }
            curr = curr->down;
        }
        colTop = colTop->right;
    }
    if (!u) return nullptr;

    
    u->id = (u->id < v->id) ? u->id : v->id;
    u->weight = floor((u->weight + v->weight) / 2.0);

    
    Neuron* vacatedSlotNeuron = v;
    Layer* currVacantLayer = layV;

    while (currVacantLayer != nullptr && vacatedSlotNeuron != nullptr) {
        Layer* nextLay = currVacantLayer->next;
        Neuron* bestCandidate = nullptr;

        if (nextLay != nullptr) {
            int vr = getNeuronRow(currVacantLayer, vacatedSlotNeuron);
            int vc = findColumnIndexOfNeuron(currVacantLayer, vacatedSlotNeuron);

            double maxWeight = -1e18;
            int bestPriority = 999;
            Synapse* syn = vacatedSlotNeuron->headAxon;
            while (syn != nullptr) {
                if (syn->targetNeuron != nullptr) {
                    int tr = getNeuronRow(nextLay, syn->targetNeuron);
                    int tc = findColumnIndexOfNeuron(nextLay, syn->targetNeuron);
                    int priority = 999;
                    if (tr == vr && tc == vc - 1) priority = 1;      // Left
                    else if (tr == vr && tc == vc + 1) priority = 2; // Right
                    else if (tr == vr - 1 && tc == vc) priority = 3; // Up
                    else if (tr == vr + 1 && tc == vc) priority = 4; // Down

                    if (syn->weight > maxWeight + 1e-9) {
                        maxWeight = syn->weight;
                        bestCandidate = syn->targetNeuron;
                        bestPriority = priority;
                    } else if (fabs(syn->weight - maxWeight) <= 1e-9) {
                        if (priority < bestPriority) {
                            bestCandidate = syn->targetNeuron;
                            bestPriority = priority;
                        }
                    }
                }
                syn = syn->nextSynapse;
            }
        }

        if (bestCandidate != nullptr) {
            
            vacatedSlotNeuron->id = bestCandidate->id;
            vacatedSlotNeuron->weight = bestCandidate->weight;

            
            vacatedSlotNeuron = bestCandidate;
            currVacantLayer = nextLay;
        } else {
            
            
            deleteNeuron(vacatedSlotNeuron->id);
            break;
        }
    }

    rewireAllSynapses();
    return u;
}









void NeuroMesh::enforceRules() {
    bool fired = true;
    while (fired) {
        fired = false;

        
        Layer* lay = headLayer;
        while (lay != nullptr && !fired) {
            Neuron* colTop = lay->topLeft;
            while (colTop != nullptr && !fired) {
                Neuron* curr = colTop;
                while (curr != nullptr && !fired) {
                    if (pruningThreshold > 0.0 && curr->weight > pruningThreshold) {
                        pruneNeuron(curr);
                        fired = true;
                        break;
                    }
                    curr = curr->down;
                }
                colTop = colTop->right;
            }
            lay = lay->next;
        }
        if (fired) continue;

        
        lay = headLayer;
        while (lay != nullptr && !fired) {
            Neuron* colTop = lay->topLeft;
            while (colTop != nullptr && !fired) {
                Neuron* curr = colTop;
                while (curr != nullptr && !fired) {
                    Synapse* s = curr->headAxon;
                    while (s != nullptr && !fired) {
                        if (fusionThreshold > 0.0 && s->weight > fusionThreshold) {
                            mergeNeurons(s);
                            fired = true;
                            break;
                        }
                        s = s->nextSynapse;
                    }
                    curr = curr->down;
                }
                colTop = colTop->right;
            }
            lay = lay->next;
        }
        if (fired) continue;

        
        lay = headLayer;
        while (lay != nullptr && !fired) {
            int prevCount = lay->currentCount;
            sortColumns(lay);
            if (lay->currentCount != prevCount) {
                fired = true;
                break;
            }
            lay = lay->next;
        }
        if (fired) continue;

        
        lay = headLayer;
        while (lay != nullptr && lay->next != nullptr && !fired) {
            double w1 = 0.0, w2 = 0.0;
            Neuron* c1 = lay->topLeft;
            while (c1 != nullptr) {
                w1 += getColumnWeight(c1);
                c1 = c1->right;
            }
            Neuron* c2 = lay->next->topLeft;
            while (c2 != nullptr) {
                w2 += getColumnWeight(c2);
                c2 = c2->right;
            }

            if (fabs(w1 - w2) < 1e-9 && lay->currentCount > 0 && lay->next->currentCount > 0) {
                mergeLayers(lay, lay->next);
                fired = true;
                break;
            }
            lay = lay->next;
        }
        if (fired) continue;

        
        lay = headLayer;
        while (lay != nullptr && !fired) {
            Layer* nextLay = lay->next;
            if (lay->currentCount <= 0) {
                removeEmptyLayer(lay);
                fired = true;
                break;
            }
            lay = nextLay;
        }
        if (fired)
         continue;
    }
}

void NeuroMesh::setSynapseWeight(int id, char dir, double newWeight) {
    Layer* lay = headLayer;
    Neuron* u = nullptr;
    int srcRow = -1, srcCol = -1;

    while (lay != nullptr && u == nullptr) {
        for (int r = 0; r < this->N; r++) {
            for (int c = 0; c < this->N; c++) {
                Neuron* n = getNeuronAt(lay, r, c);
                if (n != nullptr && n->id == id) {
                    u = n;
                    srcRow = r;
                    srcCol = c;
                    break;
                }
            }
            if (u != nullptr) break;
        }
        if (u != nullptr) break;
        lay = lay->next;
    }

    if (u == nullptr || lay == nullptr || lay->next == nullptr) {
        return;
    }

    int tr = srcRow;
    int tc = srcCol;
    if (dir == 'U' || dir == 'u') {
        tr = srcRow - 1;
    } else if (dir == 'D' || dir == 'd') {
        tr = srcRow + 1;
    } else if (dir == 'L' || dir == 'l') {
        tc = srcCol - 1;
    } else if (dir == 'R' || dir == 'r') {
        tc = srcCol + 1;
    } else {
        return;
    }

    if (tr < 0 || tr >= this->N || tc < 0 || tc >= this->N) {
        return;
    }

    Neuron* target = getNeuronAt(lay->next, tr, tc);
    if (target == nullptr) {
        return;
    }

    Synapse* s = u->headAxon;
    while (s != nullptr) {
        if (s->targetNeuron == target) {
            s->weight = newWeight;
            return;
        }
        s = s->nextSynapse;
    }

    Synapse* newS = new Synapse();
    newS->weight = newWeight;
    newS->isActive = false;
    newS->targetNeuron = target;
    newS->nextSynapse = nullptr;

    if (u->headAxon == nullptr) {
        u->headAxon = newS;
    } else {
        Synapse* tail = u->headAxon;
        while (tail->nextSynapse != nullptr) {
            tail = tail->nextSynapse;
        }
        tail->nextSynapse = newS;
    }
}


