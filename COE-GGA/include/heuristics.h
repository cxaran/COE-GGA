#ifndef HEURISTICS_H
#define HEURISTICS_H

#include "structures.h"

using namespace std;

/**
 * @brief Calcula el l�mite inferior L2 para el problema.
 * @param instance La instancia del problema a resolver.
 */
void lowerBound(Instance& instance);

/**
 * @brief Realiza la asignaci�n de un art�culo a un grupo existente en un cromosoma mediante el algoritmo First Fit.
 * @param chromosome Referencia al cromosoma en el que se realiza la asignaci�n.
 * @param itemPos Posici�n del art�culo en la lista de art�culos de la instancia del problema.
 * @param startGroup Referencia al �ndice del primer grupo a considerar en la asignaci�n.
 * @param isLast Indicador de si es el �ltimo art�culo por asignar en la soluci�n.
*/
void firstFit(Chromosome& chromosome, int itemPos, int& startGroup, bool isLast);

/**
 * @brief Asigna elementos a grupos usando la heur�stica "firstFit".
 * @param chromosome El cromosoma en el que se asignar�n los elementos.
 * @param items El vector de elementos a asignar.
*/
void firstFitN_(Chromosome& chromosome);

/**
 * @brief Inicializa la poblaci�n a un tama�o dado.
 * @param instance La instancia del problema a resolver.
 * @param population La poblaci�n del problema a resolver.
 * @param populationSize El tama�o deseado para la poblaci�n.
 * @param applyFirstFit Flag que indica si se aplicar� o no el algoritmo firstFitN a la soluci�n.
*/
void initializePopulation(Instance& instance, Population& population, int populationSize, bool applyFirstFit);

#endif

