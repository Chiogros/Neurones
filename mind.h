#ifndef MIND_H
#define MIND_H

#include <iostream>
#include <limits>
#include <filesystem>
#include <ctime>
#include <cstdlib>
#include <string>
#include "neurone.h"
#include "NUMBER.h"

using namespace std;

class Mind {
    
public:
    Mind(string mindPath);
    ~Mind();
    string createNeuron(NeuronType typeOfNeuron = association); // retourne le n° du neuron créé
    void organisation(string inputName);
    string loadNeuronsFromFiles();
    string getMindPath();
    
private:
    void createNeuronsDirectories();
    int numberOfFilesInDirectory(string path);
    string getRandomName();
    string setMindPath(string mindPath); 
    string generateMinimalNeurons();
    //
    string neuronsName[numeric_limits<NUMBER>::max()]; // contient les noms des neurons, triés
    string mindPath;
};


#endif
