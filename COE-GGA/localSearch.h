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

//Funcion para eliminar item
bool DeleteItemToGroup(Group& group, Item& item);

//Funcion para identificar si se terminaron de procesar todas las maquinas
bool isDone(vector<bool> d_list);

//Funcion para mover un trabajo a otra maquina
bool moveJob(Group origin_machine, Group target_machine, Item* job_to_move);

//Funcion para checar si mejora mover el span
bool checkMoveSpan(Chromosome chromo, Group origin_machine, Group target_machine, Item* job_to_move);

//Funcion para calcular el makespan final
double finalMakeSpan(Chromosome chromosome);

//Funcion principal del localSearch
void main_localSearch(Chromosome chromosome); 

//Funcion para intercambiar 2 trabajos
bool swapTwoJobs(Group origin_machine, Group target_machine, vector<Item*>pair1, vector<Item*>pair2);

//Funcion para checar si mover el trabajo a otra maquina representa una mejoria
bool checkSwapSpan(Chromosome chromosome, Group machine, Group target_machine, Item* origin_job, Item* target_job);

//Intercambiar tareas entre 2 maquinas
bool swapJobs(Group origin_machine, Group target_machine, Item* origin_job, Item* target_job);

// Funcion para crear un nuevo grupo sin item
bool createNewGroupWithoutItem(Chromosome& chromosome);

//Rutina para mover un trabajo
void oneJobRoutine(Chromosome chromosome);

//Rutina para mover intercambiar un trabajo
void oneByOneSwapRoutine(Chromosome chromosome);

//Funcion para generar pares
vector<vector<Item*>> uniquePairs(vector<Item*> source, int maquinas);

//Funcion de ayuda para mover trabajos
bool twoRoutineHelper(Chromosome chromosome, Group machine, int num_trabajos, int maquinas);

//Funcion de ayuda para mover 2 trabajos
void twoByTwoSwapRoutine(Chromosome chromosome);

//Funcion para generar pares aleatorios
vector<vector<Item*>> rand_pair(vector<Item*> source, int num_par);

//Funcion para checar si mover el par de trabajos a otras maquinas representa mejoria
bool checkTwoSwapSpan(Chromosome chromosome, Group origin_machine, Group target_machine, vector<Item*>pair1, vector<Item*>pair2);

#endif

