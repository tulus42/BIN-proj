#include "ebf.h"


using namespace std;
using chromosome_t = vector<int>;

Node last_node;
vector<Node> first_col;
vector<Node> sec_col;
vector<chromosome_t> population;

int best_hw;
int best_ci;
int hw;
int ci_reached_in = 0;
vector<int> res_best_hw;
vector<chromosome_t> res_best_chromosome;
// int hw_reached_in = 0;

vector<int> fill_table(string input) {
    vector<int> table;
    int n = input.size();
    for (int i=0; i < n; i++) {
        table.push_back(input[i] == '0' ? 0 : 1);
    }

    return table;
}

// -------------------------------------------------------------------------
// PRINTS // ###############################################################
void print_function(Function func) {
    switch (func)
    {
    case 1:
        cout << " & ";    
        break;
    case 2:
        cout << " || ";
        break;
    case 3:
        cout << " xor ";
        break;
    case 4:
        cout << " !";
        break;
    case 5:
        cout << " !&";
        break;
    case 6:
        cout << " !|";
        break;


    default:
        break;
    }
}


void show_function() {
    char char_ord;

    // from root -> second col - 2 children nodes
    for (int i=0; i < SEC_COL_SIZE; i++) {
        //if (function_table[])
        cout << "(";

        // from second col -> first col - max 4 children nodes
        for (int j=0; j < FIRST_COL_SIZE; j++) {
            
            cout << "(";

            // from first col -> inputs - max 4 inputs
            int max_k = first_col[j].inputs.size();
            for (int k=0; k < max_k; k++) {
                // get input char from its index
                char_ord = first_col[j].inputs[k] + 64; 

                cout << char_ord;
                if (k < max_k-1) {
                    print_function(first_col[j].func);
                }
            }

            cout << ")";
            if (j < FIRST_COL_SIZE-1) print_function(sec_col[i].func);
        }

        cout << ")";
        if (i == 0) print_function(last_node.func);
    }
    cout << endl;
}

void print_table(vector<int> table, string label) {
    cout << label << endl;
    for (int i=0; i < table.size(); i++) {
        cout << table[i] << " ";
        if ((i+1) % 64 == 0) cout << endl;
    }
    cout << endl;
}

void print_chromosome(vector<int> table, string label) {
    cout << label << endl;
    for (int i=0; i < table.size(); i++) {
        cout << table[i] << " ";
    }
    cout << endl;
}
// END PRINTS // ###########################################################
// =========================================================================

// -------------------------------------------------------------------------
// CALCULATING TRUTH TABLE // ##############################################
bool eval_single_value(bool val1, bool val2, Function func) {
    switch (func)
    {
    case fstill:
        return val1;

    case fand:
        return val1 & val2;
        
    case f_or:
        return val1 | val2;
        
    case fxor:
        return val1 ^ val2;
        break;
    case fnot:
        return !val1;
        
    case fnand:
        return !(val1 & val2);
        
    case fnor:
        return !(val1 | val2);
    
    default:
        return 0;
    }
}


bool eval_node(vector<int> indexes, vector<bool> vals, Function func) {
    if (func == fnone) return 0;

    bool actual_val = vals[indexes[0]];
    for (int i=1; i < indexes.size(); i++) {
        if (indexes[i] == 0) continue;
        actual_val = eval_single_value(actual_val, vals[i], func);
    }

    if (indexes.size() == 1) {
        if (func == fnot) return !actual_val;
        if (func == fnone) return 0;
    } 

    return actual_val;
}


bool eval_input(int input_int) {
    vector<bool> inputs_bin;
    bool tmp;

    // get 8 binary values from 1 int
    for (int i=0; i < 8; i++) {
        tmp = input_int & 0x01;
        inputs_bin.insert(inputs_bin.begin(), tmp);
        input_int = input_int >> 1;
    } 

    // evaluate first column
    vector<bool> first_col_vals;
    for (int i=0; i < FIRST_COL_SIZE; i++) {
        first_col_vals.push_back(eval_node(first_col[i].inputs, inputs_bin, first_col[i].func));
    }

    vector<bool> second_col_vals;
    for (int i=0; i < SEC_COL_SIZE; i++) {
        second_col_vals.push_back(eval_node(sec_col[i].inputs, first_col_vals, sec_col[i].func));
    }

    bool res;
    
    res = eval_node(last_node.inputs, second_col_vals, last_node.func);
    
    return res;
} 


vector<int> get_truth_table() {
    vector<int> truth_table;
    for (int i=0; i < 256; i++) {
        truth_table.push_back(eval_input(i));
        // if ((i+1) % 64 == 0) cout << endl;
    }

    return truth_table;
}
// END CALCULATING TRUTH TABLE // ##########################################
// =========================================================================


// -------------------------------------------------------------------------
// CALCULATING FWHT // #####################################################
vector<int> create_input_vector_for_fwht(vector<int> table) {
    int n = table.size();
    for (int i=0; i < n; i++) {
        table[i] = table[i] == 0 ? -1 : 1;
    }

    return table;
}


vector<int> get_fwht_indexes(vector<int> table) {
    int n = table.size();
    vector<int> indexes;
    for (int i=0; i < n; i++) {
        if (table[i] != 0) {
            indexes.push_back(i);
        }
    }

    return indexes;
}


vector<int> fwht(vector<int> table) {

    int h = 1;
    int tab_size = table.size();
    int x;
    int y;
    while (h < tab_size) {
        for (int i=0; i < tab_size; i+=h*2) {
            for (int j=i; j < i+h; j++) {
                x = table[j];
                y = table[j +h];
                table[j] = x + y;
                table[j + h] = x - y;
            }
        }
        h *= 2;
    }
    return table;
}
// END CALCULATING FWHT // #################################################
// =========================================================================


// -------------------------------------------------------------------------
// EVAL BOOL FUNCTION // ###################################################
int get_hamming_weight(vector<int> truth_table) {
    int cnt = 0;
    int n = truth_table.size();
    for (int i=0; i < n; i++) {
        if (truth_table[i] == 1) cnt++;        
    }

    return cnt;
}

int count_set_bits(int num) {
    int cnt = 0;
    while (num) {
        cnt += num & 1;
        num >>= 1;
    }

    return cnt;
}

int get_correlation_immunity(vector<int> indexes) {
    // print_table(indexes, "Cor imm");

    int lowest_immunity = 9999;
    int tmp_immunity;
    int n = indexes.size();

    if (n == 0 || (n == 1 && indexes[0] == 0)) return 0;

    for (int i=0; i < n; i++) {
        tmp_immunity = count_set_bits(indexes[i]);
        if (tmp_immunity != 0 && tmp_immunity < lowest_immunity) lowest_immunity = tmp_immunity;
    }
    
    return lowest_immunity-1;
}


void eval_bool_function(chromosome_t chrom, int* ham_w, int* cor_im) {
    decode_chromosome(chrom);
    // vector<int> truth_table = fill_table("0001000000001000000001000010000000000010010000001000000000000001000000011000000001000000000000100010000000000100000010000001000000001000000100000010000000000100010000000000001000000001100000001000000000000001000000100100000000000100001000000001000000001000");
    vector<int> truth_table = get_truth_table();

    vector<int> table = fwht(truth_table);
    vector<int> indexes = get_fwht_indexes(table);
    int hamming_weight = get_hamming_weight(truth_table);
    int correlation_immunity = get_correlation_immunity(indexes);
    // cout << "H: " << hamming_weight << " CI: " << correlation_immunity << endl;

    *ham_w = hamming_weight;
    *cor_im = correlation_immunity;
}
// END EVAL BOOL FUNCTION // ###############################################
// =========================================================================


// -------------------------------------------------------------------------
// CHROMOSOME // ###########################################################
vector<int> encode_chromosome() {
    vector<int> result;
    for (int i=0; i < FIRST_COL_SIZE; i++) {
        for (int j=0; j < FUNC_INPUT_SIZE; j++) {
            result.push_back(first_col[i].inputs[j]);
        }
        result.push_back(first_col[i].func);
    }

    for (int i=0; i < SEC_COL_SIZE; i++) {
        for (int j=0; j < FUNC_INPUT_SIZE; j++) {
            result.push_back(sec_col[i].inputs[j]);
        }
        result.push_back(sec_col[i].func);
    }

    for (int j=0; j < FUNC_INPUT_SIZE; j++) {
        result.push_back(last_node.inputs[j]);
    }
    result.push_back(last_node.func);

    return result;
}


void decode_chromosome(vector<int> chromosome) {
    for (int i=0; i < FIRST_COL_SIZE; i++) {
        for (int j=0; j < FUNC_INPUT_SIZE; j++) {
            first_col[i].inputs[j] = chromosome[i*(FUNC_INPUT_SIZE+1) + j];
        }
        first_col[i].func = (Function)chromosome[i*(FUNC_INPUT_SIZE+1) + FUNC_INPUT_SIZE];
    }

    for (int i=0; i < SEC_COL_SIZE; i++) {
        for (int j=0; j < FUNC_INPUT_SIZE; j++) {
            sec_col[i].inputs[j] = chromosome[i*(FUNC_INPUT_SIZE+1) + FIRST_COL_SIZE*(FUNC_INPUT_SIZE+1) + j];
        }
        sec_col[i].func = (Function)chromosome[i*(FUNC_INPUT_SIZE+1) + FIRST_COL_SIZE*(FUNC_INPUT_SIZE+1) + FUNC_INPUT_SIZE];
    }

    for (int j=0; j < FUNC_INPUT_SIZE; j++) {
        last_node.inputs[j] = chromosome[FIRST_COL_SIZE*(FUNC_INPUT_SIZE+1) + SEC_COL_SIZE*(FUNC_INPUT_SIZE+1) + j];
    }
    last_node.func = (Function)chromosome[FIRST_COL_SIZE*(FUNC_INPUT_SIZE+1) + SEC_COL_SIZE*(FUNC_INPUT_SIZE+1) + FUNC_INPUT_SIZE];
}

vector<int> generate_chromosome() {
    int rand_num;
    chromosome_t chromosome;
    
    for (int i=0; i < (FUNC_CNT)*(FUNC_INPUT_SIZE+1); i++) {
        rand_num = rand();
        // generate function
        if ((i+1) % (FUNC_INPUT_SIZE+1) == 0) {
            chromosome.push_back(rand_num % USABLE_FUNCTIONS);

        // generate inputs
        } else {
            // input from input vector - First column
            if (i < FIRST_COL_SIZE*(FUNC_INPUT_SIZE+1)) {
                chromosome.push_back((rand_num % INPUT_CNT) + 1);
            // input from First column - Second column
            } else if (i < (SEC_COL_SIZE + FIRST_COL_SIZE)*(FUNC_INPUT_SIZE+1)) {
                chromosome.push_back((rand_num % FIRST_COL_SIZE) + INPUT_CNT + 1);
            // input from Second column - Last node
            } else {
                chromosome.push_back((rand_num % SEC_COL_SIZE) + INPUT_CNT + FIRST_COL_SIZE + 1);
            }
        }
    }

    return chromosome;
}
// END CHROMOSOME // #######################################################
// =========================================================================


// -------------------------------------------------------------------------
// MUTATION // #############################################################
chromosome_t make_mutation(chromosome_t chrom) {
    int rand_pos;
    int rand_mut;
    int gens_num = (rand() % GENS_MUTATTIONS_NUM) + 1;

    for (int i=0; i < gens_num; i++) {
        rand_pos = rand() % ((FUNC_CNT) * (FUNC_INPUT_SIZE + 1));
        rand_mut = rand();

        // mutation of function
        if ((rand_pos + 1) % (FUNC_INPUT_SIZE + 1) == 0) {
            chrom[rand_pos] = rand_mut % USABLE_FUNCTIONS;

        // mutation of input
        } else {
            // first column - from input
            if (rand_pos / (FUNC_INPUT_SIZE + 1) < FIRST_COL_SIZE) {
                rand_mut = rand_mut % (INPUT_CNT + 1);

            // others - from blocks    
            } else {
                
                // set no input - only if not first input in block
                if ((rand_mut % (FUNC_INPUT_SIZE + 1) == FUNC_INPUT_SIZE) && (rand_pos % (FUNC_INPUT_SIZE + 1) != 0)) {
                    rand_mut = 0;
                // take input
                } else {
                    // second col - take from first col
                    if (rand_pos / (FUNC_INPUT_SIZE + 1) < (FIRST_COL_SIZE + SEC_COL_SIZE)) {
                        rand_mut = (rand_mut % FIRST_COL_SIZE) + (INPUT_CNT + 1);
                    // last node - take from second col
                    } else {
                        rand_mut = (rand_mut % SEC_COL_SIZE) + (INPUT_CNT + FIRST_COL_SIZE + 1);
                    }
                }
            }
            chrom[rand_pos] = rand_mut;
        }
    }
    return chrom;
}
// END MUTATION // #########################################################
// =========================================================================


// -------------------------------------------------------------------------
// EVOLUTION // ############################################################
void init_population() {
    population.clear();
    for (int i=0; i < POP_SIZE; i++) {
        population.push_back(generate_chromosome());
    }
}


int get_fitness(chromosome_t chrom, int required_cor_imm) {
    int fit_val = 0;
    int ham_w;
    int cor_im;
    eval_bool_function(chrom, &ham_w, &cor_im);

    // TODO 
    // FIXME

    fit_val += 256-ham_w;
    hw = ham_w;
    
    if (required_cor_imm - cor_im == 0) {
        fit_val += 500;
    } else {
        fit_val += 100 - abs(required_cor_imm - cor_im) * 20;
    }

    return fit_val;
}


void eval_population(int required_cor_imm, int* best_fit, int* best_fit_idx) {
    // eval chromosome
    int pop_best_fit = *best_fit;
    int pop_best_fit_idx = *best_fit_idx;
    for (int i=0; i < POP_SIZE; i++) {
        // skip actual best chromosome
        if (i == *best_fit_idx) continue;

        int local_fit = get_fitness(population[i], required_cor_imm);
        if (local_fit >= pop_best_fit) {
            pop_best_fit = local_fit;
            pop_best_fit_idx = i;
            // best_hw = 256 - (pop_best_fit - (pop_best_fit >= 500 ? 500 : 0));
            best_hw = hw;
        }

        // if (local_fit > pop_best_fit) {
        //     hw_reached_in = 0;
        // }
    }

    *best_fit = pop_best_fit;
    *best_fit_idx = pop_best_fit_idx;
}






void do_evolution(int required_cor_imm) {
    init_population();

    ci_reached_in = 0;
    int best_fit = 0;
    int best_fit_idx = 0;

    // initail evaluate of population
    for (int i=0; i < POP_SIZE; i++) {

        int local_fit = get_fitness(population[i], required_cor_imm);
        if (local_fit >= best_fit) {
            best_fit = local_fit;
            best_fit_idx = i;
            best_hw = 256 - (best_fit - 500);
        }
    }


    // cout << best_fit_idx << " " << best_fit << endl;
    // print_table(population[best_fit_idx], "BEST CHORMOSOME");


    
    // EVOLVE FOR MAX GENERATIONS-----------------------------------------
    for (int i=0; i < GENERATIONS_NUM; i++) {

        eval_population(required_cor_imm, &best_fit, &best_fit_idx);

        for (int i=0; i < POP_SIZE; i++) {
            // skip best chromosome
            if (i == best_fit_idx) continue;
        
            // MUTATE
            population[i] = make_mutation(population[i]);
        }

        if (i % LOG_ON_LINE == 0) {
            if (best_fit > 500) {
                cout << "Gen: " << i << "/" << GENERATIONS_NUM << " FIT: " << best_fit << " hw: " << best_hw << endl;
            } else {
                cout << "Gen: " << i << "/" << GENERATIONS_NUM << " FIT: " << best_fit << endl;
            }
        }

        if (ci_reached_in == 0) {
            if (best_fit >= 500) ci_reached_in = i;
        }
        // if (hw_reached_in == 0) {
        //     hw_reached_in = i;
        // }
        // print_table(population[best_fit_idx], "BEST CHORMOSOME");
    }
    // END EVOLUTION ALGORITHM----------------------------------------------

    res_best_hw.push_back(best_hw);
    res_best_chromosome.push_back(population[best_fit_idx]);

    cout << "BEST FIT: " << best_fit << " hw: " << best_hw << " CI reached in: " << ci_reached_in << endl;

    print_chromosome(population[best_fit_idx], "BEST CHROMOSOME");
}
// END EVOLUTION // ########################################################
// =========================================================================


void init_function_table() {
    Node tmp;

    tmp.func = (Function)0;

    // init First column with zeros
    for (int i=0; i < FIRST_COL_SIZE; i++) {
        for (int j=0; j < FUNC_INPUT_SIZE; j++) {
            tmp.inputs.push_back(0);
        }
        tmp.index = i + INPUT_CNT + 1;

        first_col.push_back(tmp);
    } 

    // init Second column with zeros
    tmp.inputs.clear();

    for (int i=0; i < SEC_COL_SIZE; i++) {
        for (int j=0; j < FUNC_INPUT_SIZE; j++) {
            tmp.inputs.push_back(0);
        }
        tmp.index = i + INPUT_CNT + FIRST_COL_SIZE + 1;

        sec_col.push_back(tmp);
    } 

    // init Last node with zeros
    tmp.inputs.clear();

    for (int j=0; j < FUNC_INPUT_SIZE; j++) {
            tmp.inputs.push_back(0);
        }
        tmp.index = INPUT_CNT + FIRST_COL_SIZE + SEC_COL_SIZE + 1;

    last_node = tmp;
}






int main(int argc, char** argv) {
    srand (time(NULL));

    init_function_table();

    int tmp_hw = 9999;
    int tmp_hw_idx = -1;
    for (int run=0; run < RUNS_NUM; run++) {
        cout << endl << "=================" << endl;
        cout << "---- RUN " << run << " ----" << endl;
        cout << "-----------------" << endl;
        
        do_evolution(REQUIRED_COR_IM);

        if (res_best_hw[run] < tmp_hw) {
            tmp_hw = res_best_hw[run];
            tmp_hw_idx = run;
        }
    }

    cout << endl << "=====================" << endl << "==== BEST RESULT ====" << "== on " << RUNS_NUM << " runs ==" << endl;
    cout << "Best HW: " << res_best_hw[tmp_hw_idx] << endl;
    print_chromosome(res_best_chromosome[tmp_hw_idx], "FINAL CHROMOSOME");

    // do_evolution(REQUIRED_COR_IM);




    return 0;
}