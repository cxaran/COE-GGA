#include "../heuristics.h"

//"firstFit" es una función que asigna elementos a grupos en un cromosoma.
//La función recibe un cromosoma y un vector de elementos.Cada elemento se 
//agrega al primer grupo en el que su volumen y el volumen del grupo sea 
//menor o igual a la capacidad máxima permitida.Si ningún grupo cumple con 
//esta condición, se crea un nuevo grupo con el elemento.
void firstFit(Chromosome& chromosome,const vector<Item*>& items) {
    //Itera sobre cada elemento en "items"
    for (Item* item : items) {
        //Marca si el elemento ha sido añadido a un grupo
        bool added = false;
        //Itera sobre cada grupo en "chromosome.groups"
        for (Group& group : chromosome.groups) {
            //Si el volumen del grupo más el peso del elemento es menor o igual a la capacidad del problema
            if (group.volume + item->weights[group.id] <= chromosome.problem->capacity) {
                //Marca si el elemento ha sido añadido a un grupo
                added = addItemToGroup(group, *item);
                //Detiene la iteración sobre los grupos
                break;
            }
        }
        //Si el elemento no ha sido añadido a ningún grupo
        if (!added) {
            //Crea un nuevo grupo con el elemento
            createNewGroupWithItem(chromosome, *item);
        }
    }
}

void firstFitM(Chromosome& chromosome, float capacity, const vector<Item*>& items) {
    //Itera sobre cada elemento en "items"
    for (Item* item : items) {
        //Marca si el elemento ha sido añadido a un grupo
        bool added = false;
        //Itera sobre cada grupo en "chromosome.groups"
        for (Group& group : chromosome.groups) {
            //Si el volumen del grupo más el peso del elemento es menor o igual a la capacidad del problema
            if (group.volume + item->weights[group.id] <= capacity) {
                //Marca si el elemento ha sido añadido a un grupo
                added = addItemToGroup(group, *item);
                //Detiene la iteración sobre los grupos
                break;
            }
        }
        //Si el elemento no ha sido añadido a ningún grupo
        if (!added) {
            //Crea un nuevo grupo con el elemento
            createNewGroupWithItem(chromosome, *item);
        }
    }
}

//"bestFit" es una función que también asigna elementos a grupos en un 
//cromosoma.Sin embargo, en lugar de agregar el elemento al primer 
//grupo que cumpla con la condición de capacidad, busca el grupo con 
//la mejor capacidad restante y agrega el elemento allí.Si ningún 
//grupo tiene capacidad suficiente, se crea un nuevo grupo.
void bestFit(Chromosome& chromosome,const vector<Item*>& items) {
    //Itera sobre cada elemento en "items"
    for (Item* item : items) {
        //Inicializa el ID del grupo con el mejor ajuste como -1 (no se ha encontrado ningún grupo)
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
        //Si se encontró un grupo con un mejor ajuste
        if (bestGroupId != -1) {
            //Añade el elemento al grupo con el mejor ajuste
            if (!addItemToGroup(chromosome.groups[bestGroupId], *item)) exit(3);
        }
        //Si no se encontró un grupo con un mejor ajuste
        else {
            //Crea un nuevo grupo con el elemento
            if (!createNewGroupWithItem(chromosome, *item)) exit(3);
        }
    }
}

// Funcion best fit que separa los N elementos grandes en grupos iniciales
void bestFitN(Chromosome& chromosome, vector<Item*>& items) {
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
            //Inicializa el ID del grupo con el mejor ajuste como -1 (no se ha encontrado ningún grupo)
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
            //Si se encontró un grupo con un mejor ajuste
            if (bestGroupId != -1) {
                //Añade el elemento al grupo con el mejor ajuste
                if (!addItemToGroup(chromosome.groups[bestGroupId], *item)) exit(3);
            }
            //Si no se encontró un grupo con un mejor ajuste
            else {
                //Crea un nuevo grupo con el elemento
                if (!createNewGroupWithItem(chromosome, *item)) exit(3);
            }
        }
    }
}


// Función de selección controlada
int controlledSelection(int chromosomesSize, float size, bool start) {
    int numIndividuos = chromosomesSize * size;
    if (numIndividuos < 1) return start ? 0 : chromosomesSize - 1;
    int startIndex = start ? 0 : chromosomesSize - numIndividuos;
    int randomIndex = rand() % numIndividuos + startIndex;
    return randomIndex;
}

// Función de cruza controlada
Chromosome* controlledCrossover(const Chromosome& a, const Chromosome& b) {
    Chromosome* child = new Chromosome();
    child->problem = a.problem;
    vector<Group> groups;
    groups.insert(groups.end(),a.groups.begin(), a.groups.end());
    groups.insert(groups.end(),b.groups.begin(), b.groups.end());
    // Order the groups by volume
    sort(groups.begin(), groups.end(), compareVolumeGroups);
    // Remove groups with repeated items
    vector<int> items(child->problem->numItems, 0);
    for (const Group group : child->groups) {
        bool hasRepeated = false;
        for (Item* item : group.items) {
            items[item->id]++;
            if (items[item->id] > 1) {
                hasRepeated = true;
                break;
            }
        }    
        if (!hasRepeated) child->groups.push_back(group);
    }
    // Create new groups with missing items
    vector<Item*> unassignedItems;
    for (Item* item : a.problem->items) {
        if (items[item->id] == 0) unassignedItems.push_back(item);
        else items[item->id] = 0;
    }
    firstFit(*child, unassignedItems);
    //bestFit(*child, unassignedItems);
    return child;
}

// Función que realiza el reacomodo por pares
void pairWiseReassignment(Chromosome& chromosome, vector<Item*>& items) {
    // Recorremos cada grupo
    for (auto& group : chromosome.groups) {
        // Recorremos cada elemento en items
        for (size_t i = 0; i < items.size(); ++i) {
            for (size_t j = i + 1; j < group.items.size(); ++j) {
                // Calcular el nuevo volumen si se intercambia items[i] y group.items[j]
                double newVolume = group.volume - group.items[j]->weights[group.items[j]->id] + items[i]->weights[group.id];
                //if (newVolume <= chromosome.problem->capacity && newVolume < group.volume) {
                if (newVolume <= chromosome.problem->capacity) {
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

// Función de muta controlada
void controlledMutation(Chromosome& chromosome) {
    // Seleccione un grupo aleatorio para ser eliminado.
    if (chromosome.groups.size() > 0) {
        int deletionRate = rand() % chromosome.groups.size()/2;
        vector<Item*> removedItems;
        // Order the groups by volume
        sort(chromosome.groups.begin(), chromosome.groups.end(), compareVolumeGroups);
        // Eliminar los grupos del cromosoma.
        for (int i = chromosome.groups.size() - 1; i > chromosome.groups.size() - deletionRate; i--) {
            removedItems.insert(removedItems.end(), chromosome.groups[i].items.begin(),chromosome.groups[i].items.end());
            chromosome.groups.erase(chromosome.groups.begin() + i);
        }
        // Ejecutar el algoritmo de reacomodo por pares.
        vector<Item*> freeItems;
        for (auto& group : chromosome.groups) {
            freeItems.insert(freeItems.end(), group.items.begin(), group.items.end());
        }
        chromosome.groups.clear();
        firstFit(chromosome, freeItems);
        //pairWiseReassignment(chromosome, removedItems);
        // Agregar el grupo eliminado al final del cromosoma.
        firstFit(chromosome, removedItems);
    }
}

// Función que aplica el algoritmo First Fit con capacidad modificada
void reorderByCapacity(Chromosome& chromosome) {
    float resize[] = {0.408, 0.333, 0.342, 0.498, 1.008, 0.294};
    vector<Item*> freeItems;
   // Chromosome best = chromosome;
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
        //firstFit(chromosome, chromosome.problem->items);
        //if (aux.groups.size() <= best.groups.size()) best = aux;
        if (aux.groups.size() <= chromosome.fitness.bins) {
            calculateFitness(aux);
            chromosome = aux;
        }
    }
    //if (allItemsIncluded(best)) { 
    //    calculateFitness(best);
    //    chromosome = best;
    //}
    //else return;
}

// Función que resuelve el problema mediante una estrategia genética
void geneticAlgorithm(Specie& specie, int GENERATION, float ELITE_SIZE, float CROSSOVER_RATE, float MUTATION_RATE) {
    // CRUZA
    // Seleccionamos los mejores individuos como padres élite para la cruza
    vector<int> parents_e;
    for (int i = 0; i < specie.members.size() * CROSSOVER_RATE; i++)
        parents_e.push_back(controlledSelection(specie.members.size(), ELITE_SIZE, true));
    // Seleccionamos los peores individuos como padres promedio para la cruza
    vector<int> parents_p;
    for (int i = 0; i < parents_e.size(); i++)
        parents_p.push_back(controlledSelection(specie.members.size(), 1 - ELITE_SIZE, false));
    // Realizamos la Cruza
    vector<Chromosome*> children;
    for (int i = 0; i < parents_p.size(); i++)
        children.push_back(controlledCrossover(specie.members[parents_e[i]], specie.members[parents_p[i]]));
    //Inserción por remplazo
    for (int parent : parents_p) {
        specie.members[parent] = *children.back();
        delete children.back();
        children.pop_back();
    }
    // MUTA
    // Seleccionamos individuos a mutar
    set<int> individualsToMutate;
    while (individualsToMutate.size() < specie.members.size() * CROSSOVER_RATE)
        individualsToMutate.insert(controlledSelection(specie.members.size(), 1 - ELITE_SIZE, false));
    for (int indx : individualsToMutate) {
        Chromosome* individual = &specie.members[indx];
        //double fitness = individual->fitness.value;
        //reorderByCapacity(*individual);
        //if (fitness == individual->fitness.value) {
            controlledMutation(*individual);
            calculateFitness(*individual);
            //if (allItemsIncluded(*individual)) calculateFitness(*individual);
            //else {
            //    exit(3);
            //    return;
            //}
        //}
    }

    // BUSQUEDA LOCAL
    // Seleccionamos los mejores individuos para la busqueda local
    for (int i = 0; i < specie.members.size() * ELITE_SIZE; i++) {
        //reorderByCapacity(specie.members[i]);
    }
}