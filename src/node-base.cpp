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

int NodeBase::set(const char* in, void* arg, bool import){
    int st;
    
    if(flags & FLAG_READ_ONLY) {return -1;} // return if read only

    st = set_override(in, arg);
    if(st >= 0) {
        if(set_flag && !import) {*set_flag = true;} // set change flag
    }

    return st;
}

int NodeBase::get(char* out, void* arg) {
    return get_override(out, arg);
}

int NodeBase::parse(const char* in, char* out, void* arg) {
    if(*in == '\0' || (strcmp(in, "get") == 0)) {
        return get(out, arg);
    }
    else if(compx(in, ":")) {
        return set(in+1, arg);
    }   
    else if(compx(in, "set:")) {
        return set(in+4, arg);
    }
    return parse_override(in, out, arg);
}

int NodeBase::get_override(char* out, void* arg){
    return -1;
}

int NodeBase::set_override(const char* in, void* arg){
    return -1;
}

int NodeBase::parse_override(const char* in, char* out, void* arg){
    return -1;
}