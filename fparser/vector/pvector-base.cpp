#include "pvector-base.hpp"

#include <stdlib.h>


PVectorBase::PVectorBase() : 
    array(this_array),
    array_size(this_array_size),
    N(this_N)
{
    array = (void**)malloc(DEFAULT_SIZE * sizeof(void*));
    array_size = DEFAULT_SIZE;
    N = 0;
}

PVectorBase::PVectorBase(const PVectorBase& other) :
    array(other.array),
    array_size(other.array_size),
    N(other.N)
{}

PVectorBase::~PVectorBase(){
}

int PVectorBase::getSize() {
    return N;
}

void* PVectorBase::get(int n) {
    if(n < 0 || n >= N) {return nullptr;}
    return array[n];
}

void PVectorBase::add(void* ptr) {
    if(N == array_size) {
        array = (void**)realloc(array, (array_size+1)*sizeof(void*));
        array_size++;
    }
    array[N] = ptr;
    N++;
}

void* PVectorBase::createAndAdd(void* arg) {
    void* ptr = new_(arg); 
    if(ptr) {add(ptr);}
    return ptr;
}

void PVectorBase::remove(int n) {
    if(n < 0 || n >= N) {return;}
    delete_(array[n]);
    N--;
    for(int i = n; i < N; i++) {
        array[i] = array[i+1];
    }
}

void PVectorBase::clear() {
    while(N>0) {
        remove(N-1);
    }
}

void* PVectorBase::new_(void* arg) {
    return nullptr;
}

void PVectorBase::delete_(void* ptr) {
    return;
}
