#include "../heuristics.h"

//"firstFit" es una función que asigna elementos a grupos en un cromosoma.
//La función recibe un cromosoma y un vector de elementos.Cada elemento se 
//agrega al primer grupo en el que su volumen y el volumen del grupo sea 
//menor o igual a la capacidad máxima permitida.Si ningún grupo cumple con 
//esta condición, se crea un nuevo grupo con el elemento.
void firstFit(Chromosome& chromosome, const vector<Item*>& items) {
    //Itera sobre cada elemento en "items"
    for (Item* item : items) {
        //Marca si el elemento ha sido añadido a un grupo
        bool added = false;
        //Itera sobre cada grupo en "chromosome.groups"
        for (int i = 0; i < chromosome.groups.size(); ++i) {
            // Verificamos el id del grupo
            if (chromosome.groups[i].id != i) {
                chromosome.groups[i].id = i;
                refactorGroupVolume(chromosome.groups[i]);
            }
            //Si el volumen del grupo más el peso del elemento es menor o igual a la capacidad del problema
            if (chromosome.groups[i].volume + item->weights[chromosome.groups[i].id] <= chromosome.problem->capacity) {
                //Marca si el elemento ha sido añadido a un grupo
                added = addItemToGroup(chromosome.groups[i], *item);
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
    // Comprueba el orden de los contenedores
    if (!is_sorted(chromosome.groups.begin(), chromosome.groups.end(), compareVolumeGroups))
        sort(chromosome.groups.begin(), chromosome.groups.end(), compareVolumeGroups);
}

//"bestFit" es una función que también asigna elementos a grupos en un 
//cromosoma.Sin embargo, en lugar de agregar el elemento al primer 
//grupo que cumpla con la condición de capacidad, busca el grupo con 
//la mejor capacidad restante y agrega el elemento allí.Si ningún 
//grupo tiene capacidad suficiente, se crea un nuevo grupo.
void bestFit(Chromosome& chromosome, const vector<Item*>& items) {
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
    // Comprueba el orden de los contenedores
    if (!is_sorted(chromosome.groups.begin(), chromosome.groups.end(), compareVolumeGroups))
        sort(chromosome.groups.begin(), chromosome.groups.end(), compareVolumeGroups);
}

// Función que realiza el "firstFit" con reacomodo
void firstFitR(Chromosome& chromosome,const vector<Item*>& itemsConst) {
    vector<Item*> items;
    items.insert(items.end(), itemsConst.begin(), itemsConst.end());
    //Itera sobre cada elemento en "items"
    for (int t = 0; t < items.size(); ++t) {
        //Marca si el elemento ha sido añadido a un grupo
        bool added = false;
        //Itera sobre cada grupo en "chromosome.groups"
        for (int i = 0; i < chromosome.groups.size(); ++i) {
            // Verificamos el id del grupo
            if (chromosome.groups[i].id != i) {
                chromosome.groups[i].id = i;
                refactorGroupVolume(chromosome.groups[i]);
            }
            //Si el volumen del grupo más el peso del elemento es menor o igual a la capacidad del problema
            if (chromosome.groups[i].volume + items[t]->weights[chromosome.groups[i].id] <= chromosome.problem->capacity) {
                //Marca si el elemento ha sido añadido a un grupo
                added = addItemToGroup(chromosome.groups[i], *items[t]);
                //Detiene la iteración sobre los grupos
                break;
            }
            //Si el elemento puede ser intercambiado por un elemento del grupo
            for (int j = 0; j < chromosome.groups[i].items.size() && !added; ++j) {
                double newVolume = chromosome.groups[i].volume
                    + items[t]->weights[chromosome.groups[i].id]
                    - chromosome.groups[i].items[j]->weights[chromosome.groups[i].id];
                if (newVolume <= chromosome.problem->capacity && newVolume > chromosome.groups[i].volume) {
                    Item* aux = items[t];
                    items[t] = chromosome.groups[i].items[j];
                    chromosome.groups[i].items[j] = aux;
                    refactorGroupVolume(chromosome.groups[i]);
                    break;
                }
            }
        }
        //Si el elemento no ha sido añadido a ningún grupo
        if (!added) {
            //Crea un nuevo grupo con el elemento
            createNewGroupWithItem(chromosome, *items[t]);
        }
    }
    // Comprueba el orden de los contenedores
    if (!is_sorted(chromosome.groups.begin(), chromosome.groups.end(), compareVolumeGroups))
        sort(chromosome.groups.begin(), chromosome.groups.end(), compareVolumeGroups);
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
    firstFitR(*child, unassignedItems);
    calculateFitness(*child);
    unassignedItems.clear();
    return child;
}

// Función de muta controlada
void controlledMutation(Chromosome& chromosome, int k) {
    // Seleccione un grupo aleatorio para ser eliminado.
    if (chromosome.groups.size() > 0) {
        int deletionRate = ceil(sqrt(chromosome.groups.size()));
        vector<Item*> removedItems;
        // Eliminar los grupos del cromosoma.
        for (int i = chromosome.groups.size() - 1; i > chromosome.groups.size() - deletionRate; i--) {
            removedItems.insert(removedItems.end(), chromosome.groups[i].items.begin(), chromosome.groups[i].items.end());
            chromosome.groups.erase(chromosome.groups.begin() + i);
        }
        // Reacomodo de los elementos eliminados.
        firstFit(chromosome, removedItems);
    }
}

// Función que resuelve el problema mediante una estrategia genética
void geneticAlgorithm(Specie& specie, int GENERATION, float ELITE_SIZE, float CROSSOVER_RATE, float MUTATION_RATE) {
    
    // Ordenamiento de las soluciones
    if (!is_sorted(specie.members.begin(), specie.members.end(), compareFitness))
        sort(specie.members.begin(), specie.members.end(), compareFitness);

    // CRUZA
    // Seleccionamos los mejores individuos como padres élite para la cruza
    vector<int> parents_e;
    while (parents_e.size() < specie.members.size() * CROSSOVER_RATE)
        parents_e.push_back(controlledSelection(specie.members.size(), ELITE_SIZE, true));
    // Seleccionamos los peores individuos como padres promedio para la cruza
    vector<int> parents_p;
    while (parents_p.size() < parents_e.size())
        parents_p.push_back(controlledSelection(specie.members.size(), 1 - ELITE_SIZE, false));
    // Realizamos la Cruza
    vector<Chromosome*> children;
    for (int i = 0; i < parents_p.size(); i++) {
        children.push_back(controlledCrossover(specie.members[parents_e[i]], specie.members[parents_p[i]]));
        children.back()->age = GENERATION;
    }
    //Inserción por remplazo controlado
    for (int parent : parents_p) {// Replazamos a los padres promedio
        if (specie.members[parent].age < GENERATION && !children.empty()) {
            specie.members.erase(specie.members.begin() + parent);
            specie.members.push_back(*children.back());
            delete children.back();
            children.pop_back();
        }
    }
    for (int i = specie.members.size() - 1; !children.empty() && i > specie.members.size() * ELITE_SIZE; i--) {
        if (specie.members[i].age < GENERATION) {// Remplazamos a los elemtos de generaciones pasadas
            specie.members.erase(specie.members.begin() + i);
            i--;
            specie.members.push_back(*children.back());
            delete children.back();
            children.pop_back();
        }
    }
    while (!children.empty()) {
        delete children.back();
        children.pop_back();
    }

    // Ordenamiento de las soluciones
    if (!is_sorted(specie.members.begin(), specie.members.end(), compareFitness))
        sort(specie.members.begin(), specie.members.end(), compareFitness);

    // MUTA
    // Seleccionamos individuos a mutar
    vector<int> individualsToMutate;
    while (individualsToMutate.size() < specie.members.size() * MUTATION_RATE)
        individualsToMutate.push_back(controlledSelection(specie.members.size(), 1 - ELITE_SIZE, false));
    for (int i = 0; i < individualsToMutate.size(); i++) {
        controlledMutation(specie.members[individualsToMutate[i]], 1.3);
        specie.members[individualsToMutate[i]].age = GENERATION;
    }
}
