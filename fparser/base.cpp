#include "base.hpp"

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>


NodeBase::NodeBase(const char* name, int flags, bool* set_flag) : 
    NamedClass(name),
    type(FIELD),
    flags(flags),
    set_flag(set_flag)
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
    return -1;
}

int NodeBase::get_override(char* out, void* arg){
    return -1;
}

int NodeBase::set_override(const char* in, void* arg){
    return -1;
}
