#include "common.h"
#include "error.h"

void error_handler(char function[256], char error_text[256], bool exit_flag) {
        printf("*** VBS Error: %s in function %s\n", error_text, function);
        if (exit_flag)
                exit(1);
}

