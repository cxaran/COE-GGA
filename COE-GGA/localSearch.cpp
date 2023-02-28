#include "include/heuristics.h"
#include "localsearch.h"
using namespace std;

//Determina el maximo makespan actual
double finalMakeSpann(Chromosome chromosome) {
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
bool checkMoveSpan(Chromosome chromo, Group origin_machine, Group target_machine, Item* job_to_move) {
	//Se obtiene makespan actual
	double cur_span = finalMakeSpann(chromo);
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
	//Se obtiene Span del trabajo a mover
	double job_span = job_to_move->weights[origin_machine.id];

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

	cout << job_span;
}

//Mueve un trabajo a otra maquina
bool moveJob(Group origin_machine, Group target_machine, Item* job_to_move) {
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
	bool all_false = all_of(d_list.begin(), d_list.end(), [](bool i) {return i == true; });
	return all_false;
}

//Rutina para mover un trabajo a otra maquina
void oneJobRoutine(Chromosome chromosome) {
	bool done = false;
	bool move_or_not_to_move = false;
	bool moved = false;
	//Numeros de maquinas
	int num_m = chromosome.groups.size();
	//Itera hasta que todas las maquinas se hayan procesado
	while (!done) {
		//Se obtiene makespan actual
		double prev_makespan = finalMakeSpann(chromosome);
		//Se crea vector booleano que se usara para identificar que cada una de las maquinas se haya trabajado.
		vector<bool> done_list(chromosome.groups.size(), false);

		for (int i = 0; i < chromosome.groups.size(); i++)
		{
			for (int j = 0; j < chromosome.groups[i].items.size(); j++)
			{
				for (int k = 1; k < chromosome.groups.size(); k++)
				{
					//Checa si representa una mejoria mover el trabajo a otra maquina
					bool move_or_not_to_move = checkMoveSpan(chromosome, chromosome.groups[i], chromosome.groups[(i + 1) % num_m], chromosome.groups[i].items[j]);
					if (move_or_not_to_move == true) {
						//Mueve el trabajo a otra maquina
						bool moved = moveJob(chromosome.groups[i], chromosome.groups[(i + 1) % num_m], chromosome.groups[i].items[j]);
					}
					//Actualiza vector booleano 
					if (moved == true) {
						if (done_list[chromosome.groups[i].id] == false) {
							done_list[chromosome.groups[i].id] = true;
						}
					}
					break;
				}
			}
			//Si el anterior makespan es mayor que el actual se actualiza el valor
			if (prev_makespan > finalMakeSpann(chromosome)) {
				prev_makespan = finalMakeSpann(chromosome);
			}
			//Si todas las maquinas fueron procesadas se termina el proceso.
			if (isDone(done_list)) {
				done = true;
				break;
			}
		}
	}
}

void main_localSearch(Chromosome chromosome) {
	oneJobRoutine(chromosome);

}