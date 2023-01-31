// COE-GGA.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include <fstream>
#include <vector>

#include "tools.h"

using namespace std;

const int NUM_SPECIES = 2;
const int POPULATION_SIZE = 100;
const int NUM_ITERATIONS = 1000;
const double MUTATION_RATE = 0.01;

struct Instance {
    int numItems;
    int numGroups;
    int capacity;
    int knowBest;
    vector<vector<int>> weights;
};

struct Group {
    int id;
    int bulk;
    std::vector<int> items;
};

struct Chromosome {
    std::vector<Group> groups;
    Instance problem;
    double fitness;
};

Instance readInstanceFromFile(string fileName) {
    Instance instance;
    ifstream inputFile(fileName);
    inputFile >> instance.numItems >> instance.numGroups >> instance.capacity >> instance.knowBest;
    int item;
    for (int i = 0; i < instance.numItems; i++) {
        vector<int> weights;
        for (int j = 0; j < instance.numGroups; j++) {
            inputFile >> item;
            weights.push_back(item);
        }
        instance.weights.push_back(weights);
    }
    std::sort(instance.weights.begin(), instance.weights.end(), compareMin);
    inputFile.close();
    return instance;
}

void calculateFitness(Chromosome& chromosome) {
    chromosome.fitness = 0;
   
}

void initializePopulation(std::vector<Chromosome>& population) {
   
}

void solveCoe() {
    std::vector<Chromosome> population;
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
            cout << instance.weights[i][j] << " ";
        }
        cout << endl;
    }



    return 0;
}

