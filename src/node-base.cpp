#include "node-base.hpp"

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>

#define compx(x,y) (strncmp(x,y,strlen(y))==0)

NodeBase::NodeBase(const char* name, int flags, bool* set_flag) : 
    NamedClass(name),
    type(FIELD),
    flags(flags),
    set_flag(set_flag)
{}

NodeBase::NodeBase(const char* name, int flags, field_type_t type) :
    NamedClass(name),
    type(type),
    flags(flags),
    set_flag(nullptr)
{}

NodeBase::~NodeBase() {}

int NodeBase::getFlags() {
    return flags;
}

bool NodeBase::isExportable() {
    return !(flags & FLAG_NOT_EXPORTABLE);
}

bool NodeBase::isVisible() {
    return !(flags & FLAG_NOT_VISIBLE);
}

int NodeBase::set(const char* in, SetArgs* arg_, bool import) {
    SetArgs arg_def;
    SetArgs* arg = arg_ ? (SetArgs*)arg_ : &arg_def;
    if(import) {arg->import = true;}
    int st;

    if(flags & FLAG_READ_ONLY) { // return if read only
        arg->err_read_only = true;
        return -1;
    }

    st = set_override(in, arg);
    if(st >= 0) {
        if(set_flag && !import) {*set_flag = true;} // set change flag
    }
    else {
        arg->err_set = true;
    }

    return st;
}

int NodeBase::get(char* out, GetArgs* arg_) {
    GetArgs arg_def;
    GetArgs* arg = arg_ ? (GetArgs*)arg_ : &arg_def;

    return get_override(out, arg);
}

int NodeBase::parse(const char* in, char* out, ParseArg* arg_) {
    ParseArg arg_def;
    ParseArg* arg = arg_ ? (ParseArg*)arg_ : &arg_def;

    skipWhiteSpace(in);
    if(*in == '!') {in++;}

    if(*in == '\0' || (strcmp(in, "get") == 0) || (compx(in, "get") && isWhitespace(in+3))) {
        return get(out, arg);
    }
    else if(compx(in, ":")) {
        in+=1;
        skipWhiteSpace(in);
        return set(in, arg);
    }   
    else if(compx(in, "set:")) {
        in+=4;
        skipWhiteSpace(in);
        return set(in, arg);
    }
    return parse_override(in, out, arg);
}

int NodeBase::get_override(char* out, GetArgs* arg){
    return -1;
}

int NodeBase::set_override(const char* in, SetArgs* arg){
    return -1;
}

int NodeBase::parse_override(const char* in, char* out, ParseArg* arg){
    return -1;
}

bool NodeBase::isWhitespace(const char* c){
    return ((*c == ' ') || (*c == '\t') || (*c == '\n') || (*c == '\r'));
}

void NodeBase::skipWhiteSpace(const char*& c){
    while(isWhitespace(c)) {c++;}
}