#include "../structures.h"

// Funci�n para comparar dos articulos seg�n su m�nimo.
bool compareMin(const Item* a, const Item* b) {
    return a->min < b->min;
}

// Funci�n para comparar dos cromosomas seg�n su fitness.
bool compareFitness(const Chromosome& a, const Chromosome& b) {
    return a.fitness.value > b.fitness.value;
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
    // Verificar si el objeto ya est� en el grupo.
    for (const auto& i : group.items) if (i->id == item.id) return false;
    // Aumentar el volumen del grupo y agregar el objeto.
    group.volume += item.weights[group.id];
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
        return true;
    }
    // Salir con un c�digo de error.
    else {
        exit(3);
        return false;
    }
}

//"firstFit" es una funci�n que asigna elementos a grupos en un cromosoma.
//La funci�n recibe un cromosoma y un vector de elementos.Cada elemento se 
//agrega al primer grupo en el que su volumen y el volumen del grupo sea 
//menor o igual a la capacidad m�xima permitida.Si ning�n grupo cumple con 
//esta condici�n, se crea un nuevo grupo con el elemento.
void firstFit(Chromosome& chromosome,const vector<Item*>& items) {
    //Itera sobre cada elemento en "items"
    for (const auto& item : items) {
        //Marca si el elemento ha sido a�adido a un grupo
        bool added = false;
        //Itera sobre cada grupo en "chromosome.groups"
        for (auto& group : chromosome.groups) {
            //Si el volumen del grupo m�s el peso del elemento es menor o igual a la capacidad del problema
            if (group.volume + item->weights[group.id] <= chromosome.problem->capacity) {
                //A�ade el elemento al grupo
                addItemToGroup(group, *item);
                //Marca que el elemento ha sido a�adido a un grupo
                added = true;
                //Detiene la iteraci�n sobre los grupos
                break;
            }
        }
        //Si el elemento no ha sido a�adido a ning�n grupo
        if (!added) {
            //Crea un nuevo grupo con el elemento
            createNewGroupWithItem(chromosome, *item);
        }
    }
}

//"bestFit" es una funci�n que tambi�n asigna elementos a grupos en un 
//cromosoma.Sin embargo, en lugar de agregar el elemento al primer 
//grupo que cumpla con la condici�n de capacidad, busca el grupo con 
//la mejor capacidad restante y agrega el elemento all�.Si ning�n 
//grupo tiene capacidad suficiente, se crea un nuevo grupo.
void bestFit(Chromosome& chromosome, vector<Item*>& items) {
    //Itera sobre cada elemento en "items"
    for (const auto& item : items) {
        //Inicializa el ID del grupo con el mejor ajuste como -1 (no se ha encontrado ning�n grupo)
        int bestGroupId = -1;
        //Inicializa el mejor ajuste como 0
        int bestFit = 0;
        //Itera sobre cada grupo en "chromosome.groups"
        for (int i = 0; i < chromosome.groups.size(); ++i) {
            //Calcula el espacio libre en el grupo actual
            int freeSpace = chromosome.problem->capacity - chromosome.groups[i].volume;
            //Si el peso del elemento es menor o igual al espacio libre y el espacio libre es mejor que el mejor ajuste actual
            if (item->weights[chromosome.groups[i].id] <= freeSpace && freeSpace > bestFit) {
                //Actualiza el mejor ajuste
                bestFit = freeSpace;
                //Actualiza el ID del grupo con el mejor ajuste
                bestGroupId = i;
            }
        }
        //Si se encontr� un grupo con un mejor ajuste
        if (bestGroupId != -1) {
            //A�ade el elemento al grupo con el mejor ajuste
            if (!addItemToGroup(chromosome.groups[bestGroupId], *item)) exit(3);
        }
        //Si no se encontr� un grupo con un mejor ajuste
        else {
            //Crea un nuevo grupo con el elemento
            if (!createNewGroupWithItem(chromosome, *item)) exit(3);
        }
    }
}

// Funcion best fit que separa los N elementos grandes en grupos iniciales
void bestFitN(Chromosome& chromosome, vector<Item*>& items) {
    //Itera en "items" y verifica si el peso del elemento es mayor a la mitad de la capacidad
    for (const auto& item : items) {
        if (item->min > chromosome.problem->capacity / 2) {
            //Crea un nuevo grupo con el elemento
            if (!createNewGroupWithItem(chromosome, *item)) exit(3);
            continue;
        }
    }
    //Itera sobre cada elemento en "items"
    for (const auto& item : items) {
        if (item->min <= chromosome.problem->capacity / 2) {
            //Inicializa el ID del grupo con el mejor ajuste como -1 (no se ha encontrado ning�n grupo)
            int bestGroupId = -1;
            //Inicializa el mejor ajuste como 0
            int bestFit = 0;
            //Itera sobre cada grupo en "chromosome.groups"
            for (int i = 0; i < chromosome.groups.size(); ++i) {
                //Calcula el espacio libre en el grupo actual
                int freeSpace = chromosome.problem->capacity - chromosome.groups[i].volume;
                //Si el peso del elemento es menor o igual al espacio libre y el espacio libre es mejor que el mejor ajuste actual
                if (item->weights[chromosome.groups[i].id] <= freeSpace && freeSpace > bestFit) {
                    //Actualiza el mejor ajuste
                    bestFit = freeSpace;
                    //Actualiza el ID del grupo con el mejor ajuste
                    bestGroupId = i;
                }
            }
            //Si se encontr� un grupo con un mejor ajuste
            if (bestGroupId != -1) {
                //A�ade el elemento al grupo con el mejor ajuste
                if (!addItemToGroup(chromosome.groups[bestGroupId], *item)) exit(3);
            }
            //Si no se encontr� un grupo con un mejor ajuste
            else {
                //Crea un nuevo grupo con el elemento
                if (!createNewGroupWithItem(chromosome, *item)) exit(3);
            }
        }
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
    for (const auto& item : chromosome.problem->items) {
        bool itemFound = false;
        for (const auto& group : chromosome.groups) {
            for (const auto& groupItem : group.items) {
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

