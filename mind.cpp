#include "mind.h"


// PUBLIC

Mind::Mind(string mindPath) {

    srand(time(NULL));              // pour string Mind::getRandomName() ;
    
    // Définit les attributs
    this->setMindPath(mindPath);
    
    // méthodes
    this->createNeuronsDirectories();
    this->loadNeuronsFromFiles();
    
    cout << "***** Mind setup finished *****" << endl;
    
}



Mind::~Mind() {
    
    
    
}



string Mind::createNeuron(NeuronType typeOfNeuron) {    // retourne l'ID du neurone ou "0"

    string pathAssociation, pathDedicated;
    pathAssociation = pathDedicated = this->mindPath;
    pathAssociation += "association";
    pathDedicated += "dedicated";

    if ((numberOfFilesInDirectory(pathAssociation) + numberOfFilesInDirectory(pathDedicated)) < numeric_limits<NUMBER>::max()) {    // vérifie qu'il n'y ait pas trop de neurones déjà crés (et chargés) par rapport à la valeur max possible du type NUMBER
        Neuron newNeuron(this->getRandomName(), this->mindPath, typeOfNeuron);
        return newNeuron.getNeuronID();
    } else {
        cout << "error: in mind.cpp > createNeuron() : no more neuron space available" << endl;
        return "0";
    }

}



void Mind::organisation(string inputName) {
    
    
}



string Mind::loadNeuronsFromFiles() {   // charge le nom de chaque neurone dans neuronsName[]
    
    NUMBER i = 0;   // compteur de neurone chargés
    
    string pathAssociation, pathDedicated;
    pathAssociation = pathDedicated = this->mindPath;
    pathAssociation += "association";
    pathDedicated += "dedicated";
    
    if((numberOfFilesInDirectory(pathAssociation) + numberOfFilesInDirectory(pathDedicated)) > numeric_limits<NUMBER>::max()) {     // vérifie qu'il n'y ait pas trop de neurones déjà présents dans les dossiers
        cout << "Too much neurons in " << this->mindPath << " folder : " << (numberOfFilesInDirectory(pathAssociation) + numberOfFilesInDirectory(pathDedicated)) << " for " << numeric_limits<NUMBER>::max() << " allowed." << endl;
        exit(1);
    } else if ((numberOfFilesInDirectory(pathAssociation) + numberOfFilesInDirectory(pathDedicated)) == 0) {
        cout << "There is no neurons in " << this->mindPath << endl;
    }
    
    string fNeuronName;
    
    // Les deux for() passent les directory_iterator 1 par 1, extrait juste le nom du neurone et l'enregistre
    for(auto& fNeuron: filesystem::directory_iterator(pathAssociation)) {   // pour le dossier association
        if (i < numeric_limits<NUMBER>::max()) {
            fNeuronName = static_cast<string>(fNeuron.path());  // cast de char[] à string
            cout << fNeuronName << " added" << endl;
            this->neuronsName[i] = fNeuronName.substr(fNeuronName.find(".IQ")-8, 8);    // enlève le format de fichier + les dossiers dans le chemin : ./Neurons/association/nonenone.IQ
            i++;
        } else {
            cout << "Too much neurons in " << pathAssociation << " folder : " << i << " for " << numeric_limits<NUMBER>::max() << " allowed." << endl;
            exit(1);
        }
    }

    
    
    for(auto& fNeuron: filesystem::directory_iterator(pathDedicated)) {     // pour le dossier dedicated
        if (i < numeric_limits<NUMBER>::max()) {
            fNeuronName = static_cast<string>(fNeuron.path());
            cout << fNeuronName << " added" << endl;
            this->neuronsName[i] = fNeuronName.substr(fNeuronName.find(".IQ")-8, 8);            
            i++;
        } else {
            cout << "Too much neurons in " << pathDedicated << " folder : " << i << " for " << numeric_limits<NUMBER>::max() << " allowed." << endl;
            exit(1);
        }
    }
    
    if (i == 0) {   // s'il n'y a aucun neurone dans les fichiers
        this->generateMinimalNeurons();
    }
    
    return neuronsName[0];
}



string Mind::generateMinimalNeurons() {     // s'il n'y a aucun neurone dans mind, alors on en créé quelques uns
    
    NUMBER i;
    
    for (i = 0 ; !(this->neuronsName[i].empty()) ; i++);    // comptabilise les neurones répertoriés suite à l'initialisation
    
    if (i < 100) {
        
        cout << "Generating the 100 minimal neurons..." << endl; 
        
        for ( ; i < 100 ; i++) {
            this->createNeuron(association);
        }
    }
    
    cout << "Creating 2 dedicated neurons , one for console reading and the other for console writing..." << endl;
    this->createNeuron(dedicated_input);
    this->createNeuron(dedicated_output);
    
    return neuronsName[0];
}



string Mind::getMindPath() {
    
    return this->mindPath;
    
}



// PRIVATE

void Mind::createNeuronsDirectories() {     // nécessaires à la mémoire, où seront stockés les neurones
    
    string associationPath = this->mindPath, dedicatedPath = this->mindPath;
    associationPath += "association";
    dedicatedPath += "dedicated";
    
    if (!filesystem::exists(this->mindPath) || !filesystem::exists(associationPath) || !filesystem::exists(dedicatedPath)) {
        cout << "Creation of directories in " << this->mindPath << endl;
        filesystem::create_directory(this->mindPath);
        filesystem::create_directory(associationPath);
        filesystem::create_directory(dedicatedPath);
    }
}



int Mind::numberOfFilesInDirectory(string path) {       // retourne el nombre de fichiers dans un répertoire donné
    // https://stackoverflow.com/questions/41304891/how-to-count-the-number-of-files-in-a-directory-using-standard/41305019
    
    using filesystem::directory_iterator;
    return distance(directory_iterator(path), directory_iterator{});
    
}



string Mind::getRandomName() {      // donne un nom au nouveau neurone créé
    
    string name = "aaaaaaaa";   // initialisation pour avoir 8 caractères dans le nom
    
    bool found;
    
    do {
        found = false;
        
        for(int i = 0 ; i < 8 ; i++) {  // pour chaque lettre du nom
            name[i] = static_cast<char>((rand()%26)+97);    // donne une lettre ASCII en minuscule (de 97 à 122) et édite cette lettre dans le nom
        }
        
        for (NUMBER i = 0 ; i < numeric_limits<NUMBER>::max() ; i++) {     // passe le nom des neurones déjà existants et vérifie que ce nom n'existe pas déjà
            if (neuronsName[i] == name) {
                found = true;
            }
        }
    } while (found);    // réalisé tant qu'un nouveau nom n'a pas été trouvé
    
    return name;
}



string Mind::setMindPath(string mindPath) {     // enregistre le répertoire d'écriture du mind
    
    if (mindPath[mindPath.length()-1] != '/') {     // vérifie que le nom de répertoire donné fini par un /
        mindPath += '/';    // sinon il ajoute un / pour pouvoir en faire un dossier
    }
    
    this->mindPath = mindPath;
    
    return this->mindPath;
    
}







