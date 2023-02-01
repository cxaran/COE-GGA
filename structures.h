#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <vector>
#include <algorithm>
#include <iostream>

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
    int id  = -1;
    int volume = 0;
    std::vector<Item> items;
};

struct Chromosome {
    std::vector<Group> groups;
    Instance problem;
    double fitness = 0;
};

void firstFit(Chromosome& chromosome, const std::vector<Item>& items);
void bestFit(Chromosome& chromosome, const std::vector<Item>& items);

#endif