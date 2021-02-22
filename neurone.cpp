#include "neurone.h"


// PUBLIC

Neuron::Neuron(string name, string mindPath, NeuronType typeOfNeuron, NUMBER activationLimit) {
    
    // Définit les attributs
    this->setMindPath(mindPath);
    this->setNeuronID(name);
    this->setNeuronType(typeOfNeuron);
    this->setActivationLimit(activationLimit);
    
    this->SetupWithNeuronFile();
    
}



Neuron::~Neuron() {
    
    
    
}



string Neuron::getNeuronID() {
    
    return this->ID;
}



void Neuron::setNeuronID(string newID) {
    
    this->ID = newID;

}



void Neuron::addNewConnexion(bool InOrOutToAdd, string neuronName) {  // neuronName est celui qui appelle cette fonction
    
    ifstream fNeuronOriginal(this->getNeuroneFileName());
    ofstream fNewNeuron(this->getNeuroneFileName() + "_copy");
    
    stringstream bufferFile;
    string bufferLineAnalysis;
    bool currentConnexionType = true;
    
    
    if (fNeuronOriginal.is_open() && fNewNeuron.is_open()) {
        
        bufferFile << fNeuronOriginal.rdbuf();
        
        for(int i = 0 ; i < 5 ; i++) {  // passer les premières lignes du fichier, on arrive au - In[500]
            getline(bufferFile, bufferLineAnalysis);
        }
    
        while (!bufferFile.eof()) {    // va passer toutes les lignes du buffer

            getline(bufferFile, bufferLineAnalysis);    // lit la ligne suivante
            
            if (!bufferLineAnalysis.empty()) {  // la ligne ne doit pas être vide pour pouvoir être analysée
                
                if (bufferLineAnalysis.find("In") != string::npos) {  // teste si c'est le titre In
                    currentConnexionType = true;
                } else if (bufferLineAnalysis.find("Out") != string::npos) {  // teste si c'est le titre Out
                    currentConnexionType = false;
                    
                } else {    // donc c'est une ligne avec des valeurs
                    
                    if (currentConnexionType == InOrOutToAdd) {     // chercher dans parmi le bon type de connexions
                    
                        if (bufferLineAnalysis.find("nonenone") != string::npos) {    // une place de libre pour une nouvelle connexion
                            
                            bufferFile.seekp(bufferFile.tellg());   // place la tête d'écriture à la position de lecture dans le bufferFile
                            bufferFile.seekp( -bufferLineAnalysis.length(), ios_base::cur );    // déplace la tête d'écriture une ligne en arrière puisque la tête de lecture est une ligne après
                            bufferFile.seekp( bufferLineAnalysis.find("nonenone")-1, ios_base::cur );   // déplace la tête là où il y a le nom du neurone
                            bufferFile.write(neuronName.c_str(), neuronName.length());  // écrit le nouveau nom
                            break;  // on arrête de parcourir tout le fichier
                            
                        }
                    }
                }
            }
        }
        
    } else {
        cout << "error: in addNewIn method : le fichier neurone" << this->getNeuroneFileName() << "n'est pas ouvert" << endl;
    }
    
    
    fNewNeuron << bufferFile.str();
    
    fNeuronOriginal.close();
    fNewNeuron.close();
    
    filesystem::rename(this->getNeuroneFileName() + "_copy", this->getNeuroneFileName());
    
}



NeuronType Neuron::getNeuronType() {
    
    return this->Type;
    
}



void Neuron::setNeuronType(NeuronType typeOfNeuron) {
    
    this->Type = typeOfNeuron;

}



bool Neuron::Update() {
    
    NUMBER calculateNewActivationLevel = 0;
    for(NUMBER i = 0 ; i < IOconnexions ; i++) {
        if (InActivated[i]) 
            calculateNewActivationLevel += factorIn[i];
    }
    
    bool activated = false;
    if (activationLevel >= activationLimit) {
        activated = true;
//         appeler une fonction qui va écrire "true" dans chaque connexion In[] de chaque neurone connecté en Out 
        
    }
    
    return activated;
    
}






// PRIVATE


void Neuron::setMindPath(string mindPath) {

    this->mindPath = mindPath;
    
}



void Neuron::SetupWithNeuronFile() {    // check si le neurone existe avec son nom, le créé ou le charge
    
    if (CheckNeuronFileExists(this->getNeuroneFileName())) {    // le neurone existe, charger en mémoire
        cout << "Neuron " << this->ID << " exists, loading..." << endl;
        this->LoadNeuronFromFile();        
    } else {    // le neurone n'existe pas, création
        cout << "Neuron " << this->ID << " doesn't exist, creating..." << endl;
        this->CreateNeuronFile();
    }
    
}



bool Neuron::CheckNeuronFileExists(string NeuronFileName) {     // check si le fichier neurone existe
    
    return filesystem::exists(NeuronFileName);
}



bool Neuron::CreateNeuronFile() {   // créé un fichier neurone (qui par conséquent n'existe pas)

    // créé le fichier
    ofstream fNeuron(this->getNeuroneFileName());
    
    if (!fNeuron.is_open()) {
        cout << "error: in CreateNeuronFile method : impossible de créer le fichier neurone";
        return false;
    }
    
    // écrit dans le fichier
    fNeuron << "Neuron type : " << this->Type << endl;
    fNeuron << "Activation limit : " << 1000 << endl << endl;
    
    // il y a des connexions entrantes et sortantes que quand c'est un type association
    fNeuron << "Connexions : " << endl;
    fNeuron << "// [number] = name - activated - factor" << endl;
    
    if (this->Type == association || this->Type == dedicated_output) {      // entrées que sur neurones association et dedicated de sortie
        fNeuron << "- In[" << sizeof(this->In) / sizeof(this->In[0]) << "]" << endl;    // écrit toutes les connexions entrantes (In) + le facteur d'importance de chaque connexion
        for (NUMBER i = 0 ; i < (sizeof(this->In) / sizeof(this->In[0])) ; i++) { 
            In[i] = "nonenone";
            fNeuron << "[" << i << "] = " << In[i];     // https://stackoverflow.com/questions/1714515/how-can-i-pad-an-int-with-leading-zeros-when-using-cout-operator
            fNeuron << " - false";
            fNeuron << " - " << factorIn[i] << endl;
        }
        fNeuron << endl;
    }

    
    
    if (this->Type == association || this->Type == dedicated_input) {   // sorties que sur neurones association et dedicated d'entrée
        fNeuron << "- Out[" << sizeof(this->In) / sizeof(this->In[0]) << "]" << endl;   // écrit toutes les connexions sortantes (Out) sans le facteur de sortie 
        for (NUMBER i = 0 ; i < (sizeof(this->Out) / sizeof(this->Out[0])) ; i++) {
            Out[i] = "nonenone";
            fNeuron << "[" << i << "] = " << Out[i] << endl;
        }
    }
    
    fNeuron.close();
    
    // message de validation de la création du fichier
    cout << "Neuron " << this->ID << " created in " << this->getNeuroneFileName() << "." << endl;
    return true;
}



bool Neuron::LoadNeuronFromFile() {   // charge le fichier dans un neurone
    
    ifstream fNeuron(this->getNeuroneFileName());
    
    if (!fNeuron.is_open()) {
        cout << "error: in LoadNeuronFromFile method : le fichier neurone n'est pas ouvert" << endl;
    } else {
    
        string bufferAnalysis;
        bool InOrOut = true; // In = true, Out = false
        int beginBracket, endBracket, numberOfIO;
        
        while (!fNeuron.eof()) {    // va passer toutes les lignes du fichier et enregistrer à chaque ligne le nom du neurone et son facteur

            getline(fNeuron, bufferAnalysis);
                
            if (!bufferAnalysis.empty() || bufferAnalysis.find("//") != string::npos) {  // teste si la ligne qu'on lit est pleine OU que ce n'est pas un commentaire
                
                if (bufferAnalysis.find("Neuron type : ") != string::npos) {
                
                } else if (bufferAnalysis.find("Activation limit : ") != string::npos) {
                    
                } else if (bufferAnalysis.find("In") != string::npos) { // teste si c'est le titre In
                    InOrOut = true;
                } else if (bufferAnalysis.find("Out") != string::npos) { // teste si c'est le titre Out
                    InOrOut = false;
                } else {    // donc c'est une ligne avec des valeurs
                    beginBracket = bufferAnalysis.find("[");    // utiles pour trouver l'index
                    endBracket = bufferAnalysis.find("]");

                    numberOfIO = stoi(bufferAnalysis.substr(beginBracket+1, endBracket-(beginBracket+1))); // valeur entre crochets [] 
                    if (numberOfIO >= IOconnexions) { 
                        cout << "warning : connexion " << numberOfIO << " of " << this->getNeuronID() << "neuron can't be added because the max IO connexions is " << IOconnexions << endl;
                        
                    } else {
                        
                        int a = bufferAnalysis.find("-");   // position de la première occurence de "-"

                        if (InOrOut) {  // teste si on est dans la catégorie In ou Out
                            In[numberOfIO] = bufferAnalysis.substr(endBracket+4, 8);
                            factorIn[numberOfIO] = stoi(bufferAnalysis.substr(bufferAnalysis.find("-", a+1)+2, bufferAnalysis.length()-(bufferAnalysis.find("-",a+1)+2)));
                            /* après avoir trouvé la première occurence de "-" (position stockée dans a), on refait la recherche à partir de sa position+1 pour trouver l'occurence d'après sans retomber sur la 1ère.
                            * une fois la position du 2ème "-" trouvée, on cherche à avoir la longueur du string qui correspond à la valeur de factor
                            * on rajoute donc +2 depuis la position du dernier "-" pour passer le "-" et l'espace et être à la position du début des chiffres
                            * en faisant longueur totale moins (-) position, on a la longueur du string qui contient la valeur de factor 
                            */
                        } else {
                            Out[numberOfIO] = bufferAnalysis.substr(endBracket+4, 8);
                        }
                        
                    }
                }
            }
        }
        
    }
    
    fNeuron.close();
    
    return true;
}



string Neuron::getNeuroneFileName() {   // retourne le chemin jusqu'au fichier neurone
    
    string NeuronFileName = this->mindPath;
    switch (this->Type) {   // dossier en fonction du type de neurone
        case 0:
            NeuronFileName += "association/";
            break;
        default:
            NeuronFileName += "dedicated/";
            break;
    }
    NeuronFileName += this->ID + ".IQ";
    
    return NeuronFileName;
    
}



bool Neuron::EditNeuroneFile(bool InOrOut, string neuronNameToEdit) {
    
    ifstream fNeuronOriginal(this->getNeuroneFileName());
    ofstream fNewNeuron(this->getNeuroneFileName() + "_copy");
    
    stringstream bufferFile;
    string bufferLineAnalysis;
    bool currentConnexionType = true;
    
    
    if (fNeuronOriginal.is_open() && fNewNeuron.is_open()) {
        
        bufferFile << fNeuronOriginal.rdbuf();
        
        for(int i = 0 ; i < 5 ; i++) {  // passer les premières lignes du fichier, on arrive au - In[500]
            getline(bufferFile, bufferLineAnalysis);
        }
    
        while (!bufferFile.eof()) {    // va passer toutes les lignes du buffer

            getline(bufferFile, bufferLineAnalysis);    // lit la ligne suivante
            
            if (!bufferLineAnalysis.empty()) {  // la ligne ne doit pas être vide pour pouvoir être analysée
                
                if (bufferLineAnalysis.find("In") != string::npos) {  // teste si c'est le titre In
                    currentConnexionType = true;
                } else if (bufferLineAnalysis.find("Out") != string::npos) {  // teste si c'est le titre Out
                    currentConnexionType = false;
                    
                } else {    // donc c'est une ligne avec des valeurs
                    
                    if (currentConnexionType == InOrOutToAdd) {     // chercher dans parmi le bon type de connexions
                    
                        if (bufferLineAnalysis.find("nonenone") != string::npos) {    // une place de libre pour une nouvelle connexion
                            
                            bufferFile.seekp(bufferFile.tellg());   // place la tête d'écriture à la position de lecture dans le bufferFile
                            bufferFile.seekp( -bufferLineAnalysis.length(), ios_base::cur );    // déplace la tête d'écriture une ligne en arrière puisque la tête de lecture est une ligne après
                            bufferFile.seekp( bufferLineAnalysis.find("nonenone")-1, ios_base::cur );   // déplace la tête là où il y a le nom du neurone
                            bufferFile.write(neuronName.c_str(), neuronName.length());  // écrit le nouveau nom
                            break;  // on arrête de parcourir tout le fichier
                            
                        }
                    }
                }
            }
        }
        
    } else {
        cout << "error: in addNewIn method : le fichier neurone" << this->getNeuroneFileName() << "n'est pas ouvert" << endl;
    }
    
    
    fNewNeuron << bufferFile.str();
    
    fNeuronOriginal.close();
    fNewNeuron.close();
    
    filesystem::rename(this->getNeuroneFileName() + "_copy", this->getNeuroneFileName());
    
}



NUMBER Neuron::getActivationLimit() {
    
    return this->activationLimit;
    
}



void Neuron::setActivationLimit(NUMBER newLimit) {
    
    this->activationLimit = newLimit;
    
}









