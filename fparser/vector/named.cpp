#include "named.hpp"

#include <string.h>

NamedClass::NamedClass(const char* name_) {
    name[NAME_SIZE] = '\0';
    strncpy(name, name_, NAME_SIZE);
}

NamedClass::~NamedClass() {
}

const char* NamedClass::getName() {
    return name;
}
