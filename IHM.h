#ifndef IHM_H
#define IHM_H

#include <limits>
#include <filesystem>
#include "mind.h"
#include "NUMBER.h"

using namespace std;

class IHM {
    
public:
    IHM(Mind *mind);
    ~IHM();
    void SetupWithDedicatedNeurons(Mind *mind);
    void ReadCharacters(Mind *mind);
    void WriteCharacters(char valueASCII);
private:
    string dedicatedName[numeric_limits<NUMBER>::max()];
};


#endif
