#ifndef MUTATION_H
#define MUTATION_H

#include "../structures.h"
#include "../heuristics.h"
#include "../permutation.h"

/**
 * @brief Funci�n para realizar reacomodo por pares en un cromosoma dado un conjunto de objetos libres.
 * @param chromosome Referencia al cromosoma en el que se desea realizar el reacomodo.
 * @param freeItems Permutaci�n de objetos libres, representados por sus posiciones en la instancia del problema.
 * @param totalFreeItems Entero que indica el n�mero total de objetos libres en la permutaci�n freeItems.
*/
void pairwiseRearrangement(Chromosome& chromosome, Permutation& freeItems, long int totalFreeItems);

/**
 * @brief Realiza la mutaci�n adaptativa de un cromosoma con una tasa de mutaci�n dada.
 * @param chromosome Cromosoma a mutar.
 * @param mutationRate Tasa de mutaci�n a aplicar en el cromosoma.
 * @param isCloned Indica si el cromosoma ha sido clonado previamente.
*/
void adaptiveMutation(Chromosome& chromosome, double mutationRate, int* seed, bool isCloned);

#endif