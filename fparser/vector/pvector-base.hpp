#pragma once

class PVectorBase {
    static const int DEFAULT_SIZE = 1;
    void**& array;
    int& array_size;
    int& N;

    void** this_array;
    int this_array_size;
    int this_N;
public:
    PVectorBase();
    PVectorBase(const PVectorBase&);
    virtual ~PVectorBase();

    int getSize();
    void* get(int n);
    void add(void* ptr);
    void* createAndAdd(void* arg = nullptr);
	void remove(int n);
    void removelast();
    void clear();

protected:
    virtual void* new_(void* arg);
    virtual void delete_(void* ptr);

private:
    PVectorBase& operator=(const PVectorBase&);
};
