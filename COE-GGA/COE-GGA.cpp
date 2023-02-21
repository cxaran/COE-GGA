// COE-GGA.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "include/structures.h"
#include "include/heuristics.h"
#include "include/localSearch.h"
using namespace std;

// Constantes
const int NUM_SPECIES = 1;
const int POPULATION_SIZE = 100;
const int NUM_ITERATIONS = 500;

// Función para leer una instancia de un archivo
Instance readInstanceFromFile(string fileName) {
    Instance* instance = new Instance();
    ifstream inputFile(fileName);
    inputFile >> instance->numItems >> instance->numGroups >> instance->capacity >> instance->knowBest;
    // Leer los pesos de cada elemento

    double weight;
    for (int i = 0; i < instance->numItems; i++) {
        Item* item = new Item();
        item->id = i;
        for (int j = 0; j < instance->numGroups; j++) {
            inputFile >> weight;
            item->weights.push_back(weight);
        }
        // Calcular el peso mínimo y agregar el elemento a la instancia
        item->min = *min_element(item->weights.begin(), item->weights.end());
        instance->items.push_back(item);
    }
    // Cerrar el archivo
    inputFile.close();
    return *instance;
}

// Función para inicializar la población, regresa la poblacion ordenada
vector<Chromosome> initializePopulation(Instance& instance, int SIZE) {
    vector<Chromosome> population;
    vector<Item*> items = instance.items;
    // Ordenar los elementos por peso mínimo
    sort(items.begin(), items.end(), compareMin);
    // Generar cromosomas hasta que se alcance el tamaño de población deseado
    int method = 0;
    while (population.size() < SIZE) {
        Chromosome* chromosome = new Chromosome();
        chromosome->problem = &instance;
        if(method == 0)firstFit(*chromosome, instance.items);
        //bestFit(*chromosome, items);
        if(method == 1)bestFitN(*chromosome, items);
        // Verificar que todos los elementos estén incluidos en el cromosoma
        if (allItemsIncluded(*chromosome)) {
            // Calcular el fitness del cromosoma
            calculateFitness(*chromosome);
            // Busqueda local del cromosoma
            //reorderByCapacity(*chromosome);
            // Agregar el cromosoma a la población
            chromosome->age = 0;
            population.push_back(*chromosome);
        }
        method++;
        if (method > 1) {
            // Mezclar los elementos de forma aleatoria
            random_shuffle(items.begin(), items.end());
            method = 0;
        }
    }
    sort(population.begin(), population.end(), compareFitness);
    return population;
}

Chromosome coevolution(Instance& instance) {

    // Species
    vector<Specie> species;
    for (int i = 0; i < NUM_SPECIES; i++){
        Specie specie;
        specie.id = i;
        specie.size = POPULATION_SIZE / NUM_SPECIES;
        specie.members = initializePopulation(instance, specie.size);
        species.push_back(specie);
    }

    // Repitir ara el número especificado de iteraciones
    //for (int generation = 1; generation <= NUM_ITERATIONS; ++generation) {
       //cout << generation << endl;
       // geneticAlgorithm(species[0], generation, 0.01, 0.8, 0.0);}
        
    //}

    Chromosome newSol = localSearch(species[0].members[0]);
    
    return newSol;
}

int main(int argc, char* argv[]) {
    Instance instance = readInstanceFromFile(argv[1]);
    Chromosome solution = coevolution(instance);
    printChromosomeAsJson(solution, true);
    return 0;
}



