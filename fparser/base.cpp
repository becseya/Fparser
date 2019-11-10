#include "base.hpp"

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>


FieldBase::FieldBase(const char* name, uint32_t flags, bool& set_flag) : 
    name(name),
    flags(flags),
    set_flag(set_flag)
{}

FieldBase::~FieldBase() {}

const char* FieldBase::getName() {
    return name;
}

uint32_t FieldBase::getFlags() {
    return flags;
}

int FieldBase::set(const char* in, ParseArg* arg){
    int st;
    
    st = set_override(in, arg);
    if(st >= 0) {
        flags |= FLAG_SET_CHANGED;
        if (!(flags & FLAG_NOT_EXPORTABLE)) {
            flags |= FLAG_SET_CHANGED_EXPORTABLE;
        }

        if(&set_flag) {set_flag = true;}
    }

    return st;
}

int FieldBase::get(char* out, bool dense, int print_depth, uint32_t flags) {
    return get_override(out, dense, print_depth, flags);
}

int FieldBase::parse(const char* in, char* out, ParseArg& arg) {
    int st;

    // root
    if(arg.depth == -1) {
        strcpy(out, "");
        if(*in == '!') {in++; arg.dense=true;}
        else {arg.dense = false;}
    }
    
    // step in
    arg.stepIn();
    if(*in == '.') {in++;}


    // parse set and get
    if((*in == ':') || compNext(in, "set")) {
        st = set(++in, &arg);
        if(st < 0 && !arg.err) {arg.errWrite("Failed command: set", in);}
        else {arg.set_ok = true;}
    }
    else if((*in == '\0') || compNext(in, "get")) {
        st = get(out, arg.dense, 0);            
        if(st < 0) {arg.errWrite("Failed command: get", in);}
    }
    else if(compNext(in, "export")) {
        st = get(out, arg.dense, 0, FLAG_NOT_EXPORTABLE);           
        if(st < 0) {arg.errWrite("Failed command: get", in);}
    }
    else {
        st = parse_override(in, out, arg);
        if((st < 0) && !arg.err) {arg.errWrite("Command not found", in);}
    }

    // final return
    if(arg.depth == 0) {printOutput(out, arg);}
    
    // step out
    arg.stepOut(name);

    return st;
}

int FieldBase::parse_override(const char* in, char* out, ParseArg& arg) {
    return -1;
}

bool FieldBase::compNext(const char*& src, const char* cmp) {
    if(*src == '\0') {return false;}
    const char* orig = src;

    while(*src && *cmp) {
        if(*(src++) != *(cmp++)) {src = orig; return false;}  
    }
    skipWhiteSpace(src);
    if((*cmp == '\0') && ((*src == '\0') || strchr(".: \",}]", *src))) {return true;}
    else {src = orig; return false;}
}

void FieldBase::skipWhiteSpace(const char*& c) {
    while((*c == ' ') || (*c == '\t') || (*c == '\n') || (*c == '\r')) {c++;}
}

void FieldBase::printNewLine(char* str, int depth, bool comma) {
    if(comma) {strcat(str, ",");}
    if(!(depth < 0)) {
        strcat(str, "\n");
        for (int i = 0; i < depth; i++) {strcat(str, "   ");}
    }
}

void FieldBase::printOutput(char* out, ParseArg& arg) {
    if(arg.err) {
        strcat(out, "{");
        strcat(out, "\"status\": \"error\"");
        printNewLine(out, (arg.dense ? -1 : arg.depth+1), true);
        sprintf(out+strlen(out), "\"arg_msg\": \"%s\"", arg.arg_msg);
        printNewLine(out, (arg.dense ? -1 : arg.depth+1), true);
        sprintf(out+strlen(out), "\"arg_stack\": \"%s\"", arg.arg_stack);
        strcat(out, "}");
    }
    else if(arg.set_ok) {
        strcat(out, "{");
        strcat(out, "\"status\": \"ok\"");
        strcat(out, "}");
    }
}
