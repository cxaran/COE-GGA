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
	bool deleted= DeleteItemFromGroup(origin_machine,*job_to_move, position);
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
		if (group.items[i]->id == item.id) return false;
	}
	// Aumentar el volumen del grupo y agregar el objeto.
	group.volume += item.weight;
	group.items[group.totalItems++] = &item;
	//group.totalItems++;
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
	if (chromosome.groups[chromosome.totalGroups]) {
		if (groups_chromo < chromosome.problem->numGroups) {
			group = chromosome.groups[chromosome.totalGroups];
			clearGroup(*group);
		}
		else {
			//cout << groups_chromo << endl;
			exit(3);
			return false;
		}
	}
	else {
		//cout << groups_chromo << endl;
		group = new Group();
		chromosome.groups[chromosome.totalGroups] = group;
		group->id = chromosome.totalGroups;
		chromosome.totalGroups++;
		//cout << chromosome.groups.size() << endl;
		return true;
	}
}

bool aux_add_item(Group& group, Item& item) {
	cout << "total_before" << group.totalItems << " ";
	//cout << "probar" << item.weights[group2.id] << "\n";
	 // Verificar si el objeto ya est  en el grupo.
	for (int i = 0; i < group.totalItems;i++) {
	Item* items = group.items[i];
	if (items[i].id == item.id) return false;
	}
	//agregar el objeto.
	//group.totalItems++;
	group.items[group.totalItems++] = &item;
	cout <<"total_after"<< group.totalItems << " ";
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
						DeleteItemFromGroup(*chromosome.groups[i],*item, y);
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
}bool DeleteItemFromGroup(Group& group, Item& item, int itemPos) {
	// Encontrar objeto dentro del 
	//cout << "weight" << item.weights[group.id] << "\n";
	group.totalItems--;
	group.items[itemPos] = group.items[group.totalItems];
	// Disminuir el volumen del grupo
	group.volume -= item.weight;
	return true;
}
bool swapJobs(Group& origin_machine, Group& target_machine, Item* origin_job, Item* target_job,int position_or,int position_targ) {
	//Before swap
	cout << "origin_machine_volume" << origin_machine.volume << "\n";
	cout << "target_machine_volume" << target_machine.volume << "\n";
	cout << "origin_job" << origin_job->weight << "\n";
	cout << "target_job" << target_job->weight << "\n";

	bool deleted=DeleteItemFromGroup(origin_machine,*origin_job,position_or);
	bool added = addItemGrouptoGroup(target_machine, *origin_job, origin_machine);

	cout << "second_E" << "\n";
	cout << "origin_machine_volume" << origin_machine.volume << "\n";
	cout << "target_machine_volume" << target_machine.volume << "\n";
	//after swap
	DeleteItemFromGroup(target_machine,*target_job,position_targ);
	bool added2 = addItemGrouptoGroup(origin_machine, *target_job, target_machine);

	//Before swap
	cout << "origin_machine_volume" << origin_machine.volume << "\n";
	cout << "target_machine_volume" << target_machine.volume << "\n";
	cout << "origin_job" << origin_job->weight << "\n";
	cout << "target_job" << target_job->weight << "\n";

	//posible error direccion de memoria -- temp
	return true;
}
bool checkSwapSpan(Chromosome& chromosome, Group& machine, Group& target_machine, Item* origin_job, Item* target_job) {
	double cur_span = finalMakeSpan(chromosome);
	//cout << "after_check" << cur_span << "\n";
	//Span de maquina de origen
	double origin_span = machine.volume;
	//cout << "origin_span" << origin_span << "\n";
	//Span de maquina objetivo
	double target_span = target_machine.volume;
	//cout << "target_span" << target_span << "\n";
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
	//cout << "local_max_span" << local_max_span << "\n";
	//Se obtiene Span del trabajo a mover
	double origin_job_span = origin_job->weight;
	//cout << "origin_job_span" << origin_job_span << "\n";
	//Se obtiene Span del trabajo de la maquina objetivo
	double target_job_span = target_job->weight;
	//cout << "target_job_Span" << target_job_span << "\n";
	//Se obtiene el nuevo maximo local
	if ((origin_span - origin_job_span) + target_job_span < (target_span - target_job_span) + origin_job_span) {
		new_local_max_span = (target_span - target_job_span) + origin_job_span;
	}
	else {
		new_local_max_span = (origin_span - origin_job_span) + target_job_span;
	}
	//cout << "new_local_max_span" << new_local_max_span << "\n";
	//Si el span actual es igual al span de la maquina objetivo no representa mejoria
	if (new_local_max_span < cur_span) {
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
void oneByOneSwapRoutine(Chromosome& chromosome) {
	bool done = false;
	bool move_or_not_to_move = false;
	bool moved = false;
	int iter = 0;
	//Numeros de maquinas
	int maquinas = chromosome.problem->numGroups;
	while (!done) {
		//Se obtiene makespan actual
		double prev_makespan = finalMakeSpan(chromosome);
		int no_swap_count = chromosome.problem->numItems;
		//cout << "before_move_makespan" << prev_makespan<< "\n";
		//Se crea vector booleano que se usara para identificar que cada una de las maquinas se haya trabajado.
		vector<bool> done_list(maquinas);
		for (int i = 0; i < maquinas; i++) {
			int indx = chromosome.groups[i]->totalItems;
			for (int y = 0; y < indx; y++) {
				Item* item = *chromosome.groups[i]->items;
				bool move_at_least_once = false;
				bool break_flag = false;
				for (int k = 1; k < maquinas; k++) {
					Group target_machine = *chromosome.groups[(i + k) % maquinas];
					for (int z = 0; z < target_machine.totalItems; z++) {
						moved = false;
						move_or_not_to_move = checkSwapSpan(chromosome, *chromosome.groups[i], *chromosome.groups[(i + k) % maquinas], item, chromosome.groups[(i + k) % maquinas]->items[z]);
						//cout << "boolean" << move_or_not_to_move << "\n";
						if (move_or_not_to_move == true) {
							moved = swapJobs(*chromosome.groups[i], *chromosome.groups[(i + k) % maquinas], item, chromosome.groups[(i + k) % maquinas]->items[z],y,z);
							move_at_least_once = true;
							if (moved == true) {
								//cout << i+k << " ";
								break_flag = true;
								break;
							}
						}
					}
					if (break_flag == true) {
						break;
					}
				}
				if (move_at_least_once == false) {
					no_swap_count = no_swap_count - 1;
				}

			}
			if (prev_makespan > finalMakeSpan(chromosome)) {
				prev_makespan = finalMakeSpan(chromosome);
			}
		}
		if (no_swap_count == 0) {
			done = true;
			//cout << "hola";
			//calculateFitness(chromosome);
			break;
		}
	}
}
vector<vector<Item*>> uniquePairs(Item* items, int maquinas,int totalItems) {
	vector<vector<Item*>>result;
	if (totalItems < 2) {
		return result; // Devolver un vector vacío si no hay suficientes elementos
	}
	for (int i = 0; i < totalItems; i++) {
		for (int j = i + 1; j < totalItems; j++) {
			result.push_back({ &items[i],&items[i] });
			//cout << " " << result[i][j];
		}
	}
	return result;
}
bool checkTwoSwapSpan(Chromosome& chromosome, Group& origin_machine, Group& target_machine, vector<Item*>pair1, vector<Item*>pair2) {
	int first_indx = 0;
	int second_indx = 0;
	int third_indx = 0;
	int fourth_indx = 0;
	for (int i = 0; i < origin_machine.totalItems; i++) {
		if (origin_machine.items[i]->id == pair1[0]->id) {
			first_indx = i;
		}
		if (origin_machine.items[i]->id == pair1[1]->id) {
			second_indx = i;
		}
	}
	for (int j = 0; j < target_machine.totalItems; j++) {
		if (target_machine.items[j]->id == pair2[0]->id) {
			third_indx = j;
		}
		if (target_machine.items[j]->id == pair2[1]->id) {
			fourth_indx = j;
		}
	}
	Item* first = origin_machine.items[first_indx];
	Item* second = origin_machine.items[second_indx];
	Item* third = target_machine.items[third_indx];
	Item* fourth = target_machine.items[fourth_indx];

	double cur_span = finalMakeSpan(chromosome);
	//cout << cur_span << "\n";
	//Span de maquina de origen
	double origin_span = origin_machine.volume;
	//cout << "origin" << origin_span <<"\n";
	//Span de maquina objetivo
	double target_span = target_machine.volume;
	//cout << "target" << target_span << "\n";
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
	//cout << "local" << local_max_span << "\n";
	//cout << "job" << pair1[0]->id << "\n";
	//cout << "firs" << first->min << "second" << second->min << "third" << third->min << "fourth" << fourth->min << "\n";
	if (origin_span - (first->weight) - (second->weight) + (third->weight) + (fourth->weight) <
		target_span - (third->weight) - (fourth->weight) + (first->weight) + (second->weight)) {

		new_local_max_span = target_span - (third->weight) - (fourth->weight) + (first->weight) + (second->weight);
	}
	else {
		new_local_max_span = origin_span - (first->weight) - (second->weight) + (third->weight) + (fourth->weight);

	}
	//cout << "new_local" << new_local_max_span << "\n";

	if (new_local_max_span < cur_span) {
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
bool swapTwoJobs(Group& origin_machine, Group& target_machine, vector<Item*>pair1, vector<Item*>pair2,int indx1,int indx2) {
	/// posible error de indices en pares verificar
	bool first_move = swapJobs(origin_machine, target_machine, pair1[0], pair2[0],indx1,indx2);
	bool second_move = swapJobs(origin_machine, target_machine, pair1[1], pair2[1],indx1,indx2);

	if (first_move && second_move) {
		return true;
	}
	else {
		return false;
	}

}
vector<vector<Item*>> rand_pair(Item* items, int num_par,int totalItems) {
	int* arr = new int[totalItems];
	// Inicializar el arreglo con valores de 0 hasta n-1
	for (int i = 0; i < totalItems; ++i) {
		arr[i] = i;
	}
	// Crear un generador de números aleatorios
	random_device rd;
	mt19937 gen(rd());
	//vector de pares de salida
	vector<vector<Item*>>result;
	const int size_n = sizeof(arr) / sizeof(arr[0]);
	shuffle(arr,arr+size_n , gen);
	int n = totalItems;
	for (int i = 0; i < n / 2; i++) {
		result.push_back({ &items[arr[i]],&items[n - 1 - arr[i]]});
		num_par = num_par - 1;
		if (num_par <= 0) {
			break;
		}
	}
	return result;

}
bool twoRoutineHelper(Chromosome& chromosome, Group& machine, int num_trabajos, int maquinas) {
	vector<vector<Item*>>origin_pairs;
	vector<vector<Item*>>target_pairs;
	bool move_or_not_to_move = false;
	bool moved = false;
	/*for (int x = 0; x < machine.items.size(); x++) {
		cout << machine.items[x]->id << " ";
	}*/
	if (num_trabajos <= 400) {
		origin_pairs = uniquePairs(*machine.items, maquinas,machine.totalItems);
		/*for (int i = 0; i < origin_pairs.size(); i++) {
			for (int j = 0; j < origin_pairs[i].size(); j++) {
				cout << origin_pairs[i][j]->id << " ";
			}
			cout << endl;
		}*/
		int cont = 0;
		for (auto const& pair1 : origin_pairs) {
			//cout << pair1[0]->id <<"p" <<pair1[1]->id << " ";

			for (int i = 1; i < maquinas; i++) {

				Group target_machine = *chromosome.groups[(machine.id + i) % maquinas];
				target_pairs = uniquePairs(*chromosome.groups[(machine.id + i) % maquinas]->items, maquinas,machine.totalItems);
				int cont2 = 0;
				for (auto const& pair2 : target_pairs) {
					move_or_not_to_move = checkTwoSwapSpan(chromosome, machine, *chromosome.groups[(machine.id + i) % maquinas], pair1, pair2);
					//cout << move_or_not_to_move;
					if (move_or_not_to_move == true) {
						//cout << move_or_not_to_move << " ";
						moved = swapTwoJobs(machine, *chromosome.groups[(machine.id + i) % maquinas], pair1, pair2,cont,cont2);
						if (moved == true) {
							return true;

						}
					}
					cont2 = cont2 + 1;
				}
			}
			cont = cont + 1;
		}
		//calculateFitness(chromosome);
	}
	else {
		int size_items = machine.totalItems;
		double porcentaje_par = 19;
		//cout << size_items <<"-";
		if (size_items <= 50) {
			porcentaje_par = 19;
		}
		else if (size_items <= 400) {
			porcentaje_par = 200;
		}
		else if (size_items <= 1000) {
			porcentaje_par = 500;
		}
		else { porcentaje_par = 800; }

		double p_par = porcentaje_par + size_items;
		//cout << "s" << p_par << "s";
		double formula = porcentaje_par / p_par;
		//cout << "f" << formula;
		int num_par = size_items * formula;
		//	cout << "num_par"<<num_par << "par";
		origin_pairs = rand_pair(*machine.items, num_par,machine.totalItems);
		int cont = 0;
		for (auto const& pair1 : origin_pairs) {
			for (int i = 1; i < maquinas; i++) {
				Group target_machine = *chromosome.groups[(i + 1) % maquinas];
				int size_items2 = target_machine.totalItems;
				//cout << "i" << i << "size" << size_items2 << "\n";
				formula = porcentaje_par / (porcentaje_par + size_items2);
				num_par = size_items2 * formula;
				if (size_items2 == 0) {
					moved = moveJob(machine, *chromosome.groups[(i + 1) % maquinas], pair1[0],cont);
					return true;
				}
				target_pairs = rand_pair(*chromosome.groups[(i + 1) % maquinas]->items, num_par,size_items2);
				int cont2 = 0;
				for (auto const& pair2 : target_pairs) {
					move_or_not_to_move = checkTwoSwapSpan(chromosome, machine, target_machine, pair1, pair2);
					if (move_or_not_to_move == true) {
						moved = swapTwoJobs(machine, *chromosome.groups[(i + 1) % maquinas], pair1, pair2,cont,cont2);
						if (moved == true) {
							return true;

						}
					}
					cont2 = cont2 + 1;
				}
			}
			cont = cont + 1;
		}
	}
	//calculateFitness(chromosome);
	return false;
}
void twoByTwoSwapRoutine(Chromosome& chromosome) {
	bool done = false;
	int machine_one_counter = 0;
	//Numeros de maquinas
	int maquinas = chromosome.problem->numGroups;
	int num_trabajos = chromosome.problem->numItems;
	bool swapped = false;
	while (!done) {
		//Se obtiene makespan actual
		double prev_makespan = finalMakeSpan(chromosome);
		for (int i = 0; i < maquinas; i++) {
			if (i == 0) {
				machine_one_counter += 1;
			}
			swapped = true;
			while (swapped == true) {
				swapped = twoRoutineHelper(chromosome, *chromosome.groups[i], num_trabajos, maquinas);
			}
			if (prev_makespan > finalMakeSpan(chromosome)) {
				prev_makespan = finalMakeSpan(chromosome);

			}
		}
		if (machine_one_counter == 2) {
			return;
		}

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
	oneByOneSwapRoutine(chromosome);
	printChromosomeAsJson(chromosome, true);

	//twoByTwoSwapRoutine(chromosome);
}