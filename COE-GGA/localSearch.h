#ifndef HEURISTICS_H
#define HEURISTICS_H

#include <vector>
#include <set>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include "../structures.h"

using namespace std;

//void removeJob(Group machine, Item item, int item_id);

void swapJobs(Group machine1, Group machines2, Item item1, Item item2);

int checkMoveSpan(Chromosome chromo, Group origin_machine, Group target_machine, Item job_to_move); /// a probar

int finalMakeSpan(Chromosome chromosome); ///////// listo

void swapTwoJobs(); //?

void oneJobRoutine(); 

void oneByOneSwapRoutine();

int uniquePairs();//? retorna una lista de pares unicos de los numeros 

void twoRoutineHelper();

void twoByTwoRoutine();

#endif

