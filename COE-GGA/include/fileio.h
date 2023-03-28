#ifndef FILEIO_H
#define FILEIO_H

#include <string>
#include <fstream>
#include <iostream>

#include "structures.h"

using namespace std;

/**
 * @brief Lee el archivo de configuración con los valores correspondientes.
 * @param fileName Nombre del archivo de configuración.
 * @param config Referencia a la estructura de configuración que se actualizará con los valores del archivo.
*/
void readConfigurationFile(string fileName, ConfigurationCOE& config);

/**
 * @brief Lee el archivo de instancia y devuelve una estructura Instance con los valores correspondientes.
 * @param fileName Nombre del archivo de instancia.
 * @return Instance La instancia del problema leída desde el archivo.
*/
Instance readInstanceFile(string fileName);

/**
 * @brief Imprime el cromosoma en formato JSON.
 * @param chromosome El cromosoma a imprimir.
 * @param printGroups Indica si se deben imprimir los grupos del cromosoma.
*/
void printChromosomeAsJson(const Chromosome& chromosome, bool printGroups);

#endif
