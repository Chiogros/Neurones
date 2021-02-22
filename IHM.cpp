#include "IHM.h" 



IHM::IHM(Mind *mind) {
    
    this->SetupWithDedicatedNeurons(mind);

}



IHM::~IHM() {
    
    
    
}



void IHM::SetupWithDedicatedNeurons(Mind *mind) {
    
    string pathDedicated = mind->getMindPath() + "dedicated";

    NUMBER i = 0;
    string neuronName;
    
    for(auto& fNeuron: filesystem::directory_iterator(pathDedicated)) {
        if (i < numeric_limits<NUMBER>::max()) {
            neuronName = static_cast<string>(fNeuron.path());
            this->dedicatedName[i] = neuronName.substr(neuronName.find(".IQ")-8, 8);
            cout << dedicatedName[i] << " added as a dedicated neuron" << endl;
            i++;
        } else {
            cout << "Too much neurons in " << pathDedicated << " folder : " << i << " for " << numeric_limits<NUMBER>::max() << " allowed." << endl;
            exit(1);
        }
    }
    
    
    
}



void IHM::ReadCharacters(Mind *mind) {
    
    char phrase[] = "D";
    string inputNeuron = "obhmafcv";
    
    while (1) {
        
        
        
    }
    
    
}



void IHM::WriteCharacters(char valueASCII) {
    cout << static_cast<char>(valueASCII);
}
