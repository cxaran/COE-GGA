#include "../../include/GGA/crossover.h"

// Funci�n que verifica si todos los elementos de un grupo pueden ser a�adidos a la soluci�n
bool containsItemInGroup(const Group& group, Permutation& items) {
    long int i;
    for (i = 0; i < group.totalItems; i++) {
        // Si el �tem no ha sido a�adido todav�a a la soluci�n, se marca como a�adido
        if (items[group.items[i]->position] == 0) {
            items[group.items[i]->position] = 1;
        }
        // Si el �tem ya ha sido a�adido a la soluci�n anteriormente, se deshacen todos los cambios realizados
        else {
            for (i = i - 1; i >= 0; i--) items[group.items[i]->position] = 0;
            // Retorna falso, indicando que el grupo no puede ser agregado
            return false;
        }
    }
    // Si todos los �tems del grupo pueden ser agregados sin conflicto, se retorna verdadero
    return true;
}

// Funci�n de cruza controlada
void controlledCrossover(Chromosome& child, const Chromosome& a, const Chromosome& b) {
    
    // Limpia el cromosoma "child" mediante la funci�n
    clearChromosome(child);

    // Inicializa variables
    long int k, counter;
    bool ban;

    // Crea dos permutaciones "items" y "freeItems" de tama�o "numItems".
    Permutation items(child.problem->numItems, 0); // Contiene los items que ya estan en la soluci�n.
    Permutation freeItems(child.problem->numItems, 0); // Contiene los items que no estan en la soluci�n.

    // Establece el valor del contenedor mas vacio de "child" como la capacidad del problema.
    child.maxEmptyVolume = child.problem->capacity;

    // Obtiene el valor m�ximo de grupos entre "a" y "b" y lo almacena en la variable "counter".
    counter = a.totalGroups > b.totalGroups ? a.totalGroups :b.totalGroups;

    // Crea dos permutaciones para padre a y para padre b de tama�o "counter" y las inicializa aleatoriamente.
    Permutation orderA(counter);
    Permutation orderB(counter);
    orderA.randomize(&child.problem->config.seed, 0, a.totalGroups);
    orderB.randomize(&child.problem->config.seed, 0, b.totalGroups);

    // Guarda el roden los grupos ( mayor a menor volumen ) del cromosoma "a" y "b" en las permutaciones "orderA" y "orderB" respectivamente.
    sortGroupsByPermutation(a, orderA, false);
    sortGroupsByPermutation(b, orderB, false);

    // Recorre todos los grupos en orden de "orderA" y "orderB" y realiza la operaci�n de cruce controlada.
    for (k = 0; k < a.totalGroups; k++) {
        // Si el volumen del grupo de "a" es mayor o igual al volumen del grupo de "b".
        if (a.getGroupVolume(orderA[k]) >= b.getGroupVolume(orderB[k])) {
            // Se comprueba si el grupo de "a" contiene alg�n art�culo en la permutaci�n "items".
            ban = containsItemInGroup(*a.groups[orderA[k]], items);
            if (ban) {
                // Se agrega el grupo al cromosoma "child"
                addGroupToChromosome(child, *a.groups[orderA[k]]);
            }
            // Luego se comprueba si el grupo de "b" en la posici� n orderB[k] existe, de ser asi su volumen sera mayor a 0.
            if (b.getGroupVolume(orderB[k]) > 0) {
                // Se comprueba si el grupo de "b" contiene alg�n art�culo en la permutaci�n "items".
                ban = containsItemInGroup(*b.groups[orderB[k]], items);
                if (ban) {
                    // Se agrega el grupo al cromosoma "child"
                    addGroupToChromosome(child, *b.groups[orderB[k]]);
                }
            }
        }
        // Si el volumen del grupo de "a" es menor que el volumen del grupo de "b".
        else {
            // Se comprueba si el grupo de "b" en la posici� n orderB[k] existe, de ser asi su volumen sera mayor a 0.
            if (b.getGroupVolume(orderB[k]) > 0) {
                // Se comprueba si el grupo de "b" contiene alg�n art�culo en la permutaci�n "items".
                ban = containsItemInGroup(*b.groups[orderB[k]], items);
                if (ban) {
                    // Si no hay articulos repetidos se agrega el grupo al cromosoma "child"
                    addGroupToChromosome(child, *b.groups[orderB[k]]);
                }
            }
            // Se comprueba si el grupo de "a" contiene alg�n art�culo en la permutaci�n "items".
            ban = containsItemInGroup(*a.groups[orderA[k]], items);
            if (ban) {
                // Se agrega el grupo al cromosoma "child"
                addGroupToChromosome(child, *a.groups[orderA[k]]);
            }
        }
    }
    k = 0;
    // Recorre todos los elementos de "items" y los agrega a "freeItems" si su valor es 0.
    for (counter = 0; counter < child.problem->numItems; counter++) {
        if (items[counter] == 0) {
            freeItems[k++] = counter;
        }
    }
    // Si "freeItems" no est� vac�o, llama a la funci�n "firstFit" para insertar los elementos libres en el cromosoma "child".
    if (k > 0){
        int startGroup = 0;
        for (counter = 0; counter < k - 1; counter++) {
            firstFit(child, freeItems[counter], startGroup, false);
        }
        firstFit(child, freeItems[counter], startGroup, true);
    }
    // Si "freeItems" est� vac�o, actualiza la aptitud del cromosoma "child" para sus grupos.
    else {
        for (k = 0; k < child.totalGroups; k++) {
            child.fitness += pow(((double)child.getGroupVolume(k) / child.problem->capacity), 2);
        }
    }

}