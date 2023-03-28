#include "../include/fileio.h"

// Lee el archivo de configuración y devuelve una estructura Configuration con los valores correspondientes
void readConfigurationFile(string fileName, ConfigurationCOE& config) {

    ifstream configFile(fileName);

    if (!configFile.is_open()) {
        cerr << "No se pudo abrir el archivo " << fileName << endl;
        return;
    }

    string paramName, _, paramValue;

    // Lee cada parámetro del archivo y lo asigna a la estructura Configuration
    while (configFile >> paramName >> _ >> paramValue) {
        if (paramName == "populationSize") {
            config.populationSize = stoi(paramValue);
        }
        else if (paramName == "iterations") {
            config.iterations = stoi(paramValue);
        }
        else if (paramName == "learningWindow") {
            config.learningWindow = stod(paramValue);
        }
        else if (paramName == "elitism") {
            config.elitism = stod(paramValue);
        }
        else if (paramName == "speciesShrinkRate") {
            config.speciesShrinkRate = stod(paramValue);
        }
        else if (paramName == "seed") {
            config.seed = stoi(paramValue);
        }
        else if (paramName == "verbose") {
            config.verbose = stoi(paramValue);
        }
        else {
            cerr << "Parametro desconocido en el archivo de configuracion: " << paramName << endl;
        }
    }

    configFile.close();
}

// Lee el archivo de instancia y devuelve una estructura Instance con los valores correspondientes
Instance readInstanceFile(string fileName) {
    Instance instance;

    ifstream instanceFile(fileName);

    if (!instanceFile.is_open()) {
        cerr << "No se pudo abrir el archivo " << fileName << endl;
        return instance;
    }

    // Lee los primeros valores del archivo y los asigna a los campos correspondientes en la estructura Instance
    double capacity;
    instanceFile >> instance.numItems >> instance.numGroups >> capacity >> instance.knowBest;

    // Lee el peso de cada artículo y lo agrega a un nuevo objeto Item en el vector items de la instancia
    float weight[MAXITEMS];
    for (int i = 0; i < instance.numItems; i++) {
        Item* newItem = new Item{(unsigned int)i};
        newItem->weight = capacity * instance.weightFactor;

        for (int k = 0; k < instance.numGroups; k++) {
            
            instanceFile >> weight[k];
            weight[k] *= instance.weightFactor;

            // Verificación si los pesos son enteros o flotantes, weightFactor ayuda a evitar errores por pérdida de precisión" o "truncamiento"
            while (weight[k] != floor(weight[k])) {
                weight[k] *= 10;
                newItem->weight *= 10;
                instance.totalWeight *= 10;
                instance.weightFactor *= 10;
                for (int l = k - 1; l >= 0; l--) {
                    weight[l] *= 10;
                }
                for (int j = i - 1; j >= 0; j--) {
                    for (int l = 0; l < instance.numGroups; l++) {
                        instance.items[j]->weights[l] *= 10;
                    }

                }
            }

            newItem->weights[k] = weight[k];
            if (weight[k] < newItem->weight) newItem->weight = weight[k];
        }

        instance.totalWeight += newItem->weight;
        instance.items[i] = newItem;
    }

    instance.capacity = capacity * instance.weightFactor;

    instanceFile.close();

    // Ordenar los elementos por su peso
    sortItemsByWeight(instance);
    instance.minWeight = instance.items[instance.numItems - 1]->weight;

    // Encontramos los ñ objetos grandes
    while (instance.items[instance.n_]->weight > instance.capacity / 2 && instance.n_ < instance.numItems) instance.n_++;

    // Determinar el tipo de problema
    if (instance.numItems > instance.numGroups) instance.problemType = 1;

    return instance;
}

// Función para imprimir la información de un cromosoma en formato JSON.
void printChromosomeAsJson(const Chromosome& chromosome, bool printGroups) {
    cout << "{" << endl;
    cout << "  \"fitness\": {" << endl;
    cout << "    \"value\": " << chromosome.fitness << endl;
    cout << "  }," << endl;
    cout << "  \"group_count\": " << chromosome.totalGroups << "," << endl;
    cout << "  \"time\": " << chromosome.time << "," << endl;
    cout << "  \"generation\": " << chromosome.iteration << "," << endl;
    if (printGroups) {
        cout << "  \"groups\": [" << endl;
        for (int i = 0; i < chromosome.totalGroups; i++) {
            cout << "    {" << endl;
            cout << "      \"id\": " << chromosome.groups[i]->id << "," << endl;
            cout << "      \"volume\": " << chromosome.groups[i]->volume << "," << endl;
            cout << "      \"items\": [";
            for (int j = 0; j < chromosome.groups[i]->totalItems; j++) {
                cout << chromosome.groups[i]->items[j]->id;
                if (j < chromosome.groups[i]->totalItems - 1)cout << ", ";
            }
            cout << "]" << endl;
            cout << "    }" << endl;
            if (i < chromosome.totalGroups - 1)cout << ", ";
        }
        cout << "  ]" << endl;
    }
    cout << "}" << endl;
}
