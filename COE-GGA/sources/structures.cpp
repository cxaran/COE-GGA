#include "../include/structures.h"

// Función para ordenar los airticulos de una instancia.
void sortItemsByWeight(Instance& instance) {
    long int m, k, ban = 1;

    k = instance.numItems - 1;
    while (ban) {
        ban = 0;
        for (m = 0; m < k; m++) {
            if (instance.items[m]->weight < instance.items[m + 1]->weight) {
                Item* aux = instance.items[m];
                instance.items[m] = instance.items[m + 1];
                instance.items[m + 1] = aux;
                ban = 1;
            }
        }
        k--;
    }

    for (int i = 0; i < instance.numItems; i++) {
        instance.items[i]->position = i;
    }
}

//  Función que vacía los items guardados en un grupo
void clearGroup(Group& group) {
    group.volume = 0;
    group.totalItems = 0;
}

// Función obtiene el articulo en la posición indicada segun su peso.
Item* getItem(const Chromosome& chromosome, int itemPos) {
    return chromosome.problem->items[itemPos];
}

// Función para agregar un articulo a un grupo.
void addItemToGroup(Group& group, Item& item) {
    // Aumentar el volumen del grupo y agregar el objeto.
    group.volume += item.weights[group.id];
    group.items[group.totalItems++] = &item;
}

// Función para crear un nuevo grupo y agregar un objeto a él
void createNewGroupWithItem(Chromosome& chromosome, Item& item) {
    Group* group;
    // Si el grupo existe se limpia
    if (chromosome.groups[chromosome.totalGroups]) {
        group = chromosome.groups[chromosome.totalGroups];
        clearGroup(*group);
    }
    // De lo contrario se crea uno nuevo
    else {
        group = new Group();
        chromosome.groups[chromosome.totalGroups] = group;
    }
    // Se agrega el item al grupo
    group->id = chromosome.totalGroups;
    group->volume += item.weights[group->id];
    group->items[group->totalItems++] = &item;
    chromosome.totalGroups++;

    // Actualizar el maxSpan
    if (group->volume > chromosome.maxSpan) {
        chromosome.maxSpan = group->volume;
    }

}

// Función para ordenar aleatoriamente el vector permutation sin tomar los ñ objetos grandes.
void shufflePermutationN_(Instance& instance) {
    // Si permutation esta vacio se inicializa
    if (instance.permutation.size == 0) {
        instance.permutation.reSize(instance.numItems);
        instance.permutation.randomize(&instance.config.seed, instance.n_, instance.numItems);
    }
    else {
        instance.permutation.randomize(&instance.config.seed, instance.n_, instance.numItems);
    }
}

// Elimina un item de un grupo dado su posición en el grupo
void removeItemFromGroup(Group& group, int itemPos) {
   group.totalItems--;
   group.items[itemPos] = group.items[group.totalItems];
}

// Función para comparar dos cromosomas según su fitness.
bool compareFitness(const Chromosome& a, const Chromosome& b) {
    if (a.problem->problemType == 1) {
        return a.maxSpan < b.maxSpan;
    }
    else {
        if (a.totalGroups == b.totalGroups) return a.fitness > b.fitness;
        else return a.totalGroups < b.totalGroups;
    }
}

// Función para verificar dos cromosomas comparten fitness.
bool compareEqualFitness(const Chromosome& a, const Chromosome& b) {
    if (a.problem->problemType == 1) {
        return a.maxSpan == b.maxSpan;
    }
    else {
        return a.fitness == b.fitness;
    }
}

// Copiar del grupo de origen al grupo de destino
void copyGroup(Group* dest, const Group* src) {
    // Copiar los elementos del grupo
    memcpy(dest->items, src->items, sizeof(Item*) * src->totalItems);
    // Copiar atributos simples
    dest->totalItems = src->totalItems;
    dest->volume = src->volume;
    dest->id = src->id;
}

// Copiar los atributos de un chromosoma en otro
void copyChromosome(Chromosome& dest, const Chromosome& src) {
    // Copiar atributos simples
    dest.iteration = src.iteration;
    dest.fitness = src.fitness;
    dest.maxSpan = src.maxSpan;

    // Reutilizar objetos de grupo existentes
    unsigned int i = 0;
    while (i < src.totalGroups) {
        if (dest.groups[i]) {
            copyGroup(dest.groups[i], src.groups[i]);
        }
        else {
            dest.groups[i] = new Group(*src.groups[i]);
        }
        ++i;
    }
    dest.totalGroups = src.totalGroups;
}

// Función que agregua un cromosoma a la población y verifica si es el mejor de la población.
void addChromosomeToPopulation(Population& population, Chromosome& chromosome) {
    // Agregar el cromosoma a la población
    population.chromosomes[population.size] = &chromosome;
    // Actualizar el tamaño de la población
    population.size++;
    // Si es el primer cromosoma agregado, se establece como el mejor
    if (population.size == 1) {
        population.bestChromosome = chromosome;
        for (unsigned int i = 0; i < chromosome.totalGroups; ++i) {
            population.bestChromosome.groups[i] = new Group(*chromosome.groups[i]);
        }
    }
    // Si el cromosoma recién agregado es mejor que el mejor cromosoma de la población, se establece como el mejor
    else {
        if (chromosome.problem->problemType == 1) {
            if (chromosome.maxSpan < population.bestChromosome.maxSpan) {
                copyChromosome(population.bestChromosome, chromosome);
            }
        }
        else {
            if (chromosome.totalGroups < population.bestChromosome.totalGroups) {
                copyChromosome(population.bestChromosome, chromosome);
            }
        }
    }
}

// Función para agrega un cromosoma a la población reemplazando otro existente.
void replaceChromosomeInPopulation(Population& population, Chromosome& chromosome, int index) {
    copyChromosome(population[index], chromosome);
}

// Función para ordenar los cromosomas según su fitness.
void sortChromosomesByFitness(Population& population) {
    long int i,
        k = population.size - 1,
        i2 = 0,
        ban = 1;

    while (ban){
        ban = 0;
        for (i = i2; i < k; i++){
            // Se ordena la población
            if (compareFitness(population[i], population[i + 1])){
                Chromosome* aux = population.chromosomes[i];
                population.chromosomes[i] = population.chromosomes[i + 1];
                population.chromosomes[i + 1] = aux;
                ban = 1;
            }
            // Los chromosomas con fitness repetidos pasan al principio de la población
            else if (compareEqualFitness(population[i], population[i + 1])){
                Chromosome* aux = population.chromosomes[i + 1];
                population.chromosomes[i + 1] = population.chromosomes[i2];
                population.chromosomes[i2] = aux;
                i2++;
            }
        }
        k--;
    }

    // Si el cromosoma recién agregado es mejor que el mejor cromosoma de la población, se establece como el mejor
    if (population.bestChromosome.maxSpan > 0) {
        if (population.bestChromosome.problem->problemType == 1) {
            if (population[population.size - 1].maxSpan < population.bestChromosome.maxSpan) {
                copyChromosome(population.bestChromosome, population[population.size - 1]);
            }
        }
        else {
            if (population[population.size - 1].totalGroups < population.bestChromosome.totalGroups) {
                copyChromosome(population.bestChromosome, population[population.size - 1]);
            }
        }
    }
    else {
        population.bestChromosome = population[population.size - 1];
        for (unsigned int i = 0; i < population[population.size - 1].totalGroups; ++i) {
            population.bestChromosome.groups[i] = new Group(*population[population.size - 1].groups[i]);
        }
    }
}

// Función para ordenar una permutacion segun el volumen de los grupos de una solución.
void sortGroupsByPermutation(const Chromosome& chromosome, Permutation& order, bool ascending) {
    long int m, k, ban = 1;
    k = chromosome.totalGroups - 1;
    if (ascending) {
        while (ban) {
            ban = 0;
            for (m = 0; m < k; m++) {

                if (chromosome.getGroupVolume(order[m]) > chromosome.getGroupVolume(order[m + 1])) {
                    order.swap(m, m + 1);
                    ban = 1;
                }


            }
            k--;
        }
    }
    else {
        while (ban) {
            ban = 0;
            for (m = 0; m < k; m++) {

                if (chromosome.getGroupVolume(order[m]) < chromosome.getGroupVolume(order[m + 1])) {
                    order.swap(m, m + 1);
                    ban = 1;
                }

            }
            k--;
        }
    }
}

// Función para ordenar una permutacion segun el peso de los articulos que corresponden.
void sortPermutationByItems(const Instance& instance, Permutation& order, int size) {
    long int m, k, ban = 1;

    k = size - 1;
    while (ban) {
        ban = 0;
        for (m = 0; m < k; m++)
        {
            if ( instance.items[order[m]]->weight < instance.items[order[m + 1]]->weight) {
                order.swap(m, m + 1);
                ban = 1;
            }
        }
        k--;
    }
}

// Función que recalcula el volumen de un grupo
void calculateGroupVolume(Group* group) {
    group->volume = 0;

    for (int i = 0; i < group->totalItems; ++i) {
        group->volume += group->items[i]->weights[group->id];
    }
}

// Agrega un grupo existente al cromosoma
void addGroupToChromosome(Chromosome& chromosome,const Group& group) {
    if (chromosome.groups[chromosome.totalGroups]) {
        copyGroup(chromosome.groups[chromosome.totalGroups], &group);
    }
    else {
        chromosome.groups[chromosome.totalGroups] = new Group(group);
    }
    chromosome.groups[chromosome.totalGroups]->id = chromosome.totalGroups;
    if (chromosome.problem->problemType == 1 && chromosome.totalGroups != group.id) {
        calculateGroupVolume(chromosome.groups[chromosome.totalGroups]);
    }
    if (chromosome.groups[chromosome.totalGroups]->volume > chromosome.maxSpan) {
        chromosome.maxSpan = chromosome.groups[chromosome.totalGroups]->volume;
    }
    chromosome.totalGroups++;
    if (group.volume < chromosome.maxEmptyVolume) {
        chromosome.maxEmptyVolume = group.volume;
    }
}

// Elimina los grupos vacíos de una solución
void removeEmptyGroups(Chromosome& chromosome) {
    int lastNonEmptyGroupIndex = -1;
    for (int i = 0; i < chromosome.totalGroups; i++) {
        if (chromosome.groups[i]->totalItems > 0) {
            lastNonEmptyGroupIndex++;
            if (i != lastNonEmptyGroupIndex) {
                swap(chromosome.groups[i], chromosome.groups[lastNonEmptyGroupIndex]);
            }
        }
    }
    chromosome.totalGroups = lastNonEmptyGroupIndex + 1;
}

// Limpia el contenido de un cromosoma
void clearChromosome(Chromosome& chromosome) {
    chromosome.totalGroups = 0;
    chromosome.fitness = 0;
    chromosome.maxSpan = 0;
}

// Función para calcular el fitness de una especie
void calculateSpeciesFitness(Species& specie) {
    double fitnessSum = 0.0; // Inicializar la suma de fitness
    int populationSize = specie.population.size; // Obtener el tamaño de la población

    // Iterar sobre cada cromosoma en la población
    if (specie.population[0].problem->problemType == 0) {
        for (int i = 0; i < populationSize; i++) {
            Chromosome* chromosome = specie.population.chromosomes[i];
            fitnessSum += chromosome->fitness; // Sumar el fitness del cromosoma
        }
    }
    else {
        for (int i = 0; i < populationSize; i++) {
            Chromosome* chromosome = specie.population.chromosomes[i];
            fitnessSum += chromosome->maxSpan; // Sumar el maxSpan del cromosoma
        }
    }

    // Calcular el promedio de fitness o maxSpan según el problemType
    specie.fitness = fitnessSum / populationSize; // Promedio de fitness
    
}


// Función que determina si todos los elementos están incluidos en un cromosoma.
bool allItemsIncluded(const Chromosome& chromosome) {
    /*/set<int> itemIds;
    int totalGroups = 0;
    for (int g = 0; g < chromosome.totalGroups; g++){
        if(chromosome.groups[g]->volume == 0) continue;
        long int volume = 0;
        totalGroups++;
        for (int i = 0; i < chromosome.groups[g]->totalItems; i++) {
            if (itemIds.count(chromosome.groups[g]->items[i]->id) > 0) {
                return false;
            }
            itemIds.insert(chromosome.groups[g]->items[i]->id);
            volume += chromosome.groups[g]->items[i]->weight;
        }
        if (volume > chromosome.problem->capacity) return false;
        if (volume != chromosome.groups[g]->volume) return false;
    }
    if (chromosome.totalGroups != totalGroups) return false;
    return itemIds.size() == chromosome.problem->numItems;
    */return true;
}

// Función que comprueba si el número de grupos en un cromosoma es igual al lower bound.
bool checkLowerBound(const Chromosome& chromosome) {
    if (chromosome.totalGroups <= chromosome.problem->lowerBound) {
        return true;
    }
    return false;
}