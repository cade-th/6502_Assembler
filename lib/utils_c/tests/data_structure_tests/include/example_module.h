#include "../../../include/error.h"

typedef enum {
	error_1,
	error_2,
	error_3,
	error_4,
} module_error_t;
ERROR(module_error_t) module_error;

module_error function_3(int function_1_val, char function_2_val, int function_3_val);
int function_1(int input);
int function_2(char input);
