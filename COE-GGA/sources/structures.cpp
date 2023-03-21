#include "../include/structures.h"

// Funci�n para comparar dos articulos seg�n su m�nimo.
bool compareMin(const Item* a, const Item* b) {
    return a->min > b->min;
}

// Funci�n para comparar dos cromosomas seg�n su fitness.
bool compareFitness(const Chromosome& a, const Chromosome& b) {
    return a.fitness.value > b.fitness.value;
}

// Funci�n para comparar dos grupos segun su volumen.
bool compareVolumeGroups(const Group& a, const Group& b) {
    return a.volume > b.volume;
}

// Funci�n para imprimir la informaci�n de un cromosoma.
void printChromosome(const Chromosome& chromosome, bool printGroups) {
    cout << "Informacion de la solucion:\n";
    cout << "fitness: " << chromosome.fitness.value << "\n";
    cout << "maxSpan: " << chromosome.fitness.maxSpan << "\n";
    cout << "Tamano de los grupos: " << chromosome.groups.size() << "\n";
    if (printGroups)
        for (const auto& group : chromosome.groups) {
            cout << "ID de grupo: " << group.id << " ";
            cout << "Volumen: " << group.volume << "\n";
            cout << "Articulos: ";
            for (const auto item : group.items) {
                cout << item->id << " ";
            }
            cout << "\n";
        }
}

void printChromosomeAsJson(const Chromosome& chromosome, bool printGroups) {
    cout << "{" << endl;
    cout << "  \"fitness\": {" << endl;
    cout << "    \"value\": " << chromosome.fitness.value << "," << endl;
    cout << "    \"maxSpan\": " << chromosome.fitness.maxSpan << endl;
    cout << "  }," << endl;
    cout << "  \"group_count\": " << chromosome.groups.size() << "," << endl;
    if (printGroups) {
        cout << "  \"groups\": [" << endl;
        for (int i = 0; i < chromosome.groups.size(); i++) {
            cout << "    {" << endl;
            cout << "      \"id\": " << chromosome.groups[i].id << "," << endl;
            cout << "      \"volume\": " << chromosome.groups[i].volume << "," << endl;
            cout << "      \"items\": [";
            for (int j = 0; j < chromosome.groups[i].items.size(); j++) {
                cout << chromosome.groups[i].items[j]->id;
                if(j < chromosome.groups[i].items.size() - 1)cout << ", ";
            }
            cout << "]" << endl;
            cout << "    }" << endl;
            if(i < chromosome.groups.size() - 1)cout << ", ";
        }
        cout << "  ]" << endl;
    }
    cout << "}" << endl;
}

// Funci�n para agregar un objeto a un grupo.
bool addItemToGroup(Group& group,Item& item) {
   // cout << "probar" << item.weights[group.id] << "\n";
    // Verificar si el objeto ya est� en el grupo.
    for (const auto& i : group.items) if (i->id == item.id) return false;
    // Aumentar el volumen del grupo y agregar el objeto.
    group.volume += item.weights[group.id];
    group.items.push_back(&item);
    return true;
}
bool addItemGrouptoGroup(Group& group, Item& item, Group& group2) {
   //cout << "probar" << item.weights[group2.id] << "\n";
    // Verificar si el objeto ya est� en el grupo.
    for (const auto& i : group.items) if (i->id == item.id) return false;
    // Aumentar el volumen del grupo y agregar el objeto.
    group.volume += item.weights[group2.id];
    group.items.push_back(&item);
    return true;
}

// Funci�n para crear un nuevo grupo y agregar un objeto a �l.
bool createNewGroupWithItem(Chromosome& chromosome, Item& item) {
    // Verificar si todav�a se pueden crear m�s grupos.
    if (chromosome.groups.size() < chromosome.problem->numGroups) {
        Group group;
        group.id = chromosome.groups.size();
        // Agregar el objeto al grupo.
        addItemToGroup(group, item);
        chromosome.groups.push_back(group);
        //cout << chromosome.groups.size() << endl;
        return true;
    }
    // Salir con un c�digo de error.
    else {
        cout << chromosome.groups.size() << endl;
        exit(3);
        return false;
    }
}

// Funci�n para calcular la aptitud de un cromosoma
void calculateFitness(Chromosome& chromosome) {
    Fitness fitness;
    for (const auto& group : chromosome.groups) {
        ++fitness.bins; // Incrementamos el n�mero de grupos
        fitness.maxSpan = max(fitness.maxSpan, group.volume); // Actualizamos el tama�o m�ximo de los grupos
        fitness.value += pow( group.volume / double(chromosome.problem->capacity) , 2);
    }
    fitness.value = fitness.value / fitness.bins; // Calculamos el valor de la aptitud
    chromosome.fitness = fitness;
}

// Funci�n para calcular la aptitud de un cromosoma
//Proporci�n de espacio vac�o : aqu�, el fitness ser�a la 
//proporci�n de espacio vac�o en cada contenedor.El 
//objetivo ser�a tener la mayor proporci�n posible, por 
//lo que el fitness m�s alto ser�a 1.
void calculateFitness3( Chromosome& chromosome) {
    Fitness fitness;
    int totalVolume = 0;
    int maxGroupVolume = 0;
    for (const auto& group : chromosome.groups) {
        totalVolume += group.volume;
        maxGroupVolume = max(double(maxGroupVolume), group.volume);
    }
    fitness.bins = chromosome.groups.size();
    fitness.value = (totalVolume/ fitness.bins) / double(chromosome.problem->capacity);
    fitness.maxSpan = maxGroupVolume;
    chromosome.fitness = fitness;
}

//Funci�n que determina si todos los elementos est�n incluidos en un cromosoma.
bool allItemsIncluded(const Chromosome& chromosome) {
    for (const Item* item : chromosome.problem->items) {
        bool itemFound = false;
        for (const Group& group : chromosome.groups) {
            for (const Item* groupItem : group.items) {
                if (groupItem->id == item->id) {
                    itemFound = true;
                    break;
                }
            }
            if (itemFound) break;
        }
        if (!itemFound) return false;
    }
    return true;
}


void refactorGroupVolume(Group& group) {
    group.volume = 0;
    for (const Item* item : group.items) {
        group.volume += item->weights[group.id];
    }
}
