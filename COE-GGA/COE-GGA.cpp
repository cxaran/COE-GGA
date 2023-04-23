
#include "include/fileio.h"
#include "include/GGA/GGA.h"
#include "include/qlearning.h"
#include "include/structures.h"
#include "include/heuristics.h"
#include "include/permutation.h"
#include "include/localsearchh.h"

using namespace std;

void coevolution(Instance& instance, Chromosome& solution) {

    // Se carga la configuracion
    ConfigurationCOE* config = &instance.config;

    // Inicializar la población
    Population population;
    initializePopulation(instance, population, config->populationSize, true);

    // Inicializar la poblacion de hijos
    Population children;
    initializePopulation(instance, children, config->populationSize, false);

    // Permutacion aleatoria de la población
    Permutation order(population.size);

    // Determinar el estado actual y la accion A1
    State currentState;
    Action action = A1;

    // Crear un vector para almacenar las especies
    Species species[TOTALSPECIES];

    // Crear 4 instancias de la estructura Species, asignándoles el tamaño 
    int chromosomeIndex = 0;
    for (int i = 0; i < TOTALSPECIES; ++i) {
        Species newSpecie;
        newSpecie.size = 1.0 / TOTALSPECIES;

        // Asignar cromosomas a la especie
        for (int j = 0; j < config->populationSize * newSpecie.size; j++) {
            newSpecie.population.chromosomes[j] = population.chromosomes[chromosomeIndex++];
            newSpecie.population.size++;
        }

        // Calcular el fitness de la especie
        calculateSpeciesFitness(newSpecie);

        // Añadir las especies al vector
        species[i] = newSpecie;

        // Acualizar el estado acual
        if (species[currentState.speciesIndex].fitness > species[i].fitness) {
            currentState.speciesIndex = i;
        }

    }

    // Arreglo de dos ConfigurationGGA con valores por defecto, para la especie 1 y 2
    ConfigurationGGA configsGGA[4];
    // Cambiar el valor de "elitism" de la especie 2
    configsGGA[1].elitism = 0.9;

    // Especie dominante (de mayor fitness) en la iteración actual
    int dominantSpeciesIndex = currentState.speciesIndex;
    // Guarda la especie dominante en cada iteracion
    int sumStates[TOTALSPECIES] = { 0 };

    // Repetir el número especificado de iteraciones
    for (int iteration = 1; iteration <= config->iterations; ++iteration) {
        
        // ************************ Especie 1 ************************
        // Aplicar GGA 
        GGA(&configsGGA[0], species[0],children, iteration);

        // ************************ Especie 2 ************************
        // Aplicar GGA 
        GGA(&configsGGA[1], species[1], children, iteration);

        // ************************ Especie 3 ************************
        // Aplicar GGA 
        GGA(&configsGGA[2], species[2], children, iteration);

        // ************************ Especie 4 ************************
        // Aplicar GGA 
        cout << "local";
        main_localSearch(*species[3].population.chromosomes[0]);
        cout << "termino";

        // Calcular el fitness de cada especie
        for (int i = 0; i < TOTALSPECIES; ++i) {
            calculateSpeciesFitness(species[i]);
        }

        // Calcular la especie dominante
        for (int i = 0; i < TOTALSPECIES; ++i) {
            if (species[i].fitness > species[dominantSpeciesIndex].fitness) {
                dominantSpeciesIndex = i;
            }
        }

        // Aumentar sumStates en la posicion de la especie dominate
        sumStates[dominantSpeciesIndex]++;

        if (iteration % config->learningWindow == 0) {

            // Calcular el estado siguiernte
            State nextState;
            for (int i = 0; i < TOTALSPECIES; ++i) {
                if (sumStates[nextState.speciesIndex] < sumStates[i]) {
                    nextState.speciesIndex = i;
                }
                cout << sumStates[i] << " ";
                // Poner todas las posiciones de sumStates en 0
                sumStates[i] = 0;
            }
            cout << endl;

            // Calcular la recompensa
            float reward = nextState.speciesIndex == currentState.speciesIndex ? rewards[0] : rewards[1];

            // Actualizar la tabla Q
            updateQTable(currentState, nextState, action, reward);

            // Actualizar el estado acual como el estado siguiente
            currentState = nextState;

            // Seleccionar una acción utilizando la política 𝜀-greedy
            action = chooseAction(currentState);
            cout << action << endl;

            // Aplicar la acción
            // Si la acción es A2, se disminuye el tamaño de las especies no dominantes y se aumenta el tamaño de la especie dominante
            if (action == A2) {
                // Variable que almacena la cantidad de especies que se pueden reducir
                int speciesRate = TOTALSPECIES - 1;
                // Tamaño que se va a aumentar en la especie dominante
                float newSize = config->speciesShrinkRate * speciesRate;
                // Si el tamaño de la especie dominante no sobrrepasa la unidad
                if (species[currentState.speciesIndex].size + newSize <= 1) {
                    // Reducir tamaño de las especies no dominantes
                    for (int i = 0; i < TOTALSPECIES; ++i) {
                        // Si la especie no es dominante
                        if (i != currentState.speciesIndex) {
                            // Si el tamaño de la especie no dominante se puede reducir sin llegar a ser negativo
                            if (species[i].size >= newSize / speciesRate) {
                                species[i].size -= newSize / speciesRate;
                            }
                            // Si la especie no dominante no se puede reducir, se reduce la cantidad de especies que se pueden reducir
                            else {
                                speciesRate--;
                            }
                        }
                    }
                    // Si las especies no dominantes se redujeron, se aumenta el tamaño de la especie dominante
                    if (speciesRate > 0) {
                        species[currentState.speciesIndex].size += newSize;
                    }
                }
            }

            if (config->verbose) {
                cout << iteration << " ";
                for (int i = 0; i < TOTALSPECIES; ++i) {
                    cout << species[i].size << " ";
                }
                cout << endl;
            }

        }

        // Ordenar aletoriamente la poblacion
        order.randomize(&config->seed, 0, config->populationSize);
        // Dividir aletoriamente la poblacion global en las especies
        int chromosomeIndex = 0;
        for (int i = 0; i < TOTALSPECIES; ++i) {
            species[i].population.size = 0;
            // Asignar cromosomas a la especie en proporción a su tamaño
            for (int j = 0; j < config->populationSize * species[i].size && chromosomeIndex < config->populationSize; j++) {
                species[i].population.chromosomes[j] = population.chromosomes[order[chromosomeIndex++]];
                species[i].population.size++;
            }

        }

        // Ordena los individuos de la población en orden ascendente de su fitness
        sortChromosomesByFitness(population);

    }

    // Regresa la mejor solucion
    solution = population.bestChromosome;
    return;
    
}

int main(int argc, char* argv[]) {
    // argv[1]: Archivo de instancia.
    // argv[2]: Archivo de configuracion (opcional).

    // Lee una instancia del problema desde el archivo.
    Instance instance = readInstanceFile(argv[1]);

    // Luego, se crea una configuración inicial que se modificara si se proporciona un archivo de configuración adicional.
    if (argc > 2) {
        // Revisar debug_configuration.txt
        readConfigurationFile(argv[2], instance.config);
    }

    // Calcular el limite inferior de la instancia para BPP.
    lowerBound(instance);

    // Inicio de la medición de tiempo
    auto start = clock();
    // A continuación, se llama al genetico pasando la instancia y la configuración.
    Chromosome solution;
    coevolution(instance, solution);
    solution.time = (clock() - start) / (CLK_TCK * 1.0);

    // Verificamos la solución
    //if (!allItemsIncluded(solution)) exit(3);

    // La solución devuelta se imprime en formato JSON en la consola.
    if (instance.config.verbose) {
        cout << "Iteration: " << solution.iteration << endl;
        cout << "Solution: " << solution.totalGroups << endl;
        printChromosomeAsJson(solution, true);
    }
    else {
        //printChromosomeAsJson(solution, true);
    }

    return 0;
}

