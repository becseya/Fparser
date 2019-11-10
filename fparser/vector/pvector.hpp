#pragma once

#include "pvector-base.hpp"
#include <type_traits>

template<class T>
class PVector : public PVectorBase {
public:
    T* get(int n) {
        return (T*)PVectorBase::get(n);
    }

    void add(T* ptr) {
        PVectorBase::add(ptr);
    }

    template<class U, typename = typename std::enable_if<std::is_convertible<T*, U*>::value>::type>
    operator PVector<U>&() {
        return *(PVector<U>*)this;
    }
};

template<class T>
class DynamicPVector : public PVector<T> {

public:
    T* createAndAdd(void* arg = nullptr) {
        return (T*)PVector<T>::createAndAdd(arg);
    }

    template<class U, typename = typename std::enable_if<std::is_base_of<U, T>::value>::type>
    operator DynamicPVector<U>&() {
        return *(DynamicPVector<U>*)this;
    }
    
private:
    void* new_(void* arg) final {
        return new_static(arg);
    }

    void* new_static(void* arg) {
        return new T;
    }

    void delete_(void* ptr) final {
        delete_static(ptr);
    }

    static void delete_static(void* ptr) {
        delete (T*)ptr;
    }
};
