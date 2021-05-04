#include <vector>
#include <iostream>

#define INPUT_CNT 8
#define FIRST_COL_SIZE 4
#define SEC_COL_SIZE 2
#define FUNC_CNT FIRST_COL_SIZE + SEC_COL_SIZE + 1

#define FUNC_INPUT_SIZE 4         // max values in one function (eg. 4: A and B and C and D)


enum Function {fstill, fand, f_or, fxor, fnot, fnand, fnor, fnone};
enum Inputs {A=1, B, C, D, E, F, G, H};


typedef struct Node_t{
    int index;
    Function func;
    std::vector<int> inputs;
} Node;