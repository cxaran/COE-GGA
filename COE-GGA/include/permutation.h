 #ifndef PERMUTATION_H
#define PERMUTATION_H

#define	MAXSIZE 5000

#include "random_utils.h"

using namespace std;

// Estructura para una permutación de índices
struct Permutation {
    long int indices[MAXSIZE]; // Vector de índices permutados
    int size = 0; // Tamaño de la permutación

    // Constructor vacío
    Permutation() {}

    // Constructor con tamaño
    Permutation(int sizePermutation) {
        size = sizePermutation;
        for (int i = 0; i < size; i++) {
            indices[i] = i;
        }
    }

    // Constructor con tamaño y valor defecto
    Permutation(int sizePermutation, int value) {
        size = sizePermutation;
        for (int i = 0; i < size; i++) {
            indices[i] = value;
        }
    }

    // Cambiar el tamaño de la permutación
    void reSize(int sizePermutation) {
        for (int i = sizePermutation > size ? size : 0; i < sizePermutation; i++) {
            indices[i] = i;
        }
        size = sizePermutation;
    }

    // Operador [] para acceder al i-ésimo elemento de la permutación
    long int operator[](int i) const {
        return indices[i];
    }

    // Operador [] para modificar el i-ésimo elemento de la permutación
    long int& operator[](int i) {
        return indices[i];
    }

    // Permutar la posición de dos elementos de la permutación
    void swap(int i, int j) {
        if (i != j) {
            indices[i] += indices[j];
            indices[j] = indices[i] - indices[j];
            indices[i] -= indices[j];
        }
    }

    // Función que organiza aleatoriamente la permutación dentro de un rango
    void randomize(int* seed, int start, int end) {
        // Generar una permutación aleatoria dentro del rango
        for (int i = end - 1; i >= start; i--) {
            int j = start + get_rand(seed, end - start) - 1;
            swap(i, j);
        }
    }

};

#endif