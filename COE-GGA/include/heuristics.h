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

// Función que realiza el "firstFit" con reacomodo
void firstFitR(Chromosome& chromosome,const vector<Item*>& itemsConst);

// Función que resuelve el problema mediante una estrategia genética
void geneticAlgorithm(Specie& specie, int GENERATIOS, float ELITE_SIZE, float CROSSOVER_RATE, float MUTATION_RATE);

#endif

