#pragma once

#include "pvector.hpp"
#include "named-vector-base.hpp"


template<class T>
class NamedPVector : public NamedPVectorBase {

public:
    NamedPVector(const char* name_base = "id") :
        NamedPVectorBase(name_base)
    {}


    T* get(int n) {
        return (T*)NamedPVectorBase::get(n);
    }

    void add(T* ptr) {
        NamedPVectorBase::add(ptr);
    }

    T* getByName(const char* name) {
        return (T*)NamedPVectorBase::getByName(name);
    }

    T* createAndAdd(void* arg = nullptr) {
        return (T*)NamedPVectorBase::createAndAdd(arg);
    }

    T* createAndAddNamed(const char* name, void* arg = nullptr) {
        return (T*)NamedPVectorBase::createAndAddNamed(name, arg);
    }

    template<class U, typename = typename std::enable_if<std::is_base_of<U, T>::value>::type>
    operator NamedPVector<U>&() {
        return *(NamedPVector<U>*)this;
    }

protected:
    NamedClass* new_named_(const char* name, void* arg) final {
        return new T(name);
    }

    void delete_(void* ptr) final {
        delete (T*)ptr;
    }
};
