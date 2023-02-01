#include "structures.h"

void addItemToGroup(Group& group, const Item& item) {
    for (const auto& i : group.items) if (i.id == item.id) return;
    group.volume += item.weights[group.id];
    group.items.push_back(item);
}

bool createNewGroupWithItem(Chromosome& chromosome, const Item& item) {
    if (chromosome.groups.size() < chromosome.problem.numGroups) {
        Group group;
        group.id = chromosome.groups.size();
        addItemToGroup(group, item);
        chromosome.groups.push_back(group);
        return true;
    }
    else return false;
}

void firstFit(Chromosome& chromosome, const std::vector<Item>& items) {
    for (const auto& item : items) {
        bool added = false;
        for (auto& group : chromosome.groups) {
            if (group.volume + item.weights[group.id] <= chromosome.problem.capacity) {
                addItemToGroup(group, item);
                added = true;
                break;
            }
        }
        if (!added) createNewGroupWithItem(chromosome, item);
    }
}

void bestFit(Chromosome& chromosome, const std::vector<Item>& items) {
    for (const auto& item : items) {
        Group bestGroup;
        int bestFit = chromosome.problem.capacity + 1;
        for (auto& group : chromosome.groups) {
            int freeSpace = chromosome.problem.capacity - group.volume;
            if (item.weights[group.id] <= freeSpace && freeSpace > bestFit) {
                bestFit = freeSpace;
                bestGroup = group;
            }
        }
        if (bestGroup.id!= -1) addItemToGroup(bestGroup, item);
        else createNewGroupWithItem(chromosome, item);
    }
}


