#include "../include/example_module.h"
#include <stdlib.h>

module_error mod_error = { .type = error_1, .ok = true, .data = NULL };

int function_1(int input) {
    // simulate failure
    if (input != 1 ) {
        mod_error.type = error_1;
        THROW(mod_error,int,input);
        return input; 
    }   
    else {
        return input;
    }
}

int function_2(char input) {
    if (input != '+' ) {
        mod_error.type = error_2;
        THROW(mod_error,int,input);
        return input;
    } else {
        return input;
    }
    
}

module_error function_3(int function_1_val, char function_2_val, int function_3_val) {

    int get_thing_1 = function_1(function_1_val);
    if (!mod_error.ok) {
        return mod_error;
    }
    int get_thing_2 = function_2(function_2_val);
    if (!mod_error.ok) {
        return mod_error;
    }
    if (function_3_val != 1 ) {
        mod_error.type = error_4;
        THROW(mod_error,int,function_3_val);
        return mod_error;
    }
    else {
        // not returning anything in the successfull case here but you can if you want 
        return mod_error;
    }
}


