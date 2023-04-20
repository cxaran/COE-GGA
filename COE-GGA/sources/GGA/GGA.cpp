#include "../../include/GGA/GGA.h"

// Funcion para ejecutar el algoritmo gen�tico.
void GGA(ConfigurationGGA* config, Species& specie, Population& children, int iteration) {

    // Inicializacion de variables
    int i, j;
    long int parent1Index1, parent1Index2, h, k;
    config->bestIndividuals.reSize(specie.population.size);
    config->randomIndividuals.reSize(specie.population.size);

    // Ordena los individuos de la poblaci�n en orden ascendente de su fitness
    sortChromosomesByFitness(specie.population);
   
    //------------- Controlled selection for crossover      -------------------------------------------------------------------

    // Seleccionamos los peores individuos como padres promedio para la cruza
    config->randomIndividuals.randomize(&config->seed, 0, (int)(specie.population.size - (int)(specie.population.size * config->elitism)));
    // Seleccionamos los mejores individuos como padres �lite para la cruza
    config->bestIndividuals.randomize(&config->seed, (1 - config->crossover) * specie.population.size, specie.population.size);

    //Inicializaci�n de variables
    k = 0;
    h = specie.population.size - 1;

    for (i = specie.population.size - 1, j = 0; i > specie.population.size - (config->crossover / 2 * specie.population.size); i--, j += 2) {
        // Se guardan los indices de los padres a cruzar
        parent1Index1 = config->bestIndividuals[h--];
        parent1Index2 = config->randomIndividuals[k++];
        // Se verfica que los indices no sean iguales
        if (parent1Index1 == parent1Index2) parent1Index1 = config->bestIndividuals[h--];

        // Cruza controlada entre los cromosomas parent1Index1 y parent1Index2 para crear los hijos j
        controlledCrossover(children[j], specie.population[parent1Index1], specie.population[parent1Index2]);
        // Establece la iteraci�n en que se genero el hijo j
        children[j].iteration = iteration;
        // Divide la aptitud total del hijo j entre el n�mero total de grupos del hijo j
        children[j].fitness /= (double)children[j].totalGroups;

        // Cruza controlada entre los cromosomas parent1Index2 y parent1Index1 para crear los hijos j+1
        controlledCrossover(children[j + 1], specie.population[parent1Index2], specie.population[parent1Index1]);
        // Establece la iteraci�n en que se genero el hijo j+1
        children[j + 1].iteration = iteration;
        // Divide la aptitud total del hijo j+1 entre el n�mero total de grupos del hijo j+1
        children[j + 1].fitness /= (double)children[j + 1].totalGroups;

    }

    //------------- Controlled replacement for crossover    -------------------------------------------------------------------

    //Inicializaci�n de variables
    k = 0;
    // Remplaza randomIndividuals con los primeros hijos generados 
    for (j = 0; j < config->crossover / 2 * specie.population.size - 1; j++) {
        replaceChromosomeInPopulation(specie.population, children[j], config->randomIndividuals[k++]);
    }
    k = 0;
    // Los hijos restantes entran a la poblaci�n remplazando las peores soluciones
    for (i = specie.population.size - 1; i > specie.population.size - (config->crossover / 2 * specie.population.size); i--, j++) {
        while (specie.population[k].iteration == iteration) k++;
        replaceChromosomeInPopulation(specie.population, children[j], k++);
    }

    //------------- Controlled selection for mutation       -------------------------------------------------------------------

    // Ordenamiento de las soluciones
    sortChromosomesByFitness(specie.population);

    // El individuo a ser remplazado por las soluciones clonadas
    j = 0;
    // Se recorre la poblaci�n empezando por el mejor individuo hasta un n�mero definido de mutaciones
    for (i = specie.population.size - 1; i > specie.population.size - (config->mutation * specie.population.size); i--) {

        // Si el cromosoma a mutar pertenece a los mejores individuos y puede ser clonado
        if (i != j && j < (int)(specie.population.size * config->elitism) && iteration - specie.population[i].iteration < config->lifeSpan) {

            // Se clona remplazando a las peores soluciones
            replaceChromosomeInPopulation(specie.population, specie.population[i], j);

            // Se le aplica una mutaci�n adaptativa a j con una tasa de mutaci�n eliteRateK
            adaptiveMutation(specie.population[j], config->eliteRateK, &config->mutationSeed, true);
            // Establece la iteraci�n en que se genero individuo mutado j
            specie.population[j].iteration = iteration;
            // Divide la aptitud total del individuo mutado j entre el n�mero total de grupos
            specie.population[j].fitness /= specie.population[j].totalGroups;
            // Siguiente individuo a ser remplazado
            j++;

        }

        // Si el cromosoma a mutar no pertenece a los mejores individuos
        else {

            // Se le aplica una mutaci�n adaptativa a i con una tasa de mutaci�n averageRateK
            adaptiveMutation(specie.population[i], config->averageRateK, &config->mutationSeed, false);
            // Establece la iteraci�n en que se genero individuo mutado j
            specie.population[i].iteration = iteration;
            // Divide la aptitud total del individuo mutado i entre el n�mero total de grupos
            specie.population[i].fitness /= specie.population[i].totalGroups;
        }
    }
}
