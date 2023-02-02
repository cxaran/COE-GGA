#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <cmath>

// Estructura para un artículo
struct Item {
    int id; // ID del artículo
    int min; // Mínimo de artículos necesarios
    std::vector<int> weights; // Peso del artículo en cada grupo
};

// Estructura para una instancia de un problema
struct Instance {
    int numItems; // Número de artículos
    int numGroups; // Número de grupos
    int capacity; // Capacidad de los grupos
    int knowBest; // Mejor solución conocida
    std::vector<Item> items; // Artículos
};

// Estructura para un grupo
struct Group {
    int id = -1; // ID del grupo
    int volume = 0; // Volumen del grupo
    std::vector<Item> items; // Artículos en el grupo
};

// Estructura para la aptitud de un cromosoma
struct Fitness {
    double value = 0; // Valor de la aptitud
    int bins = 0; // Número de grupos
    int maxSpan = 0; // Máximo tamaño de los grupos
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