// COE-GGA.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "structures.h"
#include "tools.h"

using namespace std;

const int NUM_SPECIES = 2;
const int POPULATION_SIZE = 100;
const int NUM_ITERATIONS = 500;
const double MUTATION_RATE = 0.01;


Instance readInstanceFromFile(string fileName) {
    Instance instance;
    ifstream inputFile(fileName);
    inputFile >> instance.numItems >> instance.numGroups >> instance.capacity >> instance.knowBest;
    int weight;
    for (int i = 0; i < instance.numItems; i++) {
        Item item;
        item.id = i;
        for (int j = 0; j < instance.numGroups; j++) {
            inputFile >> weight;
            item.weights.push_back(weight);
        }
        item.min = *min_element(item.weights.begin(), item.weights.end());
        instance.items.push_back(item);
    }
    sort(instance.items.begin(), instance.items.end(), compareMin);
    inputFile.close();
    return instance;
}

void initializePopulation(const Instance& instance,vector<Chromosome>& population) {
    vector<Item> items = instance.items;
    while (population.size() < POPULATION_SIZE) {
        Chromosome chromosome;
        chromosome.problem = instance;
        //firstFit(chromosome, instance.items);
        bestFit(chromosome, items);
        if (allItemsIncluded(chromosome)) {
            population.push_back(chromosome);
            printChromosome(chromosome);
        }
        random_shuffle(items.begin(), items.end());
    }
}

void coevolution(const Instance& instance) {
    vector<Chromosome> population;
    initializePopulation(instance,population);
}

int main() {
    Instance instance = readInstanceFromFile("D:\\instance.txt");

    coevolution(instance);

    return 0;
}

