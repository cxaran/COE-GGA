#ifndef RANDOM_UTILS_H
#define RANDOM_UTILS_H

/******************************************************
 Author: Adriana Cesario de Faria Alvim               *
         (alvim@inf.puc-rio.br, adriana@pep.ufrj.br ) *
*******************************************************/

/***********************************************************
 The generator produces a sequence of positive integers, "ix".*
 Input: integers "seed", "i" and "j" in [1,2147483646]     *
 Ouput: integer in [i,j]                                   *
************************************************************/

float randp(int* ix);

/***********************************************************
 To generate a random integer "k" in [i,j].                *
 Input: integers "seed", "i" and "j" in [1,2147483646]     *
 Ouput: integer in [i,j]                                   *
************************************************************/
int get_rand_ij(int* seed, int i, int j);

/**************************************************************
 To generate a random integer "k" in [1,size].                *
 Input: integers "seed" and "size" in [1,2147483646]          *
 Ouput: integer in [1,size]
                                   *
**************************************************************/
int get_rand(int* seed, int size);

/*************************************************
 To check the correctness of the implementation. *
 Output: correct (1) or wrong (0)                 *
**************************************************/
int trand();

#endif
