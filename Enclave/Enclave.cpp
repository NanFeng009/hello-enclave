#include "Enclave_t.h"
#include <string.h>
#include <stdio.h>

#define BUFSIZ 100

int o_printf(const char* fmt, ...)
{
    char buf[BUFSIZ] = { '\0' };
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, BUFSIZ, fmt, ap);
    va_end(ap);
    ocall_print(buf);
    return (int)strnlen(buf, BUFSIZ - 1) + 1;
}


int generate_random_number() {
//    __asm (
//            "lfence"
//          );
    ocall_print("Processing random number generation...");
    return 42;
}
