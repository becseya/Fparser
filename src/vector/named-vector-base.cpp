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
    NamedClass* ptr = get(n);
    if(ptr) {return ptr->getName();}
    else {return "";}
}

int NamedPVectorBase::getNByName(const char* name) {
    if(!name || (*name == '\0')) {return -1;}
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

void NamedPVectorBase::removeByName(const char* name){
    int i = getNByName(name);
    if(i >= 0) {remove(i);}
}

const char* NamedPVectorBase::generateName() {
    do {
        sprintf(name_buffer, "%s-%04x", name_base, (rand() & 0xFFFF));
    } while(getByName(name_buffer));

    return name_buffer;
}

NamedClass* NamedPVectorBase::createAndAddNamed(const char* name, void* arg) {
    NamedClass* ptr;

    if(name && *name) {
        ptr = new_named_(name, arg); 
    }
    else {
        ptr = new_named_(generateName(), arg);
    }
    
    if(ptr) {add(ptr);}
    return ptr;
}

void* NamedPVectorBase::new_(void* arg) {
    return new_named_(generateName(), arg);
}
