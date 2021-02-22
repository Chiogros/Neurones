#include <iostream>
#include <cstdlib>
#include "neurone.h"
#include "mind.h"
#include "NUMBER.h"
#include "IHM.h"

#define DEAD 0

using namespace std;


int main(int argc, char **argv) {

    string chemin;
    
    if (argc > 1) {
        chemin = static_cast<string>(argv[1]);
    } else {
        chemin = "./Neurons/";
    }
    
    Mind *mind = new Mind(chemin);

    IHM ihm(mind);
    
    delete mind;

    return DEAD;
    
}
