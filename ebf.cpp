#include "ebf.h"


using namespace std;

Node last_node;
vector<Node> first_col;
vector<Node> sec_col;


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


// TODO
// FIXME
bool eval_values(vector<int> indexes, vector<bool> vals, Function func) {
    switch (func)
    {
    case fand:
        return vals[0] & vals[1];
        break;
    case f_or:
        cout << " || ";
        break;
    case fxor:
        cout << " xor ";
        break;
    case fnot:
        cout << " !";
        break;
    case fnand:
        cout << " !&";
        break;
    case fnor:
        cout << " !|";
        break;
    
    default:
        break;
    }

}


bool eval_input(int input_int) {
    vector<bool> inputs;
    bool tmp;

    // get 8 binary values from 1 int
    for (int i=0; i < 8; i++) {
        tmp = input_int & 0x01;
        inputs.push_back(tmp);
        input_int = input_int >> 1;
    } 

    // evaluate first column
    vector<bool> first_col_vals;
    for (int i=0; i < INPUT_CNT; i++) {
        first_col_vals.push_back(eval_values(first_col[i].inputs, inputs, first_col[i].func));
    }

    vector<bool> second_col_vals;
    for (int i=0; i < FIRST_COL_SIZE; i++) {
        second_col_vals.push_back(eval_values(sec_col[i].inputs, first_col_vals, sec_col[i].func));
    }

    bool res;
    for (int i=0; i < SEC_COL_SIZE; i++) {
        res = eval_values(last_node.inputs, second_col_vals, last_node.func);
    }

    return res;
} 


vector<bool> get_eval_table() {
    vector<bool> eval_table;
    for (int i=0; i < 256; i++) {
        eval_table.push_back(eval_input(i));
    }

    return eval_table;
}


void init_function_table() {
    Node tmp;
    tmp.func = fand;
    tmp.inputs.push_back(1);
    tmp.inputs.push_back(2);
    tmp.inputs.push_back(3);
    tmp.inputs.push_back(4);
    
    for (int i=INPUT_CNT+1; i <= INPUT_CNT+FIRST_COL_SIZE; i++) {
        tmp.index = i;
        first_col.push_back(tmp);
    }

    tmp.func = f_or;
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

    return 0;
}