#include "arg.hpp"

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>

void ParseArg::stepIn() {
    if(parse_depth == -1) {
        depth = -1;
        err = false;
        err_not_found = false;
        err_set = false;
        err_read_only = false;
        set_ok = false;
        arg_msg[ERR_MSG_SIZE] = '\0';
        arg_stack[ERR_STACK_SIZE] = '\0';
        strcpy(arg_msg, "");
        strcpy(arg_stack, "");
    }
    parse_depth++;
}

void ParseArg::stepOut(const char* name) {
    parse_depth--;
    if(err && name) {stackAdd(name);}
}

void ParseArg::errWrite(const char* msg, const char* stack) {
    if(!err) {strcpy(arg_msg, msg);}
    if(stack) {stackAdd(stack);}
    err = true;
}

void ParseArg::stackAdd(const char* add) {
    if(arg_stack[0] != '\0') {strcat(arg_stack, " ");}

    const char* end = add;
    while(*end && !strchr(".: ,}]", *end)) {end++;}
    size_t size = end-add;

    if(size + strlen(arg_stack) < ERR_STACK_SIZE) {
        strncat(arg_stack, add, size);
    }
}
