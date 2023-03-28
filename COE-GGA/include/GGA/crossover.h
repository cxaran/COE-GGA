#ifndef CROSSOVER_H
#define CROSSOVER_H

#include "../structures.h"
#include "../heuristics.h"
#include "../permutation.h"

/**
 * @brief Verifica si algún item de un grupo se encuentra en un conjunto de items dado.
 * @param group El grupo que se desea verificar si contiene alguno de los items del conjunto.
 * @param items El conjunto de items del que se desea verificar si alguno se encuentra en el grupo.
 * @return Verdadero si al menos un item del conjunto se encuentra en el grupo, falso en caso contrario.
 */
bool containsItemInGroup(const Group& group, Permutation& items);

/**
 * @brief Realiza un crossover controlado entre dos cromosomas para crear un nuevo hijo.
 * @param child Cromosoma hijo resultado del crossover controlado.
 * @param a Cromosoma padre A utilizado para el crossover controlado.
 * @param b Cromosoma padre B utilizado para el crossover controlado.
*/
void controlledCrossover(Chromosome& child, const Chromosome& a, const Chromosome& b);

#endif
