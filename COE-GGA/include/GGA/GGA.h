#ifndef GGA_H
#define GGA_H

#include "../structures.h"

#include "crossover.h"
#include "mutation.h"

/**
 * @brief Ejecuta el algoritmo gen�tico.
 * @param config puntero a un objeto ConfigurationGGA con la configuraci�n para el algoritmo gen�tico.
 * @param specie referencia a un objeto Species que representa la especie a reproducir.
 * @param children referencia a un objeto Population para almacenar la descendencia.
*/
void GGA(ConfigurationGGA* config, Species& specie, Population& children, int iteration);

#endif