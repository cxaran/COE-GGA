 #ifndef PERMUTATION_H
#define PERMUTATION_H

#define	MAXSIZE 5000

#include "random_utils.h"

using namespace std;

// Estructura para una permutaci�n de �ndices
struct Permutation {
    long int indices[MAXSIZE]; // Vector de �ndices permutados
    int size = 0; // Tama�o de la permutaci�n

    // Constructor vac�o
    Permutation() {}

    // Constructor con tama�o
    Permutation(int sizePermutation) {
        size = sizePermutation;
        for (int i = 0; i < size; i++) {
            indices[i] = i;
        }
    }

    // Constructor con tama�o y valor defecto
    Permutation(int sizePermutation, int value) {
        size = sizePermutation;
        for (int i = 0; i < size; i++) {
            indices[i] = value;
        }
    }

    // Cambiar el tama�o de la permutaci�n
    void reSize(int sizePermutation) {
        for (int i = sizePermutation > size ? size : 0; i < sizePermutation; i++) {
            indices[i] = i;
        }
        size = sizePermutation;
    }

    // Operador [] para acceder al i-�simo elemento de la permutaci�n
    long int operator[](int i) const {
        return indices[i];
    }

    // Operador [] para modificar el i-�simo elemento de la permutaci�n
    long int& operator[](int i) {
        return indices[i];
    }

    // Permutar la posici�n de dos elementos de la permutaci�n
    void swap(int i, int j) {
        if (i != j) {
            indices[i] += indices[j];
            indices[j] = indices[i] - indices[j];
            indices[i] -= indices[j];
        }
    }

    // Funci�n que organiza aleatoriamente la permutaci�n dentro de un rango
    void randomize(int* seed, int start, int end) {
        // Generar una permutaci�n aleatoria dentro del rango
        for (int i = end - 1; i >= start; i--) {
            int j = start + get_rand(seed, end - start) - 1;
            swap(i, j);
        }
    }

};

#endif