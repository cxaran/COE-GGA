#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;

// Estructura para un artículo
struct Item { // trabajo
    int id; // ID del artículo
    double min; // Peso minimo del articulo
    vector<double> weights; // Peso del artículo en cada grupo
};

// Estructura para una instancia de un problema
struct Instance {
    int numItems; // Número de artículos
    int numGroups; // Número de grupos
    int capacity; // Capacidad de los grupos
    int knowBest; // Mejor solución conocida
    vector<Item*> items; // Artículos
};

// Estructura para un grupo
struct Group { //maquinas
    int id = -1; // ID del grupo
    double volume = 0; // Volumen del grupo
    vector<Item*> items; // Artículos en el grupo
};

// Estructura para la aptitud de un cromosoma
struct Fitness {
    double value = 0; // Valor de la aptitud
    int bins = 0; // Número de grupos
    double maxSpan = 0; // Máximo tamaño de los grupos
};

// Estructura para un cromosoma
struct Chromosome {
    vector<Group> groups; // Grupos
    const Instance* problem; // Instancia del problema
    Fitness fitness; // Aptitud del cromosoma
    int age; // La edad del cromosoma
};

// Estructura para una especie
struct Specie {
    int id; // El ID único de la especie
    vector<Chromosome> members; // Los miembros de la especie, un conjunto de cromosomas
    float size; // El porcentaje de la poblacion global que pertenece a la especie
};


bool compareMin(const Item* a, const Item* b);
bool compareFitness(const Chromosome& a, const Chromosome& b);
bool compareVolumeGroups(const Group& a, const Group& b);
void printChromosome(const Chromosome& chromosome, bool printGroups);
void printChromosomeAsJson(const Chromosome& chromosome, bool printGroups);
void calculateFitness(Chromosome& chromosome);
bool allItemsIncluded(const Chromosome& chromosome);
bool createNewGroupWithItem(Chromosome& chromosome, Item& item);
bool addItemToGroup(Group& group, Item& item);
void refactorGroupVolume(Group& group);

#endif