#include "structures.h"

void printChromosome(const Chromosome& chromosome) {
    std::cout << "Group information:\n";
    for (const auto& group : chromosome.groups) {
        std::cout << "Group ID: " << group.id << " ";
        std::cout << "Volume: " << group.volume << " ";
        std::cout << "Items: ";
        for (const auto& item : group.items) {
            std::cout << item.id << " ";
        }
        std::cout << "\n";
    }
    std::cout << "*************************\n";
}

bool addItemToGroup(Group& group, const Item& item) {
    for (const auto& i : group.items) if (i.id == item.id) return false;
    group.volume += item.weights[group.id];
    group.items.push_back(item);
    return true;
}

bool createNewGroupWithItem(Chromosome& chromosome, const Item& item) {
    if (chromosome.groups.size() < chromosome.problem.numGroups) {
        Group group;
        group.id = chromosome.groups.size();
        addItemToGroup(group, item);
        chromosome.groups.push_back(group);
        return true;
    }
    else {
        exit(3);
        return false; 
    }
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
        int bestGroupId = -1;
        int bestFit = 0;
        for (int i = 0; i < chromosome.groups.size(); ++i) {
            int freeSpace = chromosome.problem.capacity - chromosome.groups[i].volume;
            if (item.weights[chromosome.groups[i].id] <= freeSpace && freeSpace > bestFit) {
                bestFit = freeSpace;
                bestGroupId = i;
            }
        }
        if (bestGroupId != -1) {
            if (!addItemToGroup(chromosome.groups[bestGroupId], item)) exit(3);
        }
        else {
            if (!createNewGroupWithItem(chromosome, item)) exit(3);
        }
    }
}

bool allItemsIncluded(const Chromosome& chromosome) {
    for (const auto& item : chromosome.problem.items) {
        bool itemFound = false;
        for (const auto& group : chromosome.groups) {
            for (const auto& groupItem : group.items) {
                if (groupItem.id == item.id) {
                    itemFound = true;
                    break;
                }
            }
            if (itemFound) break;
        }
        if (!itemFound) return false;
    }
    return true;
}

