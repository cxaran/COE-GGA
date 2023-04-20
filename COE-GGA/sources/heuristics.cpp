#include "../include/heuristics.h"

// Funcion que calcula el limite L2
void lowerBound(Instance& instance) {
    long int k, m, i, j, aux1, aux2;
    long double sjx = 0, sj2 = 0, sj3 = 0;
    long int jx = 0, cj12, jp = 0, jpp = 0, cj2;

    jx = instance.n_;
    
    if (jx == instance.numItems) {
        instance.lowerBound = jx;
        return;
    }
    if (jx == 0){
        if (fmod(instance.totalWeight, instance.capacity) >= 1)
            instance.lowerBound = (long int)ceil(instance.totalWeight / instance.capacity);
        else
            instance.lowerBound = (long int)(instance.totalWeight / instance.capacity);
        return;
    }
    else{
        cj12 = jx;
        for (i = jx; i < instance.numItems; i++)
            sjx += instance.items[i]->weight;
        jp = jx;
        for (i = 0; i < jx; i++){
            if (instance.items[i]->weight <= instance.capacity - instance.items[jx]->weight){
                jp = i;
                break;
            }
        }

        cj2 = jx - jp;
        for (i = jp; i <= jx - 1; i++)
            sj2 += instance.items[i]->weight;
        jpp = jx;
        sj3 = instance.items[jpp]->weight;

        instance.items[instance.numItems] = new Item{ instance.numItems ,0 };
        
        while (instance.items[jpp + 1]->weight == instance.items[jpp]->weight){
            jpp++;
            sj3 += instance.items[jpp]->weight;
        }
        instance.lowerBound = cj12;

        do{
            if (fmod((sj3 + sj2), instance.capacity) >= 1)
                aux1 = (long int)ceil((sj3 + sj2) / instance.capacity - cj2);
            else
                aux1 = (long int)((sj3 + sj2) / instance.capacity - cj2);

            if (instance.lowerBound < (cj12 + aux1))
                instance.lowerBound = cj12 + aux1;
            jpp++;
            if (jpp < instance.numItems){
                sj3 += instance.items[jpp]->weight;
                while (instance.items[jpp + 1]->weight == instance.items[jpp]->weight){
                    jpp++;
                    sj3 += instance.items[jpp]->weight;
                }
                while (jp > 0 && instance.items[jp - 1]->weight <= instance.capacity - instance.items[jpp]->weight){
                    jp--;
                    cj2++;
                    sj2 += instance.items[jp]->weight;
                }

            }
            if (fmod((sjx + sj2), instance.capacity) >= 1)
                aux2 = (long int)ceil((sjx + sj2) / instance.capacity - cj2);
            else
                aux2 = (long int)((sjx + sj2) / instance.capacity - cj2);
        } while (jpp <= instance.capacity || (cj12 + aux2) > instance.lowerBound);
    }

}

// Funci�n que realiza el First Fit para agregar un art�culo al cromosoma.
void firstFit(Chromosome& chromosome, int itemPos, int& startGroup, bool isLast) {
    long int i;
    Item* item = getItem(chromosome, itemPos);

    // Si el art�culo actual supera la capacidad del grupo m�s vac�o, se crea un nuevo grupo.
    if (!isLast && item->weight > (chromosome.problem->capacity - chromosome.maxEmptyVolume)) {
        i = chromosome.totalGroups;
    }
    else {
        // En caso contrario, se itera por los grupos existentes buscando un grupo con capacidad suficiente para el art�culo.
        for (i = startGroup; i < chromosome.totalGroups; i++) {
            if (chromosome.getGroupVolume(i) + item->weight <= chromosome.problem->capacity) {
                // Si se encuentra un grupo, se agrega el art�culo y se actualizan los atributos del cromosoma.
                addItemToGroup(*chromosome.groups[i], *item);
                if (chromosome.getGroupVolume(i) > chromosome.maxSpan) {
                    chromosome.maxSpan = chromosome.getGroupVolume(i);
                }
                if (isLast){
                    for (i; i < chromosome.totalGroups; i++) {
                        chromosome.fitness += pow(((double)chromosome.getGroupVolume(i) / chromosome.problem->capacity), 2);
                    }
                    return;
                }
                // Se actualiza el ultimo contenedor con capacidad disponible para objetos.
                if (chromosome.getGroupVolume(i) + chromosome.problem->minWeight > chromosome.problem->capacity && i == startGroup){
                    startGroup++;
                    chromosome.fitness += pow(((double)chromosome.getGroupVolume(i) / chromosome.problem->capacity), 2);
                }
                return;
            }
            if (isLast) {
                chromosome.fitness += pow(((double)chromosome.getGroupVolume(i) / chromosome.problem->capacity), 2);
            }
        }
    }
    // Si no se encuentra un grupo con suficiente capacidad, se crea un nuevo grupo y se actualizan los atributos del cromosoma.
    createNewGroupWithItem(chromosome, *item);
    if (chromosome.getGroupVolume(i) < chromosome.maxEmptyVolume) {
        chromosome.maxEmptyVolume = chromosome.getGroupVolume(i);
    }
    if (isLast) {
        chromosome.fitness += pow(((double)chromosome.getGroupVolume(i) / chromosome.problem->capacity), 2);
    }
}

//  Funci�n que realiza el "firstFit" sin los � objetos grandes.
void firstFitN_(Chromosome& chromosome) {
    int i, j, startGroup = 0;
    // Se incializa el volumen del grupo m�s vac�o.
    chromosome.maxEmptyVolume = chromosome.problem->capacity;
    // Si el cromosoma contiene objetos grandes, se agregan en grupos separados.
    if (chromosome.problem->n_ > 0){
        for (i = 0; i < chromosome.problem->n_; i++) {
            createNewGroupWithItem(chromosome,*getItem(chromosome, i));
            if (chromosome.getGroupVolume(i) < chromosome.maxEmptyVolume) {
                chromosome.maxEmptyVolume = chromosome.getGroupVolume(i);
            }
        }
        // Se ordena aleatoriamente el vector permutation sin tomar los n objetos grandes.
        shufflePermutationN_(*chromosome.problem);
        // Se aplica el algoritmo First Fit a los elementos restantes del vector permutation.
        for (j = chromosome.problem->n_; j < chromosome.problem->numItems - 1; j++) {
            firstFit(chromosome, chromosome.problem->permutation[j], startGroup, false);
        }
        firstFit(chromosome, chromosome.problem->permutation[j], startGroup, true);
    }
    else{
        // Si el cromosoma no contiene objetos grandes, se ordena aleatoriamente el vector permutation completo.
        shufflePermutationN_(*chromosome.problem);
        // Se aplica el algoritmo First Fit a los elementos del vector permutation.
        for (j = 0; j < chromosome.problem->numItems - 1; j++) {
            firstFit(chromosome, chromosome.problem->permutation[j], startGroup, false);
        }
        firstFit(chromosome, chromosome.problem->permutation[j], startGroup, true);
    }
}

// Funci�n para inicializar la poblaci�n, regresa la poblacion ordenada
void initializePopulation(Instance& instance, Population& population, int populationSize, bool applyFirstFit) {
    for (int i = 0; i < populationSize; i++) {
        // Creamos el cromosoma
        Chromosome* chromosome = new Chromosome();
        chromosome->problem = &instance;

        // Si se aplica firstFit
        if (applyFirstFit) {
            // Llama a la funci�n FF-�
            firstFitN_(*chromosome);
            // Calcular el fitness del cromosoma
            chromosome->fitness /= chromosome->totalGroups;
            chromosome->iteration = 0;
        }

        addChromosomeToPopulation(population, *chromosome);
    }
}

