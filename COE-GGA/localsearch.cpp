#include "include/heuristics.h"
#include "include/localsearchh.h"
#include "include/fileio.h"
#include <random>


using namespace std;
//Determina el maximo makespan actual
double finalMakeSpan(Chromosome& chromosome) {
	double max = 0;
	//Recorre los grupos-- maquinas
	for (int i = 0; i < chromosome.totalGroups; i++)
	{
		//Compara el makespan actual de la maquina y si es mayor que el maximo, actualiza la variable
		if (chromosome.groups[i]->volume > max) {
			max = chromosome.groups[i]->volume;
		}
	}
	return max;
}
//Checa si representa una mejora mover el trabajo a otra maquina
bool checkMoveSpan(Chromosome& chromo, Group& origin_machine, Group& target_machine, Item* job_to_move) {
	//Se obtiene makespan actual
	double cur_span = finalMakeSpan(chromo);
	//cout << "cur_span" << cur_span<< "\n";
	//Span de maquina de origen
	double origin_span = origin_machine.volume;
	//cout << "cur" << origin_span << "\n ";
	//Span de maquina objetivo
	double target_span = target_machine.volume;
	//cout << "target" << target_span << "\n ";
	//Maximo local
	double local_max_span;
	//Nuevo maximo local
	double new_local_max_span;
	//Compara el span de la maquina original con la objetivo si es menor se remplaza el valor del maximo local. (Target_span)
	if (origin_span < target_span) {
		local_max_span = target_span;
	}
	else {
		//Remplaza valor del maximo local (origin_span)
		local_max_span = origin_span;
	}
	//cout << "local_max_span" << local_max_span << " \n";
	//Se obtiene Span del trabajo a mover
	double job_span = job_to_move->weight;
	//cout << "machine.id" << target_machine.id <<"job" << job_span << " " << job_to_move->id <<"\n";


	//Se obtiene el nuevo maximo local
	if (origin_span - job_span < target_span + job_span) {
		new_local_max_span = target_span + job_span;
	}
	else {
		new_local_max_span = origin_span - job_span;
	}
	//cout << "new_local" << new_local_max_span << "\n";
	//Si el span actual es igual al span de la maquina objetivo no representa mejoria
	if (cur_span == target_span) {
		return false;
	}
	else if (cur_span > target_span + job_span) {
		//Si el nuevo maximo local es menor; es factible mover el trabajo, en caso contrario no representa mejoria.
		if (new_local_max_span < local_max_span) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}

}
//Mueve un trabajo a otra maquina
bool moveJob(Group& origin_machine, Group& target_machine, Item* job_to_move,int position) {
	//cout << job_to_move->id <<job_to_move->weights[origin_machine.id]<< "\n";
	//cout << origin_machine.id << " " << origin_machine.volume << "\n";
	//cout << target_machine.id << " " << target_machine.volume << "\n";
	removeItemFromGroup(origin_machine, position);
	bool added = addItemGrouptoGroup(target_machine, *job_to_move, origin_machine);
	//cout << job_to_move->id << job_to_move->weights[origin_machine.id] << "\n";
	//cout << origin_machine.volume << "\n";
	//cout << target_machine.volume << "\n";
	return true;
}
bool addItemGrouptoGroup(Group& group, Item& item, Group& group2) {
	//cout << "probar" << item.weights[group2.id] << "\n";
	  // Verificar si el objeto ya est  en el grupo.
	for (int i = 0; i < group.totalItems; i++) {
		Item* items = group.items[i];
		if (items[i].id == item.id) return false;
	}
	// Aumentar el volumen del grupo y agregar el objeto.
	group.volume += item.weight;
	group.items[group.totalItems++] = &item;
	return true;
}
//Identifica si la lista de maquinas se trabajo completa
bool isDone(vector<bool> d_list) {
	bool checks = all_of(d_list.begin(), d_list.end(), [](bool elemento) {
		return elemento;
		});
	return checks;
}
//Rutina para mover un trabajo a otra maquina
void oneJobRoutine(Chromosome& chromosome) {
	bool done = false;
	bool move_or_not_to_move = false;
	bool moved = false;
	int iter = 0;
	//Numeros de maquinas
	int maquinas = chromosome.problem->numGroups;
	//Itera hasta que todas las maquinas se hayan procesado
	//cout << "maquinas" << chromosome.groups.size() << " ";
	while (!done) {
		//Se obtiene makespan actual
		double prev_makespan = finalMakeSpan(chromosome);
		//cout << "after" << prev_makespan << "\n";
		iter = iter + 1;
		//Se crea vector booleano que se usara para identificar que cada una de las maquinas se haya trabajado.
		vector<bool> done_list(maquinas);
		for (int i = 0; i < maquinas; i++)
		{
			//cout << "it:" << i << " ";
			for (int j = 0; j < chromosome.groups[i]->totalItems; j++)
			{
				//cout << "it:" << j << " ";
				for (int k = 1; k < maquinas; k++)
				{
					//cout << "it:" << i << " ";
					//Checa si representa una mejoria mover el trabajo a otra maquina
					move_or_not_to_move = checkMoveSpan(chromosome, *chromosome.groups[i], *chromosome.groups[(i + k) % maquinas], chromosome.groups[i]->items[j]);
					//cout << move_or_not_to_move << " \n";

					//printChromosomeAsJson(chromosome, true);
					if (move_or_not_to_move == true) {
						//Mueve el trabajo a otra maquina
						moved = moveJob(*chromosome.groups[i], *chromosome.groups[(i + k) % maquinas], chromosome.groups[i]->items[j],j);
						//cout << "ott:" << chromosome.groups[i].id << "\n ";
						//printChromosomeAsJson(chromosome, true);
						/*Group target = chromosome.groups[(i + 1) % maquinas];
							for (int i = 0; i < target.items.size(); i++) {
								cout << "item" << target.items[i]->id << " ";
							}
						cout << "FINAL" << " "; */
						//Actualiza vector booleano 
						if (moved == true) {
							//cout << "ott:"<< chromosome.groups[i].id << "\n ";
							if (done_list[chromosome.groups[i]->id] == false) {
								done_list[chromosome.groups[i]->id] = true;

							}
						}
						break;
					}
				}
			}
			//Si el anterior makespan es mayor que el actual se actualiza el valor
			if (prev_makespan > finalMakeSpan(chromosome)) {
				prev_makespan = finalMakeSpan(chromosome);
			}
			//Si todas las maquinas fueron procesadas se termina el proceso.
			if (isDone(done_list) || iter >= 4) {
				done = true;
				//cout << prev_makespan << " ";
				//calculateFitness(chromosome);
				break;
			}
		}

	}
}
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
	for (int i = 0; i < group.totalItems;i++) {
	Item* items = group.items[i];
	if (items[i].id == item.id) return false;
	}
	//agregar el objeto.
	group.items[group.totalItems++] = &item;
	return true;
}

void refactorGroupVolume_bestk(Group& group) {
	group.volume = 0;
	for (int i = 0;i<group.totalItems;i++) {
		group.volume += group.items[i]->weight;
	}
}
void reorder_chromosome(Chromosome& chromosome, int groups_chromo) {
	//cout << groups_chromo;
	for (int i = 0; i < groups_chromo; i++) {
		int indx = chromosome.groups[i]->totalItems;
		//cout << "indx" << chromosome.groups[i]->totalItems << " ";
		for (int y = 0; y < indx; y++) {
			Item* item = *chromosome.groups[i]->items;
			//unsigned long int *weights= item->weights;
			//cout << sizeof(weights) << "of";
			for (int z = 0; z < chromosome.totalGroups; z++) {
				//cout << " " << item->weights[z] << "f ";
				//cout << item->weight << "menor";
				if (item->weight == item->weights[z]) {
					bool added = aux_add_item(*chromosome.groups[z], *item);
					if (added == true) {
						removeItemFromGroup(*chromosome.groups[i], i);
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

	groups_chromo = chromosome.totalGroups;
	reorder_chromosome(chromosome,groups_chromo);
	oneJobRoutine(chromosome);
	printChromosomeAsJson(chromosome, true);
	oneByOneSwapRoutine(chromosome);
	//twoByTwoSwapRoutine(chromosome);
}