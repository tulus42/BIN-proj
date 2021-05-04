#include "ebf.h"


using namespace std;

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


bool eval_values(vector<int> indexes, vector<bool> vals, Function func) {
    if (func == fnone) return 0;

    bool actual_val = vals[0];
    for (int i=1; i < indexes.size(); i++) {
        actual_val = eval_single_value(actual_val, vals[i], func);
    }

    return actual_val;

}


bool eval_input(int input_int) {
    vector<bool> inputs;
    bool tmp;

    // get 8 binary values from 1 int
    for (int i=0; i < 8; i++) {
        tmp = input_int & 0x01;
        inputs.insert(inputs.begin(), tmp);
        input_int = input_int >> 1;
    } 

    // evaluate first column
    vector<bool> first_col_vals;
    for (int i=0; i < FIRST_COL_SIZE; i++) {
        first_col_vals.push_back(eval_values(first_col[i].inputs, inputs, first_col[i].func));
    }

    vector<bool> second_col_vals;
    for (int i=0; i < SEC_COL_SIZE; i++) {
        second_col_vals.push_back(eval_values(sec_col[i].inputs, first_col_vals, sec_col[i].func));
    }

    bool res;
    
    res = eval_values(last_node.inputs, second_col_vals, last_node.func);
    
    return res;
} 


vector<bool> get_truth_table() {
    vector<bool> truth_table;
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
    for (int i=0; i < 256; i++) {
        table[i] = table[i] == 0 ? -1 : 1;
    }

    return table;
}

vector<int> fwht(vector<int> table) {
    table = create_input_vector_for_fwht(table);
    print_table(table, "FWHT input vector");

    int h = 1;
    int tab_size = table.size();
    bool x;
    bool y;
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



vector<int> fill_table(string input) {
    vector<int> table;
    for (int i=0; i < 256; i++) {
        table.push_back(input[i] == '0' ? 0 : 1);
    }

    return table;
}

void init_function_table() {
    Node tmp;
    tmp.func = f_or;
    tmp.inputs.push_back(1);
    tmp.inputs.push_back(4);
    tmp.inputs.push_back(6);
    tmp.inputs.push_back(8);
    
    for (int i=INPUT_CNT+1; i <= INPUT_CNT+FIRST_COL_SIZE; i++) {
        tmp.index = i;
        first_col.push_back(tmp);
    }

    tmp.func = fand;
    tmp.inputs.pop_back();
    tmp.inputs.pop_back();
    tmp.inputs.pop_back();
    tmp.inputs.pop_back();
    tmp.inputs.push_back(9);
    tmp.inputs.push_back(10);
    tmp.inputs.push_back(11);
    tmp.inputs.push_back(12);

    for (int i=INPUT_CNT+FIRST_COL_SIZE+1; i <= INPUT_CNT+FIRST_COL_SIZE+SEC_COL_SIZE; i++) {
        tmp.index = i;
        sec_col.push_back(tmp);
    }

    last_node.index = INPUT_CNT + FIRST_COL_SIZE + SEC_COL_SIZE + 1;
    last_node.func = fxor;
    last_node.inputs.push_back(13);
    last_node.inputs.push_back(14);

}


int main(int argc, char** argv) {
    init_function_table();

    show_function();

    //vector<bool> table = get_truth_table();
    vector<int> table = fill_table("0001000000001000000001000010000000000010010000001000000000000001000000011000000001000000000000100010000000000100000010000001000000001000000100000010000000000100010000000000001000000001100000001000000000000001000000100100000000000100001000000001000000001000");

    print_table(table, "Truth Table");

    table = fwht(table);

    print_table(table, "FWHT");


    return 0;
}