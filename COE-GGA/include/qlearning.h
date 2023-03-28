#ifndef QLEARNING_H
#define QLEARNING_H

#define	TOTALSPECIES 4 // Numero de especies

#include <cstdlib>
#include <algorithm>

using namespace std;

// Estructuras de datos
struct State {
    int speciesIndex = 0; // Índice de la especie dominante
};

enum Action {
    A1, // No modifica los porcentajes
    A2  // Aumenta el porcentaje de la especie dominante y reduce el porcentaje de las demás
};

// Parámetros del algoritmo de Q-learning
const float alpha = 0.1;
const float gamma = 0.9;
const float epsilon = 0.1;
const int numStates = TOTALSPECIES;
const int numActions = 2;
const float rewards[] = { 1.0, -1.0 };

// Selecciona una acción basada en la política 𝜀-greedy
Action chooseAction(const State& state);

// Actualiza la tabla Q
void updateQTable(const State& state, const State& nextState, Action action, float reward);

#endif