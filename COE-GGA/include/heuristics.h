#ifndef HEURISTICS_H
#define HEURISTICS_H

#include "structures.h"

using namespace std;

/**
 * @brief Calcula el límite inferior L2 para el problema.
 * @param instance La instancia del problema a resolver.
 */
void lowerBound(Instance& instance);

/**
 * @brief Realiza la asignación de un artículo a un grupo existente en un cromosoma mediante el algoritmo First Fit.
 * @param chromosome Referencia al cromosoma en el que se realiza la asignación.
 * @param itemPos Posición del artículo en la lista de artículos de la instancia del problema.
 * @param startGroup Referencia al índice del primer grupo a considerar en la asignación.
 * @param isLast Indicador de si es el último artículo por asignar en la solución.
*/
void firstFit(Chromosome& chromosome, int itemPos, int& startGroup, bool isLast);

/**
 * @brief Asigna elementos a grupos usando la heurística "firstFit".
 * @param chromosome El cromosoma en el que se asignarán los elementos.
 * @param items El vector de elementos a asignar.
*/
void firstFitN_(Chromosome& chromosome);

/**
 * @brief Inicializa la población a un tamaño dado.
 * @param instance La instancia del problema a resolver.
 * @param population La población del problema a resolver.
 * @param populationSize El tamaño deseado para la población.
 * @param applyFirstFit Flag que indica si se aplicará o no el algoritmo firstFitN a la solución.
*/
void initializePopulation(Instance& instance, Population& population, int populationSize, bool applyFirstFit);

#endif

