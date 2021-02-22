#ifndef NEURONE_H
#define NEURONE_H

#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstring>
#include <limits>
#include <sstream>
#include "NUMBER.h"

using namespace std;

enum NeuronType { association, dedicated_input, dedicated_output };

class Neuron {
    
public:
    Neuron(string name, string mindPath, NeuronType typeOfNeuron = association, NUMBER activationLimit = 1000);
    ~Neuron();
    string getNeuronID();
    void setNeuronID(string newID);
    void addNewConnexion(bool InOrOutToAdd, string neuronName);
    NeuronType getNeuronType();
    void setNeuronType(NeuronType typeOfNeuron);
    bool Update();
    
private:
    void setMindPath(string mindPath); 
    void SetupWithNeuronFile();
    bool CheckNeuronFileExists(string NeuronFileName);
    bool CreateNeuronFile();
    bool LoadNeuronFromFile();
    string getNeuroneFileName();
    bool EditNeuroneFile(bool InOrOut);
    NUMBER getActivationLimit();
    void setActivationLimit(NUMBER newLimit);
    //
    string ID;
    NeuronType Type;
    string In[IOconnexions];
    NUMBER factorIn[IOconnexions];
    string Out[IOconnexions];
    string mindPath;
    NUMBER activationLimit;
    bool InActivated[IOconnexions];
};


#endif
