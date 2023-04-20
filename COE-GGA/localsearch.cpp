#include "include/heuristics.h"
#include "include/localsearchh.h"
#include "include/fileio.h"
#include <random>


using namespace std;

// Funcion para crear un nuevo grupo sin item
bool createNewGroupWithoutItem(Chromosome& chromosome, int groups_chromo,int indice) {
	// Verificar si todav a se pueden crear m s grupos.
	Group* group;
	if (groups_chromo < chromosome.problem->numGroups) {
		group = new Group();
		chromosome.groups[chromosome.totalGroups] = group;
		group->id = chromosome.totalGroups;
		chromosome.totalGroups++;
		//cout << chromosome.groups.size() << endl;
		return true;
	}
	// Salir con un codigo de error.
	else {
		//cout << groups_chromo << endl;
		exit(3);
		return false;
	}
}

bool aux_add_item(Group& group, Item& item) {
	//cout << "probar" << item.weights[group2.id] << "\n";
	 // Verificar si el objeto ya est  en el grupo.
	//for (const auto& i : group.items) if (i->id == item.id) return false;
	//agregar el objeto.
	group.items[group.totalItems++] = &item;
	return true;
}

void refactorGroupVolume_bestk(Group& group) {
	group.volume = 0;
	for (const Item* item : group.items) {
		group.volume += item->weight;
	}
}
void reorder_chromosome(Chromosome& chromosome,int groups_chromo) {

	for (int i = 0; i < groups_chromo; i++) {
		for (Item* item : chromosome.groups[i]->items) {
			for (int z = 0; z < groups_chromo; z++) {
				if (item->weight == item->weights[z]) {
					bool added = aux_add_item(*chromosome.groups[z], *item);
					if (added == true) {
						removeItemFromGroup(*chromosome.groups[i],i);
					}
					break;
				}
			}

		}
		chromosome.groups[i]->volume = 0;
	}
	for (int i = 0; i < groups_chromo; i++) {
		refactorGroupVolume_bestk(*chromosome.groups[i]);
	}
}
void main_localSearch(Chromosome& chromosome) {
    //Numeros de maquinas
	int maquinas = chromosome.problem->numGroups;
	//cout << maquinas << endl;
	//Itera hasta que todas las maquinas se hayan procesado
	//Group* groups = *chromosome.groups;
	//int groups_chromo = sizeof(groups);
	//cout << groups_chromo;
	int groups_chromo = chromosome.totalGroups;
	if (groups_chromo!= maquinas) {
		for (int i = groups_chromo; i < maquinas; i++) {
			bool adde = createNewGroupWithoutItem(chromosome,groups_chromo,i);
		}
	}
	//printChromosomeAsJson(chromosome, true);
	reorder_chromosome(chromosome,groups_chromo);
	printChromosomeAsJson(chromosome, true);
	//oneJobRoutine(chromosome);
	//oneByOneSwapRoutine(chromosome);
	//twoByTwoSwapRoutine(chromosome);
}