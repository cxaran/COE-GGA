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
    unsigned int populationSize = 100; // Tama�o de la poblaci�n
    unsigned int iterations = 500; // N�mero de iteraciones
    unsigned int learningWindow = 10; // Ventana de aprendizaje
    float elitism = 0.1; // Tasa de elitismo
    float speciesShrinkRate = 0.1; // Tasa de disminuci�n del tama�o de las especies
    int seed = 1; // Semilla para el generador de n�meros aleatorios
    int verbose = 1; // Verbosidad (1 para mostrar en pantalla las operaciones realizadas)
};



// Estructura de configuracion del genetico
struct ConfigurationGGA {
    float mutation = 0.82; // Tasa de mutaci�n
    float crossover = 0.22; // Tasa de cruce
    float averageRateK = 1.3; // Tasa de cambio de la mutaci�n para la poblaci�n
    float eliteRateK = 4.0; // Tasa de cambio de la mutaci�n para la poblaci�n elite
    float elitism = 0.1; // Tasa de elitismo
    unsigned int lifeSpan = 10; // Tiempo de vida de un individuo en generaciones
    int seed = 1; // Semilla para el generador de n�meros aleatorios en permutaciones
    int mutationSeed = 1; // Semilla para el generador de n�meros aleatorios en la muta
    Permutation bestIndividuals; // Mejores individuos para reproducci�n
    Permutation randomIndividuals; // Individuos aleatorios para reproducci�n
};

// Estructura para un art�culo
struct Item {
    const unsigned int id; // ID del art�culo
    long unsigned int weight; // Peso minimo del art�culo
    long unsigned int weights[MAXITEMS]; // Pesos del art�culo
    unsigned int position; // Posici�n en el orden de mayor a menor peso
};

// Estructura para un grupo
struct Group {
    unsigned int id; // ID del grupo
    unsigned int volume = 0; // Volumen del grupo
    Item* items[MAXITEMS]; // Art�culos en el grupo
    unsigned int totalItems = 0; // Total de art�culos en el grupo
};

// Estructura para una instancia de un problema
struct Instance {
    unsigned int numItems = 0; // N�mero de art�culos
    unsigned int numGroups = 0; // N�mero grupos posibles
    long unsigned int capacity; // Capacidad de los grupos
    unsigned int knowBest; // Mejor soluci�n conocida
    unsigned int lowerBound; // L�mite inferior
    unsigned int n_ = 0; // Articulos grandes
    long double totalWeight = 0; // Peso total acumulado
    long unsigned int minWeight; // Peso minimo de los articulos
    Item* items [MAXITEMS]; // Art�culos
    Permutation permutation; // Permutaci�n de los �ndices de los art�culos
    long int weightFactor = 1.0; // Factor de multiplicaci�n para convertir flotantes a enteros
    ConfigurationCOE config; // Configuraci�n del algoritmo coevolutivo
    unsigned int problemType = 0; // Tipo de problema de agrupaci�n
};

// Estructura para un cromosoma
struct Chromosome {
    Instance* problem; // Instancia del problema
    unsigned int iteration = 0; // Iteraci�n en la que se gener� la soluci�n
    double fitness = 0.0; // Aptitud de la soluci�n
    Group* groups[MAXITEMS]; // Grupos en la soluci�n
    unsigned int totalGroups = 0; // Total de grupos
    unsigned int maxEmptyVolume; // Volumen del grupo m�s vac�o
    unsigned int maxSpan = 0; // Variable maxSpan para almacenar el m�ximo span de los grupos
    float time; // Tiempo de proceso

     // Funci�n para obtener el volumen de un grupo en particular
    long unsigned int getGroupVolume(int groupIndex) const {
        return groupIndex < totalGroups ? groups[groupIndex]->volume : 0;
    }
};

// Estructura para una poblaci�n de cromosomas
struct Population {
    unsigned int size = 0; // Tama�o de la poblaci�n
    Chromosome* chromosomes[MAXSOLS]; // Cromosomas
    Chromosome bestChromosome; // Mejor cromosoma encontrado en la poblaci�n
    // Operador [] para acceder al i-�simo elemento de la permutaci�n
    const Chromosome* operator[](int i) const { return chromosomes[i]; }
    // Operador [] para modificar el i-�simo elemento de la permutaci�n
    Chromosome& operator[](int i) { return *chromosomes[i]; }
};

// Estructura para una especie
struct Species {
    float size; // Tama�o de la especie (representado como un float)
    Population population; // Poblaci�n de la especie
    double fitness; // Fitness de la especie
};

/**
 * @brief Ordena los elementos de una instancia seg�n su peso.
 * @param instance Instancia del problema
 */
void sortItemsByWeight(Instance& instance);

/**
 * @brief Vac�a los elementos del grupo
 * @param group El grupo que se desea vaciar
*/
void clearGroup(Group& group);

/**
 * @brief Obtiene el articulo en la posici�n indicada segun su peso.
 * @param chromosome Referencia constante a un objeto Chromosome que contiene la informaci�n de la soluci�n.
 * @param itemPos Entero que indica el �ndice del objeto a consultar.
 * @return El item en la posici�n indicada.
*/
Item* getItem(const Chromosome& chromosome, int itemPos);

/**
 * @brief Agrega un elemento a un grupo existente si es posible.
 * @param group Grupo al que se intentar� agregar el elemento.
 * @param item Elemento que se agregar� al grupo.
*/
void addItemToGroup(Group& group, Item& item);

/**
 * @brief Crea un nuevo grupo con el elemento especificado y lo agrega al cromosoma si es posible.
 * @param chromosome Cromosoma al que se intentar� agregar el grupo.
 * @param item Elemento que se agregar� al nuevo grupo.
 */
void createNewGroupWithItem(Chromosome& chromosome, Item& item);

/**
 * @brief Ordena aleatoriamente permutation sin tomar los � objetos grandes.
 * @param instance Instancia del problema
 */
void shufflePermutationN_(Instance& instance);

/**
 * @brief Elimina un item de un grupo dado su posici�n en el grupo
 * @param group Referencia al grupo del cual se eliminar� el item
 * @param itemPos Posici�n del item que se desea eliminar en el grupo
*/
void removeItemFromGroup(Group& group, int itemPos);

/**
 * @brief Agrega un cromosoma a la poblaci�n y actualiza el mejor cromosoma si es necesario.
 * @param population Vector que representa la poblaci�n actual.
 * @param chromosome Cromosoma que se desea agregar a la poblaci�n.
*/
void addChromosomeToPopulation(Population& population, Chromosome& chromosome);

/**
 * @brief Ordena los cromosomas de una poblaci�n de peor a mejor seg�n su aptitud
 * @param population Poblaci�n de cromosomas
*/
void sortChromosomesByFitness(Population& population);

/**
 * @brief Ordena una permutaci�n de los �ndices de grupos en una soluci�n, en orden descendente seg�n su volumen.
 * @param chromosome La soluci�n cuyos grupos se ordenar�n.
 * @param order La permutaci�n que se ordenar�.
 * @param ascending Si el orden es de forma ascendente, de ser falso sera descendente.
*/
void sortGroupsByPermutation(const Chromosome& chromosome, Permutation& order, bool ascending);

/**
 * @brief Funci�n para ordenar una permutacion seg�n el peso de los art�culos que corresponden.
 * @param instance Instancia del problema.
 * @param order Permutaci�n a ordenar.
 * @param size Tama�o de la permutacion.
*/
void sortPermutationByItems(const Instance& instance, Permutation& order, int size);

/**
 * @brief Agrega un grupo existente a un cromosoma, actualiza sus variables asociadas.
 * @param chromosome El cromosoma al que se le agregar� el grupo.
 * @param group El grupo que se agregar� al cromosoma.
*/
void addGroupToChromosome(Chromosome& chromosome,const Group& group);

/**
 * @brief Elimina los grupos vac�os de una soluci�n.
 * @param chromosome Referencia al cromosoma que se va a procesar.
*/
void removeEmptyGroups(Chromosome& chromosome);

/**
 * @brief Agrega un cromosoma a la poblaci�n reemplazando otro existente.
 * @param population Poblaci�n actual.
 * @param newChromosome Nuevo cromosoma a agregar.
 * @param replaceIndex �ndice del cromosoma�a�reemplazar.
*/
void replaceChromosomeInPopulation(Population& population, Chromosome& chromosome, int index);

/**
 * @brief Limpia el contenido de un cromosoma.
 * @param chromosome Referencia al cromosoma que se desea limpiar.
*/
void clearChromosome(Chromosome& chromosome);

/**
 * @brief Funci�n para calcular el fitness de una especie
 * @param specie a calcular el fitness
*/
void calculateSpeciesFitness(Species& specie);

/**
 * @brief Determina si todos los elementos est�n incluidos en un cromosoma.
 * @param chromosome Cromosoma que se desea validar.
*/
bool allItemsIncluded(const Chromosome& chromosome);

/**
 * @brief Comprueba si el n�mero de grupos en un cromosoma es igual al lower bound.
 * @param chromosome Referencia al cromosoma a comprobar.
 * @return Verdadero si el n�mero de grupos en el cromosoma es igual al lower bound, falso en caso contrario.
 */
bool checkLowerBound(const Chromosome& chromosome);

#endif