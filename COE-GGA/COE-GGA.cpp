// COE-GGA.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "include/structures.h"
#include "include/tools.h"

using namespace std;

// Constantes
const int NUM_SPECIES = 2;
const int POPULATION_SIZE = 100;
const int NUM_ITERATIONS = 500;
const double MUTATION_RATE = 0.01;

// Función para leer una instancia de un archivo
Instance readInstanceFromFile(string fileName) {
    Instance instance;
    ifstream inputFile(fileName);
    inputFile >> instance.numItems >> instance.numGroups >> instance.capacity >> instance.knowBest;
    // Leer los pesos de cada elemento
    int weight;
    for (int i = 0; i < instance.numItems; i++) {
        Item item;
        item.id = i;
        for (int j = 0; j < instance.numGroups; j++) {
            inputFile >> weight;
            item.weights.push_back(weight);
        }

        // Calcular el peso mínimo y agregar el elemento a la instancia
        item.min = *min_element(item.weights.begin(), item.weights.end());
        instance.items.push_back(item);
    }

    // Ordenar los elementos por peso mínimo
    sort(instance.items.begin(), instance.items.end(), compareMin);

    // Cerrar el archivo
    inputFile.close();
    return instance;

}

// Función para inicializar la población
void initializePopulation(const Instance& instance,vector<Chromosome>& population) {
    vector<Item> items = instance.items;
    // Generar cromosomas hasta que se alcance el tamaño de población deseado
    while (population.size() < POPULATION_SIZE) {
        Chromosome chromosome;
        chromosome.problem = instance;
        //firstFit(chromosome, instance.items);
        //bestFit(chromosome, items);
        bestFitN(chromosome, items);

        // Verificar que todos los elementos estén incluidos en el cromosoma
        if (allItemsIncluded(chromosome)) {
            // Calcular el fitness del cromosoma
            calculateFitness(chromosome);

            // Agregar el cromosoma a la población
            population.push_back(chromosome);
        }

        // Mezclar los elementos de forma aleatoria
        random_shuffle(items.begin(), items.end());
    }
}

Chromosome coevolution(const Instance& instance) {
    vector<Chromosome> population;
    // Inicializar la población
    initializePopulation(instance,population);
    sort(population.begin(), population.end(), compareFitness);
    return population[0];
}

int main(int argc, char* argv[]) {
    Instance instance = readInstanceFromFile(argv[1]);
    Chromosome solution = coevolution(instance);
    printChromosome(solution, true);
    return 0;
}

