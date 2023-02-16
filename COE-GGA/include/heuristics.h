#ifndef HEURISTICS_H
#define HEURISTICS_H

#include <vector>
#include <set>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include "structures.h"

using namespace std;

//Función que asigna elementos a grupos usando la heurística "firstFit"
void firstFit(Chromosome& chromosome,const vector<Item*>& items);

//Función que asigna elementos a grupos usando la heurística "bestFit"
void bestFit(Chromosome& chromosome,const vector<Item*>& items);
void bestFitN(Chromosome& chromosome,vector<Item*>& items);

// Función que resuelve el problema mediante una estrategia genética
void geneticAlgorithm(Specie& specie, int GENERATIOS, float ELITE_SIZE, float MUTATION_RATE, float CROSSOVER_RATE);

// Función que resuelve el problema mediante una búsqueda local
//void localSearch(const Problem& problem, const Chromosome& initialSolution);

// Función que aplica el algoritmo First Fit con capacidad modificada
void reorderByCapacity(Chromosome& chromosome);

#endif

