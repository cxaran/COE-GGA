#include "../../include/GGA/mutation.h"

// Función para realizar reacomodo por pares en un cromosoma dado un conjunto de objetos libres.
void pairwiseRearrangement(Chromosome& chromosome, Permutation& freeItems, long int totalFreeItems) {

    long int i, j, totalFree = 0;

    // indices de los items a intercambiar
    int iA, iB, jA, jB;
    int iAp, iBp, jAp, jBp;
    long int iAw, iBw, jAw, jBw;

    bool ban;

    long int higherWeight, lighterWeight;
    higherWeight = getItem(chromosome, freeItems[0])->weight;
    lighterWeight = higherWeight;

    int startGroup = chromosome.totalGroups;

    // Establece la aptitud y el numero de grupos en su maxima capacidad en 0
    chromosome.fitness = 0;

    // Establece el valor del contenedor mas vacio como la capacidad maxima
    chromosome.maxEmptyVolume = chromosome.problem->capacity;

    Permutation orderGroups(chromosome.totalGroups);
    orderGroups.randomize(&chromosome.problem->config.seed, 0, chromosome.totalGroups);

    freeItems.randomize(&chromosome.problem->config.seed, 0, totalFreeItems);

    unsigned long int sum = 0;

    // Recorremos los gurpos en la solución parcial
    for (i = 0; i < chromosome.totalGroups; i++) {
        Group* group = chromosome.groups[orderGroups[i]];
        sum = (long int)chromosome.getGroupVolume(orderGroups[i]);

        // Para cada par de items iA en el grupo i
        for (iA = 0; iA < chromosome.groups[orderGroups[i]]->totalItems; iA++) {
            // Si el item iA ha sido intercambiado.
            ban = false;
            // Se guarda la posocion ordenada y el peso del item iA
            iAp = group->items[iA]->position;
            iAw = group->items[iA]->weights[group->id];

            // Se recorren los items siguientes a iA para generar cada par (iA,iB) del grupo i
            for (iB = iA + 1; iB < chromosome.groups[orderGroups[i]]->totalItems; iB++) {
                // Se guarda la posocion ordenada y el peso del item iB
                iBp = group->items[iB]->position;
                iBw = group->items[iB]->weights[group->id];

                // Para cada par de items jA de los items libres
                for (jA = 0; jA < totalFreeItems - 1; jA++) {
                    // Se guarda la posocion ordenada y el peso del item jA
                    jAp = freeItems[jA];
                    jAw = getItem(chromosome, jAp)->weights[group->id];

                    // Si el reocrrido esta en el ultimo grupo
                    if (i == chromosome.totalGroups - 1) {
                        // Se actualiza el peso del item libre de mayor peso
                        if (jAw > higherWeight) {
                            higherWeight = jAw;
                        }
                    }

                    // Se recorren los items siguientes a jA para generar cada par (jA,jB) de los items libres
                    for (jB = jA + 1; jB < totalFreeItems; jB++) {
                        // Se guarda la posocion ordenada y el peso del item jA
                        jBp = freeItems[jB];
                        jBw = getItem(chromosome, jBp)->weights[group->id];

                        // Intercambio de jA por el par de items (iA,iB)
                        if (jAw >= iAw + iBw && sum - (iAw + iBw) + jAw <= chromosome.problem->capacity) {
                            sum = sum - (iAw + iBw) + jAw;
                            freeItems[jA] = iAp;
                            freeItems[totalFreeItems + totalFree] = iBp;
                            chromosome.groups[orderGroups[i]]->items[iA] = getItem(chromosome, jAp);
                            removeItemFromGroup(*chromosome.groups[orderGroups[i]], iB);
                            totalFree++;
                            ban = true;
                            break;
                        }

                        // Intercambio de jB por el par de items (iA,iB)
                        if (jBw >= iAw + iBw && sum - (iAw + iBw) + jBw <= chromosome.problem->capacity) {
                            sum = sum - (iAw + iBw) + jBw;
                            freeItems[jB] = iAp;
                            freeItems[totalFreeItems + totalFree] = iBp;
                            chromosome.groups[orderGroups[i]]->items[iA] = getItem(chromosome, jBp);
                            removeItemFromGroup(*chromosome.groups[orderGroups[i]], iB);
                            totalFree++;
                            ban = true;
                            break;
                        }

                        // Intercambio de (jA,jB) por el par de items (iA,iB)
                        if (jAw + jBw > iAw + iBw || ((jAw + jBw == iAw + iBw) && !(jAw == iAw || jAw == iBw))) {
                            if (sum - (iAw + iBw) + (jAw + jBw) > chromosome.problem->capacity) break;
                            sum = sum - (iAw + iBw) + (jAw + jBw);
                            freeItems[jA] = iAp;
                            freeItems[jB] = iBp;
                            chromosome.groups[orderGroups[i]]->items[iA] = getItem(chromosome, jAp);
                            chromosome.groups[orderGroups[i]]->items[iB] = getItem(chromosome, jBp);
                            iAp = jAp;
                            iAw = jAw;
                            iBp = jBp;
                            iBw = jBw;
                            jAp = freeItems[jA];
                            jAw = getItem(chromosome, jAp)->weight;
                            if (sum == chromosome.problem->capacity) {
                                ban = true;
                                break;
                            }
                        }

                    } if (ban) break;
                } if (ban) break;
            } if (ban) break;
        }

        // Actualiza el volumen del grupo luego de los intercambios
        chromosome.groups[orderGroups[i]]->volume = sum;

        // Establece el valor del contenedor mas vacio
        if (chromosome.getGroupVolume(orderGroups[i]) < chromosome.maxEmptyVolume) {
            chromosome.maxEmptyVolume = chromosome.getGroupVolume(orderGroups[i]);
        }

        else {
            // Verifica el grupo de inicio de llenado, si al grupo actual no puede contener el item menor
            if (chromosome.getGroupVolume(orderGroups[i]) + chromosome.problem->minWeight <= chromosome.problem->capacity) {
                // Se actualiza el grupo de inicio de llenado si es menor al anterior
                if (orderGroups[i] < startGroup) {
                    startGroup = orderGroups[i];
                }
            }
        }
    }

    // Se calcula la aptitud de los grupos que ya no pueden contener items
    for (i = 0; i < startGroup; i++) {
        chromosome.fitness += pow(((double)chromosome.getGroupVolume(i) / chromosome.problem->capacity), 2);
    }

    totalFreeItems += totalFree;

    // Si los items libres contiene items ñ grandes
    if (higherWeight < .5 * chromosome.problem->capacity) {
        // Se ordenan los objetos libres de forma aletoria
        freeItems.randomize(&chromosome.problem->config.seed, 0, totalFreeItems);
        // Se busca el  lighterWeight
        for (i = 0; i < totalFreeItems && lighterWeight > chromosome.problem->minWeight; i++) {
            if (lighterWeight > getItem(chromosome, freeItems[i])->weight) {
                lighterWeight = getItem(chromosome, freeItems[i])->weight;
            }
        }

    }
    else {
        // En caso contrario se orden en forma descencente segun su peso
        sortPermutationByItems(*chromosome.problem, freeItems, totalFreeItems);
        lighterWeight = chromosome.problem->items[freeItems[totalFreeItems - 1]]->weight; // aquiiii estaaa el errorrr 
    }

    // Si el item libre mas pqueño no entra dentro del grupo mas vacío se calcula la aptitud del resto de grupos
    if (lighterWeight > chromosome.problem->capacity - chromosome.maxEmptyVolume) {
        for (i = startGroup; i < chromosome.totalGroups; i++) {
            chromosome.fitness += pow(((double)chromosome.getGroupVolume(i) / chromosome.problem->capacity), 2);
        }
        // Se actualiza el numero de grupos como el total de grupos en la solución parcial
        startGroup = chromosome.totalGroups;
    }

    // Se llama a la función "firstFit" para insertar los elementos libres en el cromosoma
    for (i = 0; i < totalFreeItems - 1; i++) {
        firstFit(chromosome, freeItems[i], startGroup, false);
    }
    firstFit(chromosome, freeItems[i], startGroup, true);

}

// Realiza la mutación adaptativa de un cromosoma con una tasa de mutación dada.
void adaptiveMutation(Chromosome& chromosome, double mutationRate, int* seed, bool isCloned) {

    //Declara e inicializa variables
    long double _p_;
    long int i, j, numGroupsToRemove, leastGroup = 0, totalFreeItems = 0;

    // Inicializa objetos Permutation para guardar los elementos libres y orden de grupos del mas vacio al mas lleno.
    Permutation freeItems(chromosome.problem->numItems, 0);
    Permutation orderGroups(chromosome.totalGroups);

    // Si la solución es clonada, los grupos se ordenan aleatoriamente.
    if (isCloned) orderGroups.randomize(&chromosome.problem->config.seed, 0, chromosome.totalGroups);

    // Guarda en orderGroups el orden de los grupos según su volumen.
    sortGroupsByPermutation(chromosome, orderGroups, true);

    // Encuentra el numero de grupos que no estan en su maxima capacidad.
    i = 1;
    while (chromosome.getGroupVolume(orderGroups[i]) < chromosome.problem->capacity) {
        i++;
        if (i == chromosome.totalGroups) {
            break;
        }
    }

    // Calcula el número de grupos a eliminar con base en la tasa de mutación.
    _p_ = 1 / (float)(mutationRate);
    numGroupsToRemove = (long int)ceil(i * ((2 - i / (double)chromosome.totalGroups) / pow(i, _p_)) * (1 - ((double)get_rand(seed, (long int)ceil((1 / pow(i, _p_)) * 100)) / 100)));

    if (numGroupsToRemove == 0) {
        return;
    }

    //Recorre los grupos a eliminar y agrega los elementos libres a la lista de elementos libres.
    for (i = 0; i < numGroupsToRemove; i++) {
        for (j = 0; j < chromosome.groups[orderGroups[leastGroup]]->totalItems; j++) {
            freeItems[totalFreeItems++] = chromosome.groups[orderGroups[leastGroup]]->items[j]->position;
        }
        // Vacia los grupos eliminados de la solución.
        clearGroup(*chromosome.groups[orderGroups[leastGroup]]);
        leastGroup++;
    }
    // Elimina los grupos vacíos.
    removeEmptyGroups(chromosome);

    // Realiza el reordenamiento en pares de los elementos libres.
    pairwiseRearrangement(chromosome, freeItems, totalFreeItems);
}