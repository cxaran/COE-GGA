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
const int NUM_ITERATIONS = 1000;
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
    std::sort(instance.items.begin(), instance.items.end(), compareMin);
    inputFile.close();
    return instance;
}

void calculateFitness(Chromosome& chromosome) {
    chromosome.fitness = 0;
   
}

void initializePopulation(vector<Chromosome>& population) {
    
}

void solveCoe() {
    vector<Chromosome> population;
    initializePopulation(population);
}

int main() {
    Instance instance = readInstanceFromFile("D:\\instance.txt");
    cout << "Instance read from file: " << endl;
    cout << "Number of Items: " << instance.numItems << endl;
    cout << "Number of Groups: " << instance.numGroups << endl;
    cout << "Weights: " << endl;
    for (int i = 0; i < instance.numItems; i++) {
        for (int j = 0; j < instance.numGroups; j++) {
            cout << instance.items[i].weights[j] << " ";
        }
        cout << endl;
    }

    return 0;
}

