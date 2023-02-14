#include "../heuristics.h"

//"firstFit" es una función que asigna elementos a grupos en un cromosoma.
//La función recibe un cromosoma y un vector de elementos.Cada elemento se 
//agrega al primer grupo en el que su volumen y el volumen del grupo sea 
//menor o igual a la capacidad máxima permitida.Si ningún grupo cumple con 
//esta condición, se crea un nuevo grupo con el elemento.
void firstFit(Chromosome& chromosome,const vector<Item*>& items) {
    //Itera sobre cada elemento en "items"
    for (const auto& item : items) {
        //Marca si el elemento ha sido añadido a un grupo
        bool added = false;
        //Itera sobre cada grupo en "chromosome.groups"
        for (auto& group : chromosome.groups) {
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

//"bestFit" es una función que también asigna elementos a grupos en un 
//cromosoma.Sin embargo, en lugar de agregar el elemento al primer 
//grupo que cumpla con la condición de capacidad, busca el grupo con 
//la mejor capacidad restante y agrega el elemento allí.Si ningún 
//grupo tiene capacidad suficiente, se crea un nuevo grupo.
void bestFit(Chromosome& chromosome,const vector<Item*>& items) {
    //Itera sobre cada elemento en "items"
    for (const auto& item : items) {
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
Chromosome& controlledSelection(vector<Chromosome>& chromosomes, float size, bool start) {
    int numIndividuos = chromosomes.size() * size;
    if (numIndividuos < 1) return start ? chromosomes[0] : chromosomes[chromosomes.size() - 1];
    int startIndex = start ? 0 :  chromosomes.size() - numIndividuos;
    int randomIndex = rand() % numIndividuos + startIndex;
    return chromosomes[randomIndex];
}


vector<Item*> getCommonItems(const vector<Item*>& a, const vector<Item*>& b) {
    set<int> setA, setB;
    for (Item* item : a) {
        setA.insert(item->id);
    }
    for (Item* item : b) {
        setB.insert(item->id);
    }

    set<int> intersection;
    set_intersection(setA.begin(), setA.end(), setB.begin(), setB.end(), inserter(intersection, intersection.begin()));

    vector<Item*> commonItems;
    for (Item* item : a) {
        if (intersection.count(item->id) > 0) {
            commonItems.push_back(item);
        }
    }
    return commonItems;
}


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
        if (!hasRepeated) {
            child->groups.push_back(group);
        }
    }

    // Create new groups with missing items
    vector<Item*> unassignedItems;
    for (Item* item : a.problem->items) {
        if (items[item->id] == 0) {
            unassignedItems.push_back(item);
        }
        else {
            items[item->id] = 0;
        }
    }
    
    firstFit(*child, unassignedItems);

    //cout << child->groups.size() << endl;

    return child;
}


// Función que resuelve el problema mediante una estrategia genética
void geneticAlgorithm(Specie& specie, float ELITE_SIZE, float CROSSOVER_RATE, float MUTATION_RATE) {
    
    // Seleccionamos los mejores individuos como padres élite
    vector<Chromosome*> parents_e;
    for (int i = 0; i < specie.members.size() * CROSSOVER_RATE; i++) {
        // Utilizamos la función de selección controlada para seleccionar los padres élite
        parents_e.push_back(&controlledSelection(specie.members, ELITE_SIZE, true));
    }

    // Seleccionamos los peores individuos como padres promedio
    vector<Chromosome*> parents_p;
    for (int i = 0; i < parents_e.size(); i++) {
        // Utilizamos la función de selección controlada para seleccionar los padres promedio
        parents_p.push_back(&controlledSelection(specie.members, 1 - ELITE_SIZE, false));
    }
   
    // Cruza
    vector<Chromosome*> childs;
    for (int i = 0; i < parents_p.size(); i++) {
        childs.push_back(controlledCrossover(*parents_e[i], *parents_p[i]));
    }

    // Insersion
    for (Chromosome* child : childs) {
        if (allItemsIncluded(*child)) {
            calculateFitness(*child);
            specie.members.push_back(*child);
        }
    }

}