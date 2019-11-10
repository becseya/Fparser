#pragma once

#include "pvector.hpp"
#include "named-vector-base.hpp"


template<class T>
class NamedPVector : public NamedPVectorBase {

public:
    T* get(int n) {
        return (T*)NamedPVectorBase::get(n);
    }

    void add(T* ptr) {
        NamedPVectorBase::add(ptr);
    }

    T* createAndAdd(void* arg = nullptr) {
        return (T*)NamedPVectorBase::createAndAdd(arg);
    }

    template<class U, typename = typename std::enable_if<std::is_base_of<U, T>::value>::type>
    operator NamedPVector<U>&() {
        return *(NamedPVector<U>*)this;
    }

protected:
    NamedClass* new_named_(const char* name, void* arg) final {
        return new_named_static(name, arg);
    }

    static NamedClass* new_named_static(const char* name, void* arg) {
        return new T(name);
    }

    void delete_(void* ptr) final {
        delete_static(ptr);
    }

    static void delete_static(void* ptr) {
        delete (T*)ptr;
    }
};
