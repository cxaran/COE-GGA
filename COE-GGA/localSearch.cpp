#include "include/heuristics.h"
#include "localsearch.h"
#include <random>
using namespace std;
//Determina el maximo makespan actual
double finalMakeSpan(Chromosome& chromosome) {
	double max = 0;
	//Recorre los grupos-- maquinas
	for (int i = 0; i < chromosome.groups.size(); i++)
	{
		//Compara el makespan actual de la maquina y si es mayor que el maximo, actualiza la variable
		if (chromosome.groups[i].volume > max) {
			max = chromosome.groups[i].volume;
		}
	}
	return max;
}

//Checa si representa una mejora mover el trabajo a otra maquina
bool checkMoveSpan(Chromosome& chromo, Group& origin_machine, Group& target_machine, Item* job_to_move) {
	//Se obtiene makespan actual
	double cur_span = finalMakeSpan(chromo);
	//Span de maquina de origen
	double origin_span = origin_machine.volume;
	//cout << "cur" << origin_span << " ";
	//Span de maquina objetivo
	double target_span = target_machine.volume;
	//cout << "target" << target_span << " ";
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
	//Se obtiene Span del trabajo a mover
	double job_span = job_to_move->weights[origin_machine.id];
	//cout << "job" << target_machine.id<< target_machine.volume<< " ";

	//Se obtiene el nuevo maximo local
	if (origin_span - job_span < target_span + job_span) {
		new_local_max_span = target_span + job_span;
	}
	else {
		new_local_max_span = origin_span - job_span;
	}
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
bool moveJob(Group& origin_machine, Group& target_machine, Item* job_to_move) {
	//cout << job_to_move->id;
	bool item_deleted = DeleteItemToGroup(origin_machine, *job_to_move);
	bool added = addItemToGroup(target_machine, *job_to_move);
	return true;
}
//Se elimina la tarea de la maquina
bool DeleteItemToGroup(Group& group, Item& item) {
	// Encontrar objeto dentro del 
	for (auto it = group.items.begin(); it != group.items.end(); ++it) {
		if ((*it)->id == item.id) {
			group.items.erase(it);
			break;
		}
	}
	// Disminuir el volumen del grupo
	group.volume -= item.weights[group.id];
	return true;
}

//Identifica si la lista de maquinas se trabajo completa
bool isDone(vector<bool> d_list) {
	bool checks = all_of(d_list.begin(), d_list.end(), [](bool elemento) {
		return elemento;
		});
	return checks;
}
// Funcion para crear un nuevo grupo sin item
bool createNewGroupWithoutItem(Chromosome& chromosome) {
	// Verificar si todav�a se pueden crear m�s grupos.
	if (chromosome.groups.size() < chromosome.problem->numGroups) {
		Group group;
		group.id = chromosome.groups.size();
		chromosome.groups.push_back(group);
		//cout << chromosome.groups.size() << endl;
		return true;
	}
	// Salir con un c�digo de error.
	else {
		cout << chromosome.groups.size() << endl;
		exit(3);
		return false;
	}
}
//Rutina para mover un trabajo a otra maquina
void oneJobRoutine(Chromosome& chromosome) {
	bool done = false;
	bool move_or_not_to_move = false;
	bool moved = false;
	//Numeros de maquinas
	int maquinas = chromosome.problem->numGroups;
	//Itera hasta que todas las maquinas se hayan procesado
	//cout << "maquinas" << chromosome.groups.size() << " ";
	while (!done) {
		//Se obtiene makespan actual
		double prev_makespan = finalMakeSpan(chromosome);
		//Se crea vector booleano que se usara para identificar que cada una de las maquinas se haya trabajado.
		vector<bool> done_list(maquinas);
		for (int i = 0; i < maquinas; i++)
		{
			//cout << "it:" << i << " ";
			for (int j = 0; j < chromosome.groups[i].items.size(); j++)
			{
				//cout << "it:" << j << " ";
				for (int k = 1; k < maquinas; k++)
				{
					//cout << "it:" << i << " ";
					//Checa si representa una mejoria mover el trabajo a otra maquina
					move_or_not_to_move = checkMoveSpan(chromosome, chromosome.groups[i], chromosome.groups[(i + k) % maquinas], chromosome.groups[i].items[j]);
					//printChromosomeAsJson(chromosome, true);
					if (move_or_not_to_move == true) {
						//Mueve el trabajo a otra maquina
						moved = moveJob(chromosome.groups[i], chromosome.groups[(i + k) % maquinas], chromosome.groups[i].items[j]);
						//cout << "ott:" << chromosome.groups[i].id << " ";
						/*Group target = chromosome.groups[(i + 1) % maquinas];
							for (int i = 0; i < target.items.size(); i++) {
								cout << "item" << target.items[i]->id << " ";
							}
						cout << "FINAL" << " "; */
						//Actualiza vector booleano 
						if (moved == true) {
							//cout << "ott:"<< chromosome.groups[i].id << " ";
							if (done_list[chromosome.groups[i].id] == false) {
								done_list[chromosome.groups[i].id] = true;
								
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
			if (isDone(done_list)) {
				done = true;
				//cout << prev_makespan << " ";
				calculateFitness(chromosome);
				break;
			}
		}

	}
}

bool checkSwapSpan(Chromosome& chromosome,Group& machine,Group& target_machine,Item* origin_job,Item* target_job) {
	double cur_span = finalMakeSpan(chromosome);
	//cout << "final" << cur_span << " ";
	//Span de maquina de origen
	double origin_span = machine.volume;
	//cout << "origin_span" << origin_span << " ";
	//Span de maquina objetivo
	double target_span = target_machine.volume;
	//cout << "target_span" << target_span << " ";
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
	//cout << "local_max_span" << local_max_span << " ";
	//Se obtiene Span del trabajo a mover
	double origin_job_span = origin_job->weights[machine.id];
	//cout << "origin_job_span" << origin_job_span << " ";
	//Se obtiene Span del trabajo de la maquina objetivo
	double target_job_span = target_job->weights[target_machine.id];
	//cout << "target_job_Span" << target_job_span << " ";
	//Se obtiene el nuevo maximo local
	if ((origin_span - origin_job_span) + target_job_span < (target_span - target_job_span) + origin_job_span) {
		new_local_max_span = (target_span - target_job_span) + origin_job_span;
	}
	else {
		new_local_max_span = (origin_span - origin_job_span) + target_job_span;
	}
	//cout << "new_local_max_span" << new_local_max_span << " ";
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

bool swapJobs(Group& origin_machine,Group& target_machine,Item* origin_job,Item* target_job) {
	bool item_deleted = DeleteItemToGroup(origin_machine, *origin_job);
	bool added = addItemToGroup(target_machine, *origin_job);
	bool item_deleted2 = DeleteItemToGroup(target_machine, *target_job);
	bool added2 = addItemToGroup(origin_machine, *target_job);
	//posible error direccion de memoria -- temp
	return true;
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
		//Se crea vector booleano que se usara para identificar que cada una de las maquinas se haya trabajado.
		vector<bool> done_list(maquinas);
		for (int i = 0; i < maquinas;i++) {
			for (int j = 0; j < chromosome.groups[i].items.size();j++) {
				bool move_at_least_once = false;
				bool break_flag = false;
				for (int k = 1; k < maquinas;k++) {
					Group target_machine = chromosome.groups[(i + k) % maquinas];
					for (int z = 0; z < target_machine.items.size(); z++) {
						moved = false;
						move_or_not_to_move = checkSwapSpan(chromosome,chromosome.groups[i], chromosome.groups[(i + k) % maquinas], chromosome.groups[i].items[j], chromosome.groups[(i + k) % maquinas].items[z]);
						
						if (move_or_not_to_move == true) {
							iter = iter + 1;
							moved = swapJobs(chromosome.groups[i], chromosome.groups[(i + k) % maquinas], chromosome.groups[i].items[j], chromosome.groups[(i + k) % maquinas].items[z]);
							move_at_least_once = true;
							if (moved==true) {
						        //cout << i+k << " ";
								break_flag = true;
								break;
							}
						}
					}
					if (break_flag==true) {
						break;
					}
				}
				if (move_at_least_once==false) {
					no_swap_count = no_swap_count - 1;
				}
				
			}
			if (prev_makespan > finalMakeSpan(chromosome)) {
				prev_makespan = finalMakeSpan(chromosome);
			}
		}
		if (no_swap_count==0 ) {
			done = true;
			cout << "hola";
			calculateFitness(chromosome);
			break;
		}
	}
}

bool checkTwoSwapSpan(Chromosome chromosome,Group origin_machine,Group target_machine,vector<Item*>pair1,vector<Item*>pair2) {
	Item* first = origin_machine.items[pair1[0]->id];
	Item* second = origin_machine.items[pair1[1]->id];
	Item* third = target_machine.items[pair2[0]->id];
	Item* fourth = target_machine.items[pair2[1]->id];

	double cur_span = finalMakeSpan(chromosome);
	//Span de maquina de origen
	double origin_span = origin_machine.volume;
	//Span de maquina objetivo
	double target_span = target_machine.volume;
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
	if (origin_span-first->weights[origin_machine.id]-second->weights[origin_machine.id]+third->weights[target_machine.id]+fourth->weights[target_machine.id] <
		target_span-third->weights[target_machine.id]-fourth->weights[target_machine.id]+first->weights[origin_machine.id]+second->weights[origin_machine.id]) {

		new_local_max_span = target_span - third->weights[target_machine.id] - fourth->weights[target_machine.id] + first->weights[origin_machine.id] + second->weights[origin_machine.id];
	}
	else {
		new_local_max_span = origin_span - first->weights[origin_machine.id] - second->weights[origin_machine.id] + third->weights[target_machine.id] + fourth->weights[target_machine.id];

	}
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

bool swapTwoJobs(Group origin_machine,Group target_machine, vector<Item*>pair1, vector<Item*>pair2) {
	bool first_move = swapJobs(origin_machine,target_machine,pair1[0],pair2[0]);
	bool second_move = swapJobs(origin_machine,target_machine,pair1[1],pair2[1]);

	if (first_move && second_move) {
		return true;
	}
	else {
		return false;
	}

}
bool twoRoutineHelper(Chromosome chromosome,Group machine,int num_trabajos,int maquinas) {
	vector<vector<Item*>>origin_pairs;
	vector<vector<Item*>>target_pairs;
	bool move_or_not_to_move = false;
	bool moved = false;
	if (num_trabajos<400) {
		origin_pairs = uniquePairs(machine.items,maquinas);
	
		for (auto const& pair1 : origin_pairs) {
			for (int i = 1; i < maquinas ; i++) {
				Group target_machine = chromosome.groups[(i + 1) % maquinas];
				target_pairs = uniquePairs(target_machine.items,maquinas);

				for (auto const& pair2 : target_pairs) {
					move_or_not_to_move = checkTwoSwapSpan(chromosome,machine,target_machine,pair1,pair2);
					if (move_or_not_to_move==true) {
						moved = swapTwoJobs(machine,target_machine,pair1,pair2);
						if (moved==true) {
							return true;

						}
				}
				}
			}
		}
	}
	else {
		int size_items = machine.items.size();
		const int porcentaje_par = 19.0;
		int formula = porcentaje_par / (porcentaje_par + size_items);
		int num_par = size_items * formula;
		origin_pairs = rand_pair(machine.items,num_par);
		for (auto const& pair1 : origin_pairs) {
			for (int i = 1; i < maquinas; i++) {
				Group target_machine = chromosome.groups[(i + 1) % maquinas];
				int size_items2= target_machine.items.size();
				int formula = porcentaje_par / (porcentaje_par + size_items2);
				int num_par = size_items2 * formula;
				target_pairs = rand_pair(machine.items, num_par);
				for (auto const& pair2 : target_pairs) {
					move_or_not_to_move = checkTwoSwapSpan(chromosome, machine, target_machine, pair1, pair2);
					if (move_or_not_to_move == true) {
						moved = swapTwoJobs(machine, target_machine, pair1, pair2);
						if (moved == true) {
							return true;

						}
					}
				}
			}
		}
	}
	return false;
}

vector<vector<Item*>> rand_pair(vector<Item*> source,int num_par) {
	// Crear un generador de números aleatorios
	random_device rd;
	mt19937 g(rd());
	//vector de pares de salida
	vector<vector<Item*>>result;
	shuffle(source.begin(), source.end(),g);
	int n = source.size();
	for (int i = 0; i < n/2 ;i++) {
		result.push_back({ source[i],source[n-1-i]});
		num_par = num_par - 1;
		if (num_par<=0) {
			break;
		}
	}
	return result;

}
vector<vector<Item*>> uniquePairs(vector<Item*> source,int maquinas) {
vector<vector<Item*>>result;
for (int i = 0; i <source.size() ;i++) {
	for (int j = i+1; j < source.size() ; j++) {
		result.push_back({source[i],source[j]});
	}
}
return result;
}

void twoByTwoSwapRoutine(Chromosome chromosome) {
	bool done = false;
	int machine_one_counter = 0;
	//Numeros de maquinas
	int maquinas = chromosome.problem->numGroups;
	int num_trabajos = chromosome.problem->numItems;
	bool swapped = false;
	while (!done) {
		//Se obtiene makespan actual
		double prev_makespan = finalMakeSpan(chromosome);
		for (int i = 0; i <maquinas ;i++) {
			if (i==0) {
				machine_one_counter += 1;
				swapped = true;
		}
			while (swapped==true) {
				swapped = twoRoutineHelper(chromosome,chromosome.groups[i],num_trabajos,maquinas);
		}
			if (prev_makespan > finalMakeSpan(chromosome)) {
				prev_makespan = finalMakeSpan(chromosome);

			}
		}
		if (machine_one_counter==2) {
			return;
		}

	}
}

void main_localSearch(Chromosome& chromosome) {
	//Numeros de maquinas
	int maquinas = chromosome.problem->numGroups;
	//Itera hasta que todas las maquinas se hayan procesado
	int groups_chromo = chromosome.groups.size();
	if (groups_chromo != maquinas) {
		for (int i = groups_chromo; i < maquinas; i++) {
			bool adde = createNewGroupWithoutItem(chromosome);
		}
	}
	oneJobRoutine(chromosome);
	oneByOneSwapRoutine(chromosome);
	//twoByTwoSwapRoutine(chromosome);
}