#include "named-vector-base.hpp"

#include <string.h>
#include <stdio.h>
#include <cstdlib>

NamedPVectorBase::NamedPVectorBase(const char* name_base) : 
    name_base(name_base)
{}

NamedPVectorBase::~NamedPVectorBase(){
}

/*NamedClass* NamedPVectorBase::get(int n) {
    return (NamedClass*)PVectorBase::get(n);
}

void NamedPVectorBase::add(NamedClass* ptr) {
    PVectorBase::add(ptr);
}

NamedClass* NamedPVectorBase::createAndAdd(void* arg) {
    return (NamedClass*)PVectorBase::createAndAdd(arg);
}*/

const char* NamedPVectorBase::getName(int n) {
    return get(n)->getName();
}

int NamedPVectorBase::getNByName(const char* name) {
    for(int i = 0; i < getSize(); i++) {
        if(strcmp(name, getName(i)) == 0){
            return i;
        }
    }
    return -1;
}

NamedClass* NamedPVectorBase::getByName(const char* name) {
    int i = getNByName(name);
    if(i < 0) {return nullptr;}
    else {return get(i);}
}

const char* NamedPVectorBase::generateName() {
    do {
        sprintf(name_buffer, "%s-%0x", name_base, (rand() & 0xFFFF));
    } while(getByName(name_buffer));

    return name_buffer;
}

void* NamedPVectorBase::new_(void* arg_) {
    name_and_arg_t name_and_arg;
    name_and_arg.name = generateName();
    name_and_arg.arg = arg_;

    return new_named_(name_and_arg.name, name_and_arg.arg);
}
