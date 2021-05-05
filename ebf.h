#include <vector>
#include <iostream>
#include <stdlib.h>
#include <time.h>

#define INPUT_CNT 8
#define FIRST_COL_SIZE 4
#define SEC_COL_SIZE 2
#define FUNC_CNT FIRST_COL_SIZE + SEC_COL_SIZE + 1

#define FUNC_INPUT_SIZE 4         // max values in one function (eg. 4: A and B and C and D)
#define USABLE_FUNCTIONS 5

#define POP_SIZE 5              // size of population
#define GENERATIONS_NUM 100     // number of generations - length of evolution
#define EVOLUTIONS_NUM 100      // number of separate evolutions
#define GENS_MUTATTIONS_NUM 3   // max number of mutations in 1 chromosome




enum Function {fstill, fand, f_or, fxor, fnot, fnand, fnor, fnone};
enum Inputs {A=1, B, C, D, E, F, G, H};


typedef struct Node_t{
    int index;
    Function func;
    std::vector<int> inputs;
} Node;