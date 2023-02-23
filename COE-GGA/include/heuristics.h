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

// Funci�n que realiza el "firstFit" con reacomodo
void firstFitR(Chromosome& chromosome,const vector<Item*>& itemsConst);

// Funci�n que resuelve el problema mediante una estrategia gen�tica
void geneticAlgorithm(Specie& specie, int GENERATIOS, float ELITE_SIZE, float CROSSOVER_RATE, float MUTATION_RATE);

#endif

