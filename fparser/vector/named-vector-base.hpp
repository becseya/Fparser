#pragma once

#include "pvector.hpp"
#include "named.hpp"

class NamedPVectorBase : public PVector<NamedClass> {
    char name_buffer[NamedClass::NAME_SIZE+1];
    const char* name_base;

    struct name_and_arg_t {
        const char* name;
        void* arg;
    };

public:
    NamedPVectorBase(const char* name_base = "id");
    virtual ~NamedPVectorBase();

    /*NamedClass* get(int n);
    void add(NamedClass* ptr);
    NamedClass* createAndAdd(void* arg = nullptr);*/
    const char* getName(int n);
    int getNByName(const char* name);
    NamedClass* getByName(const char* name);

private:
    const char* generateName();
    void* new_(void* arg_) final;

protected:
    virtual NamedClass* new_named_(const char* name, void* arg) {return nullptr;} 
};
