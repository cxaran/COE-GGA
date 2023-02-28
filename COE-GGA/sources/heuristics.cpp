#include "../include/heuristics.h"

//"firstFit" es una funci�n que asigna elementos a grupos en un cromosoma.
//La funci�n recibe un cromosoma y un vector de elementos.Cada elemento se 
//agrega al primer grupo en el que su volumen y el volumen del grupo sea 
//menor o igual a la capacidad m�xima permitida.Si ning�n grupo cumple con 
//esta condici�n, se crea un nuevo grupo con el elemento.
void firstFit(Chromosome& chromosome,const vector<Item*>& items) {
    // Comprueba el orden de los contenedores
    if (!is_sorted(chromosome.groups.begin(), chromosome.groups.end(), compareVolumeGroups))
        sort(chromosome.groups.begin(), chromosome.groups.end(), compareVolumeGroups);
    //Itera sobre cada elemento en "items"
    for (Item* item : items) {
        //Marca si el elemento ha sido a�adido a un grupo
        bool added = false;
        //Itera sobre cada grupo en "chromosome.groups"
        for (int i = 0; i < chromosome.groups.size(); ++i) {
            // Verificamos el id del grupo
            if (chromosome.groups[i].id != i) {
                chromosome.groups[i].id = i;
                refactorGroupVolume(chromosome.groups[i]);
            }
            //Si el volumen del grupo m�s el peso del elemento es menor o igual a la capacidad del problema
            if (chromosome.groups[i].volume + item->weights[chromosome.groups[i].id] <= chromosome.problem->capacity) {
                //Marca si el elemento ha sido a�adido a un grupo
                added = addItemToGroup(chromosome.groups[i], *item);
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

void firstFitM(Chromosome& chromosome, float capacity, const vector<Item*>& items) {
    // Comprueba el orden de los contenedores
    if (!is_sorted(chromosome.groups.begin(), chromosome.groups.end(), compareVolumeGroups))
        sort(chromosome.groups.begin(), chromosome.groups.end(), compareVolumeGroups);
    //Itera sobre cada elemento en "items"
    for (Item* item : items) {
        //Marca si el elemento ha sido a�adido a un grupo
        bool added = false;
        //Itera sobre cada grupo en "chromosome.groups"
        for (int i = 0; i < chromosome.groups.size(); ++i) {
            // Verificamos el id del grupo
            if (chromosome.groups[i].id != i) { 
                chromosome.groups[i].id = i;
                refactorGroupVolume(chromosome.groups[i]); 
            }
            //Si el volumen del grupo m�s el peso del elemento es menor o igual a la capacidad del problema
            if (chromosome.groups[i].volume + item->weights[chromosome.groups[i].id] <= capacity) {
                //Marca si el elemento ha sido a�adido a un grupo
                added = addItemToGroup(chromosome.groups[i], *item);
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
void bestFit(Chromosome& chromosome,const vector<Item*>& items) {
    // Comprueba el orden de los contenedores
    if (!is_sorted(chromosome.groups.begin(), chromosome.groups.end(), compareVolumeGroups))
        sort(chromosome.groups.begin(), chromosome.groups.end(), compareVolumeGroups);
    //Itera sobre cada elemento en "items"
    for (Item* item : items) {
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
    // Comprueba el orden de los contenedores
    if (!is_sorted(chromosome.groups.begin(), chromosome.groups.end(), compareVolumeGroups))
        sort(chromosome.groups.begin(), chromosome.groups.end(), compareVolumeGroups);
    //Itera en "items" y verifica si el peso del elemento es mayor a la mitad de la capacidad
    for (Item* item : items) {
        if (item->min > chromosome.problem->capacity / 2) {
            //Crea un nuevo grupo con el elemento
            if (!createNewGroupWithItem(chromosome, *item)) exit(3);
            continue;
        }
    }
    //Itera sobre cada elemento en "items"
    for (Item* item : items) {
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


// Funci�n de selecci�n controlada
int controlledSelection(int chromosomesSize, float size, bool start) {
    int numIndividuos = chromosomesSize * size;
    if (numIndividuos < 1) return start ? 0 : chromosomesSize - 1;
    int startIndex = start ? 0 : chromosomesSize - numIndividuos;
    int randomIndex = rand() % numIndividuos + startIndex;
    return randomIndex;
}

// Funci�n de cruza controlada
Chromosome* controlledCrossover(const Chromosome& a, const Chromosome& b) {
    Chromosome* child = new Chromosome();
    child->problem = a.problem;
    // Remove groups with repeated items
    vector<int> items(child->problem->numItems, 0);
    for (int i = 0; i < a.problem->numGroups; i++) {
        if (i < a.groups.size()) {
            bool repeatedA = false;
            for (int j = 0; j < a.groups[i].items.size(); j++) {
                items[a.groups[i].items[j]->id]++;
                if (items[a.groups[i].items[j]->id] > 1) {
                    for (j; j >= 0; j--) items[a.groups[i].items[j]->id]--;
                    repeatedA = true;
                    break;
                }
            }
            if (!repeatedA) child->groups.push_back(a.groups[i]);
        }
        if (i < b.groups.size()) {
            bool repeatedB = false;
            for (int j = 0; j < b.groups[i].items.size(); j++) {
                items[b.groups[i].items[j]->id]++;
                if (items[b.groups[i].items[j]->id] > 1) {
                    for (j; j >= 0; j--) items[b.groups[i].items[j]->id]--;
                    repeatedB = true;
                    break;
                }
            }
            if (!repeatedB) child->groups.push_back(b.groups[i]);
        }
    }
    // Create new groups with missing items
    vector<Item*> unassignedItems;
    for (Item* item : a.problem->items)
        if (items[item->id] == 0) unassignedItems.push_back(item);
    firstFit(*child, unassignedItems);
    return child;
}

// Funci�n que realiza el reacomodo por pares
void pairWiseReassignment(Chromosome& chromosome, vector<Item*>& items) {
    // Recorremos cada grupo
    for (auto& group : chromosome.groups) {
        // Recorremos cada elemento en items
        for (size_t i = 0; i < items.size(); ++i) {
            for (size_t j = i + 1; j < group.items.size(); ++j) {
                // Calcular el nuevo volumen si se intercambia items[i] y group.items[j]
                double newVolume = group.volume - group.items[j]->weights[group.items[j]->id] + items[i]->weights[group.id];
                //if (newVolume <= chromosome.problem->capacity && newVolume < group.volume) {
                if (newVolume <= chromosome.problem->capacity && group.volume < newVolume) {
                    // Actualizar el volumen del grupo
                    group.volume = newVolume;
                    // Intercambiar items[i] y group.items[j]
                    Item* temp = items[i];
                    items[i] = group.items[j];
                    group.items[j] = temp;
                }
            }
        }
    }
}

// Funci�n de muta controlada
void controlledMutation(Chromosome& chromosome) {
    // Seleccione un grupo aleatorio para ser eliminado.
    if (chromosome.groups.size() > 0) {
        int deletionRate = rand() % chromosome.groups.size()/3;
        vector<Item*> removedItems;
        // Eliminar los grupos del cromosoma.
        for (int i = chromosome.groups.size() - 1; i > chromosome.groups.size() - deletionRate; i--) {
            removedItems.insert(removedItems.end(), chromosome.groups[i].items.begin(),chromosome.groups[i].items.end());
            chromosome.groups.erase(chromosome.groups.begin() + i);
        }
        // Ejecutar el algoritmo de reacomodo por pares.
        //vector<Item*> freeItems;
        //for (auto& group : chromosome.groups) {
        //    freeItems.insert(freeItems.end(), group.items.begin(), group.items.end());
        //}
        //chromosome.groups.clear();
        //firstFit(chromosome, freeItems);
        pairWiseReassignment(chromosome, removedItems);
        // Agregar el grupo eliminado al final del cromosoma.
        firstFit(chromosome, removedItems);
    }
}

// Funci�n que aplica el algoritmo First Fit con capacidad modificada
bool reorderByCapacity(Chromosome& chromosome) {
    float resize[] = {0.408, 0.333, 0.342, 0.498, 1.008, 0.294};
    vector<Item*> freeItems;
    Chromosome best = chromosome;
    Chromosome aux = chromosome;
    for (float s : resize) {
        freeItems.clear();
        for (auto& group : aux.groups) {
            freeItems.insert(freeItems.end(), group.items.begin(), group.items.end());
            group.items.clear();
        }
        aux.groups.clear();
        reverse(freeItems.begin(), freeItems.end());
        firstFitM(aux, aux.problem->capacity * s, freeItems);
        freeItems.clear();
        for (auto& group : aux.groups) {
            freeItems.insert(freeItems.end(), group.items.begin(), group.items.end());
            group.items.clear();
        }
        aux.groups.clear();
        firstFitM(aux, aux.problem->capacity, freeItems);
        if (aux.groups.size() <= best.groups.size()) best = aux;
    }
    if (best.groups.size() < chromosome.fitness.bins) {
        if (allItemsIncluded(best)) {
            calculateFitness(best);
            chromosome = best;
            return true;
        }
        return false;
    }
    return false;
}

// Funci�n que resuelve el problema mediante una estrategia gen�tica
void geneticAlgorithm(Specie& specie, int GENERATION, float ELITE_SIZE, float CROSSOVER_RATE, float MUTATION_RATE) {
    // CRUZA
    // Seleccionamos los mejores individuos como padres �lite para la cruza
    vector<int> parents_e;
    for (int i = 0; i < specie.members.size() * CROSSOVER_RATE; i++)
        parents_e.push_back(controlledSelection(specie.members.size(), ELITE_SIZE, true));
    // Seleccionamos los peores individuos como padres promedio para la cruza
    vector<int> parents_p;
    for (int i = 0; i < parents_e.size(); i++)
        parents_p.push_back(controlledSelection(specie.members.size(), 1 - ELITE_SIZE, false));
    // Realizamos la Cruza
    vector<Chromosome*> children;
    for (int i = 0; i < parents_p.size(); i++) {
        children.push_back(controlledCrossover(specie.members[parents_e[i]], specie.members[parents_p[i]]));
        children.back()->age = GENERATION;
    }
    //Inserci�n por remplazo controlado
    for (int i = specie.members.size() -1; !children.empty() && i > specie.members.size() * ELITE_SIZE; i--){
        if (specie.members[i].age < GENERATION) {
            specie.members[i] = *children.back();
            delete children.back();
            children.pop_back();
        }
    }
    
    // MUTA
    // Seleccionamos individuos a mutar
    set<int> individualsToMutate;
    while (individualsToMutate.size() < specie.members.size() * CROSSOVER_RATE) {
        individualsToMutate.insert(controlledSelection(specie.members.size(), 1 - ELITE_SIZE, false));
    }
    for (int indx : individualsToMutate) {
        Chromosome* individual = &specie.members[indx];
        //controlledMutation(*individual);
        individual->age = GENERATION;
        calculateFitness(*individual);
    }

    // BUSQUEDA LOCAL
    // Seleccionamos los mejores individuos para la busqueda local
    sort(specie.members.begin(), specie.members.end(), compareFitness);
    for (int i = 0; i < specie.members.size() * ELITE_SIZE; i++) {
        if (specie.members[i].age == GENERATION) 
            if(reorderByCapacity(specie.members[i]))
                specie.members[i].age = GENERATION;
    }
}