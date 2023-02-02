#include "structures.h"

// Función para comparar dos articulos según su mínimo.
bool compareMin(const Item& a, const Item& b) {
    return a.min < b.min;
}

// Función para comparar dos cromosomas según su fitness.
bool compareFitness(const Chromosome& a, const Chromosome& b) {
    return a.fitness.value > b.fitness.value;
}

// Función para imprimir la información de un cromosoma.
void printChromosome(const Chromosome& chromosome, bool printGroups) {
    std::cout << "Informacion de los grupos:\n";
    std::cout << "fitness: " << chromosome.fitness.value << "\n";
    std::cout << "Tamano de los grupos: " << chromosome.groups.size() << "\n";
    if (printGroups)
        for (const auto& group : chromosome.groups) {
            std::cout << "ID de grupo: " << group.id << " ";
            std::cout << "Volumen: " << group.volume << " ";
            std::cout << "Articulos: ";
            for (const auto& item : group.items) {
                std::cout << item.id << " ";
            }
            std::cout << "\n";
        }
    std::cout << "*************************\n";
}

// Función para agregar un objeto a un grupo.
bool addItemToGroup(Group& group, const Item& item) {
    // Verificar si el objeto ya está en el grupo.
    for (const auto& i : group.items) if (i.id == item.id) return false;
    // Aumentar el volumen del grupo y agregar el objeto.
    group.volume += item.weights[group.id];
    group.items.push_back(item);
    return true;
}

// Función para crear un nuevo grupo y agregar un objeto a él.
bool createNewGroupWithItem(Chromosome& chromosome, const Item& item) {
    // Verificar si todavía se pueden crear más grupos.
    if (chromosome.groups.size() < chromosome.problem.numGroups) {
        Group group;
        group.id = chromosome.groups.size();
        // Agregar el objeto al grupo.
        addItemToGroup(group, item);
        chromosome.groups.push_back(group);
        return true;
    }
    // Salir con un código de error.
    else {
        exit(3);
        return false;
    }
}

//"firstFit" es una función que asigna elementos a grupos en un cromosoma.
//La función recibe un cromosoma y un vector de elementos.Cada elemento se 
//agrega al primer grupo en el que su volumen y el volumen del grupo sea 
//menor o igual a la capacidad máxima permitida.Si ningún grupo cumple con 
//esta condición, se crea un nuevo grupo con el elemento.
void firstFit(Chromosome& chromosome, const std::vector<Item>& items) {
    //Itera sobre cada elemento en "items"
    for (const auto& item : items) {
        //Marca si el elemento ha sido añadido a un grupo
        bool added = false;
        //Itera sobre cada grupo en "chromosome.groups"
        for (auto& group : chromosome.groups) {
            //Si el volumen del grupo más el peso del elemento es menor o igual a la capacidad del problema
            if (group.volume + item.weights[group.id] <= chromosome.problem.capacity) {
                //Añade el elemento al grupo
                addItemToGroup(group, item);
                //Marca que el elemento ha sido añadido a un grupo
                added = true;
                //Detiene la iteración sobre los grupos
                break;
            }
        }
        //Si el elemento no ha sido añadido a ningún grupo
        if (!added) {
            //Crea un nuevo grupo con el elemento
            createNewGroupWithItem(chromosome, item);
        }
    }
}

//"bestFit" es una función que también asigna elementos a grupos en un 
//cromosoma.Sin embargo, en lugar de agregar el elemento al primer 
//grupo que cumpla con la condición de capacidad, busca el grupo con 
//la mejor capacidad restante y agrega el elemento allí.Si ningún 
//grupo tiene capacidad suficiente, se crea un nuevo grupo.
void bestFit(Chromosome& chromosome, const std::vector<Item>& items) {
    //Itera sobre cada elemento en "items"
    for (const auto& item : items) {
        //Inicializa el ID del grupo con el mejor ajuste como -1 (no se ha encontrado ningún grupo)
        int bestGroupId = -1;
        //Inicializa el mejor ajuste como 0
        int bestFit = 0;
        //Itera sobre cada grupo en "chromosome.groups"
        for (int i = 0; i < chromosome.groups.size(); ++i) {
            //Calcula el espacio libre en el grupo actual
            int freeSpace = chromosome.problem.capacity - chromosome.groups[i].volume;
            //Si el peso del elemento es menor o igual al espacio libre y el espacio libre es mejor que el mejor ajuste actual
            if (item.weights[chromosome.groups[i].id] <= freeSpace && freeSpace > bestFit) {
                //Actualiza el mejor ajuste
                bestFit = freeSpace;
                //Actualiza el ID del grupo con el mejor ajuste
                bestGroupId = i;
            }
        }
        //Si se encontró un grupo con un mejor ajuste
        if (bestGroupId != -1) {
            //Añade el elemento al grupo con el mejor ajuste
            if (!addItemToGroup(chromosome.groups[bestGroupId], item)) exit(3);
        }
        //Si no se encontró un grupo con un mejor ajuste
        else {
            //Crea un nuevo grupo con el elemento
            if (!createNewGroupWithItem(chromosome, item)) exit(3
            );
        }
    }
}

// Función para calcular la aptitud de un cromosoma
void calculateFitness(Chromosome& chromosome) {
    Fitness fitness;
    for (const auto& group : chromosome.groups) {
        ++fitness.bins; // Incrementamos el número de grupos
        fitness.maxSpan = std::max(fitness.maxSpan, group.volume); // Actualizamos el tamaño máximo de los grupos
        fitness.value += pow( group.volume / double(chromosome.problem.capacity) , 2);
    }
    fitness.value = fitness.value / fitness.bins; // Calculamos el valor de la aptitud
    chromosome.fitness = fitness;
}

// Función para calcular la aptitud de un cromosoma
//Proporción de espacio vacío : aquí, el fitness sería la 
//proporción de espacio vacío en cada contenedor.El 
//objetivo sería tener la mayor proporción posible, por 
//lo que el fitness más alto sería 1.
void calculateFitness3( Chromosome& chromosome) {
    Fitness fitness;
    int totalVolume = 0;
    int maxGroupVolume = 0;
    for (const auto& group : chromosome.groups) {
        totalVolume += group.volume;
        maxGroupVolume = std::max(maxGroupVolume, group.volume);
    }
    fitness.bins = chromosome.groups.size();
    fitness.value = (totalVolume/ fitness.bins) / double(chromosome.problem.capacity);
    fitness.maxSpan = maxGroupVolume;
    chromosome.fitness = fitness;
}

//Función que determina si todos los elementos están incluidos en un cromosoma.
bool allItemsIncluded(const Chromosome& chromosome) {
    for (const auto& item : chromosome.problem.items) {
        bool itemFound = false;
        for (const auto& group : chromosome.groups) {
            for (const auto& groupItem : group.items) {
                if (groupItem.id == item.id) {
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

