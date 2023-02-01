#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <vector>

struct Item {
    int id;
    int min;
    std::vector<int> weights;
};

struct Instance {
    int numItems;
    int numGroups;
    int capacity;
    int knowBest;
    std::vector<Item> items;
};

struct Group {
    int id;
    int volume;
    std::vector<Item> items;
};

struct Chromosome {
    std::vector<Group> groups;
    Instance problem;
    double fitness;
};

#endif