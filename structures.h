#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <cmath>

// Estructura para un art�culo
struct Item {
    int id; // ID del art�culo
    int min; // M�nimo de art�culos necesarios
    std::vector<int> weights; // Peso del art�culo en cada grupo
};

// Estructura para una instancia de un problema
struct Instance {
    int numItems; // N�mero de art�culos
    int numGroups; // N�mero de grupos
    int capacity; // Capacidad de los grupos
    int knowBest; // Mejor soluci�n conocida
    std::vector<Item> items; // Art�culos
};

// Estructura para un grupo
struct Group {
    int id = -1; // ID del grupo
    int volume = 0; // Volumen del grupo
    std::vector<Item> items; // Art�culos en el grupo
};

// Estructura para la aptitud de un cromosoma
struct Fitness {
    double value = 0; // Valor de la aptitud
    int bins = 0; // N�mero de grupos
    int maxSpan = 0; // M�ximo tama�o de los grupos
};

// Estructura para un cromosoma
struct Chromosome {
    std::vector<Group> groups; // Grupos
    Instance problem; // Instancia del problema
    Fitness fitness; // Aptitud del cromosoma
};

bool compareMin(const Item& a, const Item& b);
bool compareFitness(const Chromosome& a, const Chromosome& b);
void printChromosome(const Chromosome& chromosome, bool printGroups);
void firstFit(Chromosome& chromosome, const std::vector<Item>& items);
void bestFit(Chromosome& chromosome, const std::vector<Item>& items);
void calculateFitness(Chromosome& chromosome);
bool allItemsIncluded(const Chromosome& chromosome);

#endif