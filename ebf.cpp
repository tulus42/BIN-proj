#include "ebf.h"


using namespace std;
using chromosome_t = vector<int>;

Node last_node;
vector<Node> first_col;
vector<Node> sec_col;

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

    bool actual_val = vals[0];
    for (int i=1; i < indexes.size(); i++) {
        actual_val = eval_single_value(actual_val, vals[i], func);
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


vector<int> fill_table(string input) {
    vector<int> table;
    int n = input.size();
    for (int i=0; i < n; i++) {
        table.push_back(input[i] == '0' ? 0 : 1);
    }

    return table;
}


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
    table = create_input_vector_for_fwht(table);
    print_table(table, "FWHT input vector");


    int n = table.size();
    for(int m = 1; 2 * m <= n; m *= 2) {
        for(int i = 0; i < n; i += 2 * m) {
            for(int j = 0; j < m; ++j) {
                auto x = table[i + j];
                auto y = table[i + j + m];
                
                    // table[i + j] = y;
                    // table[i + j + m] = x + y;
                                        
                    table[i + j] = -x + y;
                    table[i + j + m] = x;
            }
        }
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
    int lowest_immunity = 9999;
    int tmp_immunity;
    int n = indexes.size();
    for (int i=0; i < n; i++) {
        tmp_immunity = count_set_bits(indexes[i]);
        if (tmp_immunity != 0 && tmp_immunity < lowest_immunity) lowest_immunity = tmp_immunity;
    }
    
    return lowest_immunity-1;
}


void eval_bool_function() {
    // vector<int> truth_table = fill_table("0001000000001000000001000010000000000010010000001000000000000001000000011000000001000000000000100010000000000100000010000001000000001000000100000010000000000100010000000000001000000001100000001000000000000001000000100100000000000100001000000001000000001000");
    vector<int> truth_table = get_truth_table();

    vector<int> table = fwht(truth_table);
    vector<int> indexes = get_fwht_indexes(table);
    int hamming_weight = get_hamming_weight(truth_table);
    int correlation_immunity = get_correlation_immunity(indexes);
    cout << "H: " << hamming_weight << " CI: " << correlation_immunity << endl;
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

    // show_function();

    chromosome_t chrom = generate_chromosome();
    print_table(chrom, "CHROMOSOME");

    decode_chromosome(chrom);

    vector<int> table = get_truth_table();
    //vector<int> table = fill_table("0001000000001000000001000010000000000010010000001000000000000001000000011000000001000000000000100010000000000100000010000001000000001000000100000010000000000100010000000000001000000001100000001000000000000001000000100100000000000100001000000001000000001000");
    // vector<int> table = fill_table("10100110");

    print_table(table, "Truth Table");

    table = fwht(table);

    // print_table(table, "FWHT");

    vector<int> indexes = get_fwht_indexes(table);

    // print_table(indexes, "INDEXES");

    eval_bool_function();

    // print_table(encode_chromosome(), "CHROMOSOME");


    return 0;
}