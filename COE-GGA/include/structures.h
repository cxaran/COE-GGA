/*

This code defines several structures and classes to model a coevolutionary algorithm, including the following main components:

1. Configuration structures: These are used to store the algorithm's parameters.
    ConfigurationCOE: Stores configuration parameters for the coevolutionary algorithm.
    ConfigurationGGA: Stores configuration parameters for the genetic algorithm used in the coevolutionary algorithm.

2. Problem-related structures:
    Item: Represents an item with its ID, weight, and position.
    Group: Represents a group containing an ID, volume, and a list of items.
    Instance: Represents an instance of the problem with the number of items, groups, capacity, and other related data.

3. Evolution-related structures:
    Chromosome: Represents a chromosome containing a problem instance, fitness, groups, and other data related to the solution.
    Population: Represents a population of chromosomes, including the best chromosome found in the population.
    Species: Represents a species containing its size, population, and fitness.

The purpose of the coevolutionary algorithm is to find the optimal or near-optimal solution for grouping items into groups with limited capacity. The algorithm combines genetic algorithms and coevolutionary strategies to achieve this goal. In the code, the problem instance, items, and groups are all modeled with their respective structures, while the evolution process is modeled with chromosome, population, and species structures.
The code also includes a Chromosome method getGroupVolume that returns the volume of a specific group within the chromosome. The population structure has overloaded operator[] to access and modify chromosomes in the population.

*/

#ifndef STRUCTURES_H
#define STRUCTURES_H

#define	MAXITEMS 5000 // Numero maximo de items
#define MAXSOLS 500 // Numero maximo de cromosomas

#include <algorithm>

#include "permutation.h"

using namespace std;

// Estructura de configuracion del algoritmo coevolutivo
struct ConfigurationCOE {
    unsigned int populationSize = 100; // Tamaño de la población
    unsigned int iterations = 500; // Número de iteraciones
    unsigned int learningWindow = 10; // Ventana de aprendizaje
    float elitism = 0.1; // Tasa de elitismo
    float speciesShrinkRate = 0.1; // Tasa de disminución del tamaño de las especies
    int seed = 1; // Semilla para el generador de números aleatorios
    int verbose = 1; // Verbosidad (1 para mostrar en pantalla las operaciones realizadas)
};



// Estructura de configuracion del genetico
struct ConfigurationGGA {
    float mutation = 0.82; // Tasa de mutación
    float crossover = 0.22; // Tasa de cruce
    float averageRateK = 1.3; // Tasa de cambio de la mutación para la población
    float eliteRateK = 4.0; // Tasa de cambio de la mutación para la población elite
    float elitism = 0.1; // Tasa de elitismo
    unsigned int lifeSpan = 10; // Tiempo de vida de un individuo en generaciones
    int seed = 1; // Semilla para el generador de números aleatorios en permutaciones
    int mutationSeed = 1; // Semilla para el generador de números aleatorios en la muta
    Permutation bestIndividuals; // Mejores individuos para reproducción
    Permutation randomIndividuals; // Individuos aleatorios para reproducción
};

// Estructura para un artículo
struct Item {
    const unsigned int id; // ID del artículo
    long unsigned int weight; // Peso minimo del artículo
    long unsigned int weights[MAXITEMS]; // Pesos del artículo
    unsigned int position; // Posición en el orden de mayor a menor peso
};

// Estructura para un grupo
struct Group {
    unsigned int id; // ID del grupo
    unsigned int volume = 0; // Volumen del grupo
    Item* items[MAXITEMS]; // Artículos en el grupo
    unsigned int totalItems = 0; // Total de artículos en el grupo
};

// Estructura para una instancia de un problema
struct Instance {
    unsigned int numItems = 0; // Número de artículos
    unsigned int numGroups = 0; // Número grupos posibles
    long unsigned int capacity; // Capacidad de los grupos
    unsigned int knowBest; // Mejor solución conocida
    unsigned int lowerBound; // Límite inferior
    unsigned int n_ = 0; // Articulos grandes
    long double totalWeight = 0; // Peso total acumulado
    long unsigned int minWeight; // Peso minimo de los articulos
    Item* items [MAXITEMS]; // Artículos
    Permutation permutation; // Permutación de los índices de los artículos
    long int weightFactor = 1.0; // Factor de multiplicación para convertir flotantes a enteros
    ConfigurationCOE config; // Configuración del algoritmo coevolutivo
    unsigned int problemType = 0; // Tipo de problema de agrupación
};

// Estructura para un cromosoma
struct Chromosome {
    Instance* problem; // Instancia del problema
    unsigned int iteration = 0; // Iteración en la que se generó la solución
    double fitness = 0.0; // Aptitud de la solución
    Group* groups[MAXITEMS]; // Grupos en la solución
    unsigned int totalGroups = 0; // Total de grupos
    unsigned int maxEmptyVolume; // Volumen del grupo más vacío
    unsigned int maxSpan = 0; // Variable maxSpan para almacenar el máximo span de los grupos
    float time; // Tiempo de proceso

     // Función para obtener el volumen de un grupo en particular
    long unsigned int getGroupVolume(int groupIndex) const {
        return groupIndex < totalGroups ? groups[groupIndex]->volume : 0;
    }
};

// Estructura para una población de cromosomas
struct Population {
    unsigned int size = 0; // Tamaño de la población
    Chromosome* chromosomes[MAXSOLS]; // Cromosomas
    Chromosome bestChromosome; // Mejor cromosoma encontrado en la población
    // Operador [] para acceder al i-ésimo elemento de la permutación
    const Chromosome* operator[](int i) const { return chromosomes[i]; }
    // Operador [] para modificar el i-ésimo elemento de la permutación
    Chromosome& operator[](int i) { return *chromosomes[i]; }
};

// Estructura para una especie
struct Species {
    float size; // Tamaño de la especie (representado como un float)
    Population population; // Población de la especie
    double fitness; // Fitness de la especie
};

/**
 * @brief Ordena los elementos de una instancia según su peso.
 * @param instance Instancia del problema
 */
void sortItemsByWeight(Instance& instance);

/**
 * @brief Vacía los elementos del grupo
 * @param group El grupo que se desea vaciar
*/
void clearGroup(Group& group);

/**
 * @brief Obtiene el articulo en la posición indicada segun su peso.
 * @param chromosome Referencia constante a un objeto Chromosome que contiene la información de la solución.
 * @param itemPos Entero que indica el índice del objeto a consultar.
 * @return El item en la posición indicada.
*/
Item* getItem(const Chromosome& chromosome, int itemPos);

/**
 * @brief Agrega un elemento a un grupo existente si es posible.
 * @param group Grupo al que se intentará agregar el elemento.
 * @param item Elemento que se agregará al grupo.
*/
void addItemToGroup(Group& group, Item& item);

/**
 * @brief Crea un nuevo grupo con el elemento especificado y lo agrega al cromosoma si es posible.
 * @param chromosome Cromosoma al que se intentará agregar el grupo.
 * @param item Elemento que se agregará al nuevo grupo.
 */
void createNewGroupWithItem(Chromosome& chromosome, Item& item);

/**
 * @brief Ordena aleatoriamente permutation sin tomar los ñ objetos grandes.
 * @param instance Instancia del problema
 */
void shufflePermutationN_(Instance& instance);

/**
 * @brief Elimina un item de un grupo dado su posición en el grupo
 * @param group Referencia al grupo del cual se eliminará el item
 * @param itemPos Posición del item que se desea eliminar en el grupo
*/
void removeItemFromGroup(Group& group, int itemPos);

/**
 * @brief Agrega un cromosoma a la población y actualiza el mejor cromosoma si es necesario.
 * @param population Vector que representa la población actual.
 * @param chromosome Cromosoma que se desea agregar a la población.
*/
void addChromosomeToPopulation(Population& population, Chromosome& chromosome);

/**
 * @brief Ordena los cromosomas de una población de peor a mejor según su aptitud
 * @param population Población de cromosomas
*/
void sortChromosomesByFitness(Population& population);

/**
 * @brief Ordena una permutación de los índices de grupos en una solución, en orden descendente según su volumen.
 * @param chromosome La solución cuyos grupos se ordenarán.
 * @param order La permutación que se ordenará.
 * @param ascending Si el orden es de forma ascendente, de ser falso sera descendente.
*/
void sortGroupsByPermutation(const Chromosome& chromosome, Permutation& order, bool ascending);

/**
 * @brief Función para ordenar una permutacion según el peso de los artículos que corresponden.
 * @param instance Instancia del problema.
 * @param order Permutación a ordenar.
 * @param size Tamaño de la permutacion.
*/
void sortPermutationByItems(const Instance& instance, Permutation& order, int size);

/**
 * @brief Agrega un grupo existente a un cromosoma, actualiza sus variables asociadas.
 * @param chromosome El cromosoma al que se le agregará el grupo.
 * @param group El grupo que se agregará al cromosoma.
*/
void addGroupToChromosome(Chromosome& chromosome,const Group& group);

/**
 * @brief Elimina los grupos vacíos de una solución.
 * @param chromosome Referencia al cromosoma que se va a procesar.
*/
void removeEmptyGroups(Chromosome& chromosome);

/**
 * @brief Agrega un cromosoma a la población reemplazando otro existente.
 * @param population Población actual.
 * @param newChromosome Nuevo cromosoma a agregar.
 * @param replaceIndex Índice del cromosoma a reemplazar.
*/
void replaceChromosomeInPopulation(Population& population, Chromosome& chromosome, int index);

/**
 * @brief Limpia el contenido de un cromosoma.
 * @param chromosome Referencia al cromosoma que se desea limpiar.
*/
void clearChromosome(Chromosome& chromosome);

/**
 * @brief Función para calcular el fitness de una especie
 * @param specie a calcular el fitness
*/
void calculateSpeciesFitness(Species& specie);

/**
 * @brief Determina si todos los elementos están incluidos en un cromosoma.
 * @param chromosome Cromosoma que se desea validar.
*/
bool allItemsIncluded(const Chromosome& chromosome);

/**
 * @brief Comprueba si el número de grupos en un cromosoma es igual al lower bound.
 * @param chromosome Referencia al cromosoma a comprobar.
 * @return Verdadero si el número de grupos en el cromosoma es igual al lower bound, falso en caso contrario.
 */
bool checkLowerBound(const Chromosome& chromosome);

#endif