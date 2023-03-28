#include "../include/qlearning.h"

float Qtable[numStates][numActions];

// Selecciona una acción basada en la política 𝜀-greedy
Action chooseAction(const State& state) {
    float randomValue = rand() / RAND_MAX;
    if (randomValue < epsilon) {
        // Acción aleatoria
        return static_cast<Action>(rand() % numActions);
    }
    else {
        // Acción con el mayor valor en la tabla Q
        return Qtable[state.speciesIndex][A1] > Qtable[state.speciesIndex][A2] ? A1 : A2;
    }
}

// Actualiza la tabla Q
void updateQTable(const State& state, const State& nextState, Action action, float reward) {
    float oldValue = Qtable[state.speciesIndex][action];
    float newValue = reward + gamma * max(Qtable[nextState.speciesIndex][A1], Qtable[nextState.speciesIndex][A2]);
    Qtable[state.speciesIndex][action] = oldValue + alpha * (newValue - oldValue);
}