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

//Funci�n que asigna elementos a grupos usando la heur�stica "firstFit"
void firstFit(Chromosome& chromosome,const vector<Item*>& items);

//Funci�n que asigna elementos a grupos usando la heur�stica "bestFit"
void bestFit(Chromosome& chromosome,const vector<Item*>& items);
void bestFitN(Chromosome& chromosome,vector<Item*>& items);

// Funci�n que resuelve el problema mediante una estrategia gen�tica
void geneticAlgorithm(Specie& specie, int GENERATIOS, float ELITE_SIZE, float MUTATION_RATE, float CROSSOVER_RATE);

// Funci�n que resuelve el problema mediante una b�squeda local
//void localSearch(const Problem& problem, const Chromosome& initialSolution);

// Funci�n que aplica el algoritmo First Fit con capacidad modificada
bool reorderByCapacity(Chromosome& chromosome);

#endif

