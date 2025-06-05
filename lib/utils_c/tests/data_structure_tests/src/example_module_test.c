#include "../include/example_module_test.h"
#include "../include/example_module.h"
#include <stdio.h>

int test_Error_1() {

    module_error mod_error = function_3(1,'+',1);    

    if(mod_error.ok) {
        printf("Test Success!\n");
        return 0;
    }
    else {
        printf("Test Failed:\n");
        switch (mod_error.type) {
            case error_1 :
                printf("Error 1 with value %d\n", CATCH(int,mod_error));
                break;
            case error_2 :
                printf("Error 2 with value '%c'\n", CATCH(char,mod_error));
                break;
            case error_3 :
                printf("Error 3 with value %d\n", CATCH(int,mod_error));
                break;
            case error_4 :
                printf("Error 4 with value %d\n", CATCH(int,mod_error));
                break;
        };
        return 1;
    }
}
