#include <vector>
#include <iostream>
#include <stdlib.h>
#include <time.h>

#define INPUT_CNT 8             // !! DON'T CHANGE !! - given by assignment
#define FIRST_COL_SIZE 8        // MAY CHANGE - size of first column
#define SEC_COL_SIZE 8          // MAY CHANGE - size of second column
#define FUNC_CNT FIRST_COL_SIZE + SEC_COL_SIZE + 1      // !! DON'T CHANGE !! - sum of parameters

#define FUNC_INPUT_SIZE 4       // MAY CHANGE - max values in one function (eg. 4: (A and B and C and D))
#define USABLE_FUNCTIONS 4      // MAY CHANGE - number of enumerated functions (Function) that will be used

#define POP_SIZE 5              // MAY CHANGE - size of population
#define GENERATIONS_NUM 100000    // MAY CHANGE - number of generations - length of evolution
#define RUNS_NUM 100            // MAY CHANGE - number of separate evolutions
#define GENS_MUTATTIONS_NUM 4   // MAY CHANGE - max number of mutations in 1 chromosome

#define REQUIRED_COR_IM 1       // MAY CHANGE - required correlation immunity
#define LOG_PERIODE 5000         // MAY CHANGE - periode of logging in evolution (log will be printed every LOG_PERIODE generation)

using namespace std;
using chromosome_t = vector<int>;

enum Function {fxor, fand, f_or, fnot, fnand, fnor, fstill, fnone};
enum Inputs {A=1, B, C, D, E, F, G, H};


typedef struct Node_t{
    int index;
    Function func;
    std::vector<int> inputs;
} Node;


void print_function(Function func);
void show_function();
void print_table(vector<int> table, string label);bool eval_single_value(bool val1, bool val2, Function func);
bool eval_node(vector<int> indexes, vector<bool> vals, Function func);
bool eval_input(int input_int);
vector<int> get_truth_table();
vector<int> create_input_vector_for_fwht(vector<int> table);
vector<int> get_fwht_indexes(vector<int> table);
vector<int> fwht(vector<int> table);
int get_hamming_weight(vector<int> truth_table);
int count_set_bits(int num);
int get_correlation_immunity(vector<int> indexes);
void eval_bool_function(chromosome_t chrom, int* ham_w, int* cor_im);
vector<int> encode_chromosome();
void decode_chromosome(vector<int> chromosome);
vector<int> generate_chromosome();
chromosome_t make_mutation(chromosome_t chrom);
void init_population();
int get_fitness(chromosome_t chrom, int required_cor_imm);
void eval_population(int required_cor_imm, int* best_fit, int* best_fit_idx);
void do_evolution(int required_cor_imm);
void init_function_table();
