#ifndef LOCAL_H
#define LOCAL_H

#include <vector>
#include <set>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include "include/structures.h"

using namespace std;

//Funcion para mover trabajos
void swapJobs(Group machine1, Group machines2, Item item1, Item item2);

//Funcion para checar si mejora mover el span
int checkMoveSpan(Chromosome chromo, Group origin_machine, Group target_machine, Item job_to_move); 

//Funcion para calcular el makespan final
int finalMakeSpan(Chromosome chromosome);

//Funcion principal del localSearch
void main_localSearch(Chromosome chromosome); 

//Funcion para intercambiar 2 trabajos
void swapTwoJobs();

//Rutina para mover un trabajo
void oneJobRoutine(Chromosome chromosome);

//Rutina para mover intercambiar un trabajo
void oneByOneSwapRoutine();

//Funcion para generar pares
int uniquePairs();

//Funcion de ayuda para mover trabajos
void twoRoutineHelper();

//Funcion de ayuda para mover 2 trabajos
void twoByTwoRoutine();

#endif

