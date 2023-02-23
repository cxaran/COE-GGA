#include "../heuristics.h"

double finalMakeSpan(Chromosome chromosome) {
	double max = 0;
	for (int i = 0; i < chromosome.groups.size(); i++)
	{
		if (chromosome.groups[i].volume> max) {
			max = chromosome.groups[i].volume;
		}
	}
	return max;
}

bool checkMoveSpan(Chromosome chromo, Group origin_machine, Group target_machine, Item job_to_move) {
	double cur_span = finalMakeSpan(chromo);
	double origin_span = origin_machine.volume;
	double target_span = target_machine.volume;
	double local_max_span;
	double new_local_max_span;
	if (origin_span < target_span){
		local_max_span = target_span;
	}
	else {
		local_max_span = origin_span;
	}
	double job_span = job_to_move.weights[origin_machine.id];

	if (origin_span - job_span < target_span+ job_span) {
		new_local_max_span = target_span + job_span;
	}
	else {
		new_local_max_span = origin_span - job_span;
	}

	if (cur_span == target_span) {
		return false;
	}
	else if (cur_span>target_span+job_span) {
		if (new_local_max_span<local_max_span) {
			return true;
		}
		else{
			return false;
		}
	}
	else {
		return false;
	}
	
	cout << job_span;
}

bool moveJob(Group origin_machine, Group target_machine, Item job_to_move) {
	origin_machine.

}





void oneJobRoutine(Chromosome chromosome) {
	double prev_makespan = finalMakeSpan(chromosome);

	for (int i = 0; i < chromosome.groups.size(); i++)
	{
		for (int j = 0; j < chromosome.groups[i].items.size(); j++)
		{
			for (int k = 0; k < chromosome.groups.size(); k++)
			{

			}
		}
	}
}