#ifndef MUTATION_H
#define MUTATION_H

#include "../structures.h"
#include "../heuristics.h"
#include "../permutation.h"

/**
 * @brief Función para realizar reacomodo por pares en un cromosoma dado un conjunto de objetos libres.
 * @param chromosome Referencia al cromosoma en el que se desea realizar el reacomodo.
 * @param freeItems Permutación de objetos libres, representados por sus posiciones en la instancia del problema.
 * @param totalFreeItems Entero que indica el número total de objetos libres en la permutación freeItems.
*/
void pairwiseRearrangement(Chromosome& chromosome, Permutation& freeItems, long int totalFreeItems);

/**
 * @brief Realiza la mutación adaptativa de un cromosoma con una tasa de mutación dada.
 * @param chromosome Cromosoma a mutar.
 * @param mutationRate Tasa de mutación a aplicar en el cromosoma.
 * @param isCloned Indica si el cromosoma ha sido clonado previamente.
*/
void adaptiveMutation(Chromosome& chromosome, double mutationRate, int* seed, bool isCloned);

#endif