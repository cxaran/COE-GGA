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
	double job_span = job_to_move->min;
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
bool moveJob(Group& origin_machine, Group& target_machine, Item* job_to_move) {
	//cout << job_to_move->id <<job_to_move->weights[origin_machine.id]<< "\n";
	//cout << origin_machine.id << " " << origin_machine.volume << "\n";
	//cout << target_machine.id << " " << target_machine.volume << "\n";
	bool item_deleted = DeleteItemToGroup(origin_machine, *job_to_move);
	bool added = addItemGrouptoGroup(target_machine, *job_to_move,origin_machine);
	//cout << job_to_move->id << job_to_move->weights[origin_machine.id] << "\n";
	//cout << origin_machine.volume << "\n";
	//cout << target_machine.volume << "\n";
	return true;
}
//Se elimina la tarea de la maquina
bool DeleteItemToGroup(Group& group, Item& item) {
	// Encontrar objeto dentro del 
	//cout << "weight" << item.weights[group.id] << "\n";

	for (auto it = group.items.begin(); it != group.items.end(); ++it) {
		if ((*it)->id == item.id) {
			group.items.erase(it);
			break;
		}
	}
	// Disminuir el volumen del grupo
	group.volume -= item.min;
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
			for (int j = 0; j < chromosome.groups[i].items.size(); j++)
			{
				//cout << "it:" << j << " ";
				for (int k = 1; k < maquinas; k++)
				{
					//cout << "it:" << i << " ";
					//Checa si representa una mejoria mover el trabajo a otra maquina
					move_or_not_to_move = checkMoveSpan(chromosome, chromosome.groups[i], chromosome.groups[(i + k) % maquinas], chromosome.groups[i].items[j]);
					//cout << move_or_not_to_move << " \n";
					
					//printChromosomeAsJson(chromosome, true);
					if (move_or_not_to_move == true) {
						//Mueve el trabajo a otra maquina
						moved = moveJob(chromosome.groups[i], chromosome.groups[(i + k) % maquinas], chromosome.groups[i].items[j]);
						//cout << "ott:" << chromosome.groups[i].id << "\n ";
						//printChromosomeAsJson(chromosome, true);
						/*Group target = chromosome.groups[(i + 1) % maquinas];
							for (int i = 0; i < target.items.size(); i++) {
								cout << "item" << target.items[i]->id << " ";
							}
						cout << "FINAL" << " "; */
						//Actualiza vector booleano 
						if (moved == true ) {
							//cout << "ott:"<< chromosome.groups[i].id << "\n ";
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
			if (isDone(done_list)|| iter>=4 ) {
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
	double origin_job_span = origin_job->min;
	//cout << "origin_job_span" << origin_job_span << "\n";
	//Se obtiene Span del trabajo de la maquina objetivo
	double target_job_span = target_job->min;
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

bool swapJobs(Group& origin_machine, Group& target_machine, Item* origin_job, Item* target_job) {
	//Before swap
	//cout << "origin_machine_volume" << origin_machine.volume << "\n";
	//cout << "target_machine_volume" << target_machine.volume << "\n";
	//cout << "origin_job" << origin_job->weights[origin_machine.id] << "\n";
	//cout << "target_job" << target_job->weights[target_machine.id] << "\n";

	bool item_deleted = DeleteItemToGroup(origin_machine, *origin_job);
	bool added = addItemGrouptoGroup(target_machine, *origin_job,origin_machine);

	//cout << "second_E" << "\n";
	//cout << "origin_machine_volume" << origin_machine.volume << "\n";
	//cout << "target_machine_volume" << target_machine.volume << "\n";
	//after swap
	bool item_deleted2 = DeleteItemToGroup(target_machine, *target_job);
	bool added2 = addItemGrouptoGroup(origin_machine, *target_job,target_machine);

	//Before swap
	//cout << "origin_machine_volume" << origin_machine.volume << "\n";
	//cout << "target_machine_volume" << target_machine.volume << "\n";
	//cout << "origin_job" << origin_job->weights[origin_machine.id] << "\n";
	//cout << "target_job" << target_job->weights[target_machine.id] << "\n";

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
		//cout << "before_move_makespan" << prev_makespan<< "\n";
		//Se crea vector booleano que se usara para identificar que cada una de las maquinas se haya trabajado.
		vector<bool> done_list(maquinas);
		for (int i = 0; i < maquinas;i++) {
			for (Item* item : chromosome.groups[i].items) {
				bool move_at_least_once = false;
				bool break_flag = false;
				for (int k = 1; k < maquinas;k++) {
					Group target_machine = chromosome.groups[(i + k) % maquinas];
					for (int z = 0; z < target_machine.items.size(); z++) {
						moved = false;
						move_or_not_to_move = checkSwapSpan(chromosome,chromosome.groups[i], chromosome.groups[(i + k) % maquinas], item, chromosome.groups[(i + k) % maquinas].items[z]);
						//cout << "boolean" << move_or_not_to_move << "\n";
						if (move_or_not_to_move == true) {
							moved = swapJobs(chromosome.groups[i], chromosome.groups[(i + k) % maquinas], item, chromosome.groups[(i + k) % maquinas].items[z]);
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
				if (move_at_least_once==false ) {
					no_swap_count = no_swap_count - 1;
				}
				
			}
			if (prev_makespan > finalMakeSpan(chromosome)) {
				prev_makespan = finalMakeSpan(chromosome);
			}
		}
		if (no_swap_count==0 ) {
			done = true;
			//cout << "hola";
			calculateFitness(chromosome);
			break;
		}
	}
}

bool checkTwoSwapSpan(Chromosome& chromosome,Group& origin_machine,Group& target_machine,vector<Item*>pair1,vector<Item*>pair2) {
	int first_indx=0;
	int second_indx=0;
	int third_indx=0;
	int fourth_indx=0;
	for (int i = 0; i < origin_machine.items.size();i++) {
		if (origin_machine.items[i]->id == pair1[0]->id) {
			first_indx = i;
		}
		if (origin_machine.items[i]->id == pair1[1]->id) {
			second_indx = i;
		}
	}
	for (int j = 0; j < target_machine.items.size();j++) {
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
	if (origin_span-(first->min)-(second->min)+(third->min)+(fourth->min) <
		target_span-(third->min)-(fourth->min)+(first->min)+(second->min)) {

		new_local_max_span = target_span - (third->min) - (fourth->min) + (first->min) + (second->min);
	}
	else {
		new_local_max_span = origin_span - (first->min) - (second->min) + (third->min) + (fourth->min);

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

bool swapTwoJobs(Group& origin_machine,Group& target_machine, vector<Item*>pair1, vector<Item*>pair2) {
	bool first_move = swapJobs(origin_machine,target_machine,pair1[0],pair2[0]);
	bool second_move = swapJobs(origin_machine,target_machine,pair1[1],pair2[1]);

	if (first_move && second_move) {
		return true;
	}
	else {
		return false;
	}

}
bool twoRoutineHelper(Chromosome& chromosome,Group& machine,int num_trabajos,int maquinas) {
	vector<vector<Item*>>origin_pairs;
	vector<vector<Item*>>target_pairs;
	bool move_or_not_to_move = false;
	bool moved = false;
	/*for (int x = 0; x < machine.items.size(); x++) {
		cout << machine.items[x]->id << " ";
	}*/
	if (num_trabajos<=400) {
		origin_pairs = uniquePairs(machine.items, maquinas);
		/*for (int i = 0; i < origin_pairs.size(); i++) {
			for (int j = 0; j < origin_pairs[i].size(); j++) {
				cout << origin_pairs[i][j]->id << " ";
			}
			cout << endl;
		}*/
		for (auto const& pair1 : origin_pairs) {
			//cout << pair1[0]->id <<"p" <<pair1[1]->id << " ";
			
			for (int i = 1; i < maquinas ; i++) {
				
				Group target_machine = chromosome.groups[(machine.id+i) % maquinas];
				target_pairs = uniquePairs(chromosome.groups[(machine.id+i) % maquinas].items,maquinas);
				for (auto const& pair2 : target_pairs) {
					move_or_not_to_move = checkTwoSwapSpan(chromosome,machine, chromosome.groups[(machine.id + i) % maquinas],pair1,pair2);
					//cout << move_or_not_to_move;
					if (move_or_not_to_move==true) {
						//cout << move_or_not_to_move << " ";
						moved = swapTwoJobs(machine, chromosome.groups[(machine.id + i) % maquinas],pair1,pair2);
						if (moved==true) {
							return true;

						}
				}
				}
			}
		}
		calculateFitness(chromosome);
	}
	else {
		int size_items = machine.items.size();
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
		origin_pairs = rand_pair(machine.items,num_par);
		for (auto const& pair1 : origin_pairs) {
			for (int i = 1; i < maquinas; i++) {
				Group target_machine = chromosome.groups[(i + 1) % maquinas];
				int size_items2= target_machine.items.size();
				//cout << "i" << i << "size" << size_items2 << "\n";
				formula = porcentaje_par / (porcentaje_par + size_items2);
				num_par = size_items2 * formula;
				if (size_items2 == 0) {
					moved = moveJob(machine, chromosome.groups[(i + 1) % maquinas], pair1[0]);
					return true;
				}
				target_pairs = rand_pair(chromosome.groups[(i + 1) % maquinas].items, num_par);
				for (auto const& pair2 : target_pairs) {
					move_or_not_to_move = checkTwoSwapSpan(chromosome, machine, target_machine, pair1, pair2);
					if (move_or_not_to_move == true) {
						moved = swapTwoJobs(machine, chromosome.groups[(i + 1) % maquinas], pair1, pair2);
						if (moved == true) {
							return true;

						}
					}
				}
			}
		}
	}
	calculateFitness(chromosome);
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
if (source.size() < 2) {
	return result; // Devolver un vector vacío si no hay suficientes elementos
}
for (int i = 0; i <source.size() ;i++) {
	for (int j = i+1; j < source.size() ; j++) {
		result.push_back({source[i],source[j]});
		//cout << " " << result[i][j];
	}
}
return result;
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
		for (int i = 0; i <maquinas ;i++) {
			if (i==0) {
				machine_one_counter += 1;
			}
			swapped = true;
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

void reorder_chromosome(Chromosome& chromosome) {
	
	for (int i = 0; i < chromosome.groups.size(); i++) {
		for (Item* item : chromosome.groups[i].items) {
			for (int z = 0; z < chromosome.groups.size();z++) {
				if (item->min == item->weights[z]) {
					bool added=aux_add_item(chromosome.groups[z],*item);
					if (added == true) {
						DeleteItemToGroup(chromosome.groups[i], *item);
					}
					break;
				}
			}
			
		}
		chromosome.groups[i].volume = 0;
	}
	for (int i = 0; i < chromosome.groups.size(); i++) {
		refactorGroupVolume_bestk(chromosome.groups[i]);
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
	//printChromosomeAsJson(chromosome, true);
	reorder_chromosome(chromosome);
	oneJobRoutine(chromosome);
	oneByOneSwapRoutine(chromosome);
twoByTwoSwapRoutine(chromosome);
}