

#include <iostream>
#include "map.h"
#include "visualizer.h"

using namespace std;

void runTerminalMenu(NeuroMesh &neuralMesh)
{

    int menuChoice = -1; // as for unknown -1 is standered

    cout << "\n";
    cout << "////////////////////////////////////////////////////////////////////////////////" << endl;
    cout << "// NeuroMesh Terminal Menu                                                   //" << endl;
    cout << "// Assignment 01 - Question 01                                              //" << endl;
    cout << "////////////////////////////////////////////////////////////////////////////////" << endl;

    while (menuChoice != 0)
    {

        cout << "\n";
        cout << "  -------------------------------------------------------" << endl;
        cout << "  [1]  Load from file" << endl;
        cout << "  [2]  Insert neuron (id, weight)" << endl;
        cout << "  [3]  Delete neuron (id)" << endl;
        cout << "  [4]  Delete layer (layerId)" << endl;
        cout << "  [5]  Prune neuron (id)" << endl;
        cout << "  [6]  Merge neurons via synapse (id)" << endl;
        cout << "  [7]  Merge columns (col indices)" << endl;
        cout << "  [8]  Merge layers (layer indices)" << endl;
        cout << "  [9]  Print layer (index)" << endl;
        cout << "  [10] Print neuron (id)" << endl;
        cout << "  [11] Find neuron by ID" << endl;
        cout << "  [12] Find neuron by position (layer, row, col)" << endl;
        cout << "  [13] Navigate mesh (arrow keys)" << endl;
        cout << "  [14] Export mesh to file" << endl;
        cout << "  [15] Forward propagate (letter)" << endl;
        cout << "  [16] Backward propagate (target letter)" << endl;
        cout << "  [0]  Exit terminal menu" << endl;
        cout << "  -------------------------------------------------------" << endl;
        cout << "  Enter choice: ";

        cin >> menuChoice;


        if (cin.fail()) //check weather the previous input is write or wrong for example if i type abc in int it will give error 

        {

            cout <<" Not the proper type \n";

            cin.clear();

            // clear the buffer and remove /n

            cin.ignore(1000, '\n');

            menuChoice = -1;

        }

        cout << "\n";


        if (menuChoice == 1)
        {

            ////////////////////////
            //  Load from file
            ///////////////////////

            string fileName;

            cout << "  Enter file path: ";

            cin >> fileName;

            neuralMesh.loadFromFile(fileName);

            cout << "  Mesh loaded." << endl;

        }
        else if (menuChoice == 2)
        {

            ////////////////////
            // Insert neuron
            ///////////////////


            int neuronId = 0;
            double neuronWeight = 0.0;
            cout << "  Enter ID: ";
            cin >> neuronId;
            cout << "  Enter weight: ";
            cin >> neuronWeight;
            neuralMesh.insertNeuron(neuronId, neuronWeight);
            cout << "  Inserted neuron " << neuronId << " with weight " << neuronWeight << endl;
        }
        else if (menuChoice == 3)
        {
 
            //////////////////
            // Delete neuron
            /////////////////


            int neuronId = 0;

            cout << "  Enter neuron ID to delete: ";

            cin >> neuronId;

            neuralMesh.deleteNeuron(neuronId);

            cout << "  Deleted neuron " << neuronId << endl;

        }
        else if (menuChoice == 4)
        {

            ///////////////////
            // Delete layer
            ////////////////////

            int layerIdentifier = 0;

            cout << "  Enter layer ID to delete: ";

            cin >> layerIdentifier;

            neuralMesh.deleteLayer(layerIdentifier);

            cout << "  Deleted layer " << layerIdentifier << endl;


        }
        else if (menuChoice == 5)
        {

            //////////////////////////
            // Prune neuron
            //////////////////////////

            int neuronId = 0;

            cout << "  Enter neuron ID to prune: ";

            cin >> neuronId;

            Neuron *targetNeuron = neuralMesh.findNeuronById(neuronId);

            if (targetNeuron != nullptr)
            {
                neuralMesh.pruneNeuron(targetNeuron);

                cout << "  Pruned neuron " << neuronId << endl;

            }
            else
            {

                cout << "  Neuron " << neuronId << " not found." << endl;


            }
        }
        else if (menuChoice == 6)
        {

             ///////////////////////////
            // Merge neurons via synapse
            /////////////////////////////

            cout << "  Enter neuron ID whose highest-weight synapse to trigger: ";

            int neuronId = 0;

            cin >> neuronId;

            Neuron *sourceNeuron = neuralMesh.findNeuronById(neuronId);

            if (sourceNeuron != nullptr && sourceNeuron->headAxon != nullptr)
            {
                 //////////////////////////////////
                // Find highest-weight synapse
                //////////////////////////////////


                Synapse *bestSynapse = sourceNeuron->headAxon;

                Synapse *synapseRunner = sourceNeuron->headAxon->nextSynapse;


                while (synapseRunner != nullptr)
                {

                    if (synapseRunner->weight > bestSynapse->weight)
                    {

                        bestSynapse = synapseRunner;
                    }

                    synapseRunner = synapseRunner->nextSynapse;

                }

                neuralMesh.mergeNeurons(bestSynapse);

                cout << "  Merge neurons executed." << endl;

            }

            else
            {

                cout << "  Neuron not found or no outgoing synapses." << endl;

            }
        }
        else if (menuChoice == 7)
        {

            // Merge columns

            int columnIndexA = 0;

            int columnIndexB = 0;

            cout << "  Enter column index A (0-based): ";

            cin >> columnIndexA;

            cout << "  Enter column index B (0-based): ";

            cin >> columnIndexB;


            Layer *targetLayer = neuralMesh.getHeadLayer();

            if (targetLayer == nullptr)
            {

                cout << "  Mesh is empty." << endl;

            }

            else
            {

                Neuron *columnHeadA = neuralMesh.getColumnTop(targetLayer, columnIndexA);

                Neuron *columnHeadB = neuralMesh.getColumnTop(targetLayer, columnIndexB);


                if (columnHeadA != nullptr && columnHeadB != nullptr)
                {

                    neuralMesh.mergeColumns(columnHeadA, columnHeadB);

                    cout << "  Columns merged." << endl;

                }
                else
                {

                    cout << "  Column index out of bounds." << endl;


                }
            }
        }
        else if (menuChoice == 8)
        {

            // Merge layers

            int layerIndexA = 0;

            int layerIndexB = 0;

            cout << "  Enter layer index A (0-based): ";

            cin >> layerIndexA;

            cout << "  Enter layer index B (0-based, must be adjacent to A): ";

            cin >> layerIndexB;

            Layer *headLayer = neuralMesh.getHeadLayer();

            Layer *layerA = headLayer;


            // Walk to layerA
            
            for (int offset = 0; offset < layerIndexA && layerA != nullptr; offset++)
            {
                layerA = layerA->next;
            }

            Layer *layerB = headLayer;

            for (int offset = 0; offset < layerIndexB && layerB != nullptr; offset++)
            {
                layerB = layerB->next;
            }

            if (layerA != nullptr && layerB != nullptr)
            {

                neuralMesh.mergeLayers(layerA, layerB);

                cout << "  Layers merged." << endl;

            }
            else
            {

                cout << "  Layer index out of bounds." << endl;

            }
        }
        else if (menuChoice == 9)
        {

            // Print layer
            int layerIndex = 0;

            cout << "  Enter layer index (0-based): ";

            cin >> layerIndex;

            neuralMesh.printLayer(layerIndex);


        }
        else if (menuChoice == 10)
        {

            // Print neuron

            int neuronId = 0;

            cout << "  Enter neuron ID: ";

            cin >> neuronId;

            neuralMesh.printNeuron(neuronId);


        }
        else if (menuChoice == 11)
        {

            // Find neuron by ID

            int neuronId = 0;
            
            cout << "  Enter neuron ID: ";

            cin >> neuronId;

            Neuron *foundNeuron = neuralMesh.findNeuronById(neuronId);


            if (foundNeuron != nullptr)
            {

                cout << "  Neuron " << neuronId << " found. Weight=" << foundNeuron->weight << endl;


            }
            else
            {

                cout << "  Neuron " << neuronId << " not found." << endl;


            }
        }
        else if (menuChoice == 12)
        {

            // Find neuron by position

            int layerIndex = 0;

            int rowIndex = 0;

            int columnIndex = 0;

            cout << "  Enter layer index: ";

            cin >> layerIndex;

            cout << "  Enter row index: ";

            cin >> rowIndex;

            cout << "  Enter column index: ";

            cin >> columnIndex;


            Neuron *foundNeuron = neuralMesh.findNeuronByPosition(layerIndex, rowIndex, columnIndex);

            if (foundNeuron != nullptr)
            {

                cout << "  Found ID=" << foundNeuron->id << " Weight=" << foundNeuron->weight << endl;


            }
            else
            {

                cout << "  Slot is vacant." << endl;


            }
        }
        else if (menuChoice == 13)
        {

            // Navigate mesh
            neuralMesh.navigateMesh();


        }
        else if (menuChoice == 14)
        {

            // Export mesh
            string exportFileName;

            cout << "  Enter output file name: ";

            cin >> exportFileName;

            neuralMesh.exportMesh(exportFileName);


        }
        else if (menuChoice == 15)
        {

            // Forward propagate
            char inputLetter = 'A';

            cout << "  Enter input letter (A-Z): ";

            cin >> inputLetter;

            char outputLetter = neuralMesh.forwardPropagate(inputLetter);

            cout << "  Output letter: " << outputLetter << endl;


        }
        else if (menuChoice == 16)
        {

            // Backward propagate
            char targetLetter = 'A';

            cout << "  Enter target letter (A-Z): ";

            cin >> targetLetter;

            neuralMesh.backwardPropagate(targetLetter);

            cout << "  Backward propagation complete." << endl;


        }
        else if (menuChoice == 0)
        {

            cout << "  Exiting terminal menu." << endl;

        }
        else
        {

            cout << "  Invalid choice. Please try again." << endl;

            menuChoice = -1;


        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//
// Visualizaton
//
////////////////////////////////////////////////////////////////////////////////

int main()
{

    cout << "\n";

    cout << "////////////////////////////////////////////////////////////////////////////////" << endl;

    cout << "// ---------------- 01------------------                                     //" << endl;

    cout << "// FAST NUCES | Fall 2026 | Data Structures Assignment 01                     //" << endl;

    cout << "// Made by Ahmad Ali | i250817                                                  //" << endl;

    cout << "////////////////////////////////////////////////////////////////////////////////" << endl;

    cout << "\n";

    cout << "  Select mode:" << endl;

    cout << "    [1] Terminal Menu" << endl;

    cout << "    [2] SFML Graphical Visualizer (Bonus)" << endl;

    cout << "\n";

    cout << "  Enter choice: ";

    int modeChoice = 0;

    cin >> modeChoice;

    cout << "\n";

    NeuroMesh neuralMesh;

    if (modeChoice == 1)
    {

        ///////////////////////

        // Just run the terminal mode

        /////////////////////

        runTerminalMenu(neuralMesh);
    }
    else if (modeChoice == 2)
    {

        ///////////////////////////

        // else bonus

        /////////////////////////

        MeshVisualizer visualizer;

        visualizer.run(neuralMesh);
    }
    else
    {

        cout << "Invalid choice.Yar na kro yar." << endl;

        runTerminalMenu(neuralMesh);
    }

    return 0;
}
