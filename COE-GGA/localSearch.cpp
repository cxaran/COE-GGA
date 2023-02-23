#include "../heuristics.h"

void addJob(Group machine, Item item) {
	
}


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



