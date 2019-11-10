#pragma once

class NamedClass {
public:
    static const int NAME_SIZE = 32;
private:
    char name[NAME_SIZE+1];


public:
    NamedClass(const char* name_);
    virtual ~NamedClass();
    
    const char* getName();
};
