
#include "fparser/vector/pvector.hpp"
#include "fparser/vector/named-vector.hpp"
#include <gtest/gtest.h>

int st;
PVector<int> iv;

TEST(VectorTest, Init) { 
    ASSERT_EQ(iv.get(-1), nullptr);
    ASSERT_EQ(iv.get(0), nullptr);
    ASSERT_EQ(iv.getSize(), 0);
}



int int1, int2;

TEST(VectorTest, Add) {
    int1 = 1;
    int2 = 5;
    iv.add(&int1);

    ASSERT_EQ(iv.getSize(), 1);
    ASSERT_EQ(iv.get(0), &int1);
    ASSERT_EQ(iv.get(1), nullptr);
    ASSERT_EQ(*iv.get(0), 1);

    iv.add(&int2);
    ASSERT_EQ(iv.getSize(), 2);
    ASSERT_EQ(iv.get(0), &int1);
    ASSERT_EQ(iv.get(1), &int2);
    ASSERT_EQ(*iv.get(0), 1);
    ASSERT_EQ(*iv.get(1), 5);
    *iv.get(1) = 0;
    ASSERT_EQ(*iv.get(1), 0);
}



struct A {
    virtual ~A() {}
    int number_a;
    virtual int val() {
        return 1;
    }
};

struct  B : virtual public A {
    virtual ~B() {}
    int number_b;
    int val() override {
        return 2;
    }
};

int calc_sum(PVector<A>& v) {
    int sum = 0;
    for(int i = 0; i < v.getSize(); i++) {
        sum += v.get(i)->val();
    }
    return sum;
}

A a;
B b;
PVector<B> bv;
PVector<A> av;

TEST(VectorTest, Inherit) { 
    bv.add(&b);
    ASSERT_EQ(bv.get(0)->val() , 2);
    st = calc_sum(bv);
    ASSERT_EQ(st , 2);

    bv.add(&b);
    ASSERT_EQ(bv.get(1)->val() , 2);
    st = calc_sum(bv);
    ASSERT_EQ(st , 4);

    av.add(&a);
    av.add(&b);
    ASSERT_EQ(av.get(0)->val() , 1);
    ASSERT_EQ(av.get(1)->val() , 2);
    st = calc_sum(av);
    ASSERT_EQ(st , 3);
}




DynamicPVector<B> dbv;
DynamicPVector<A> dav;

void remove_last_and_add(PVectorBase& v) {
    v.remove(v.getSize()-1);
    void* ptr = v.createAndAdd();
    ASSERT_NE(ptr, nullptr);
}

TEST(VectorTest, Dynamic) {
    B* bptr;
    A* aptr;
    int size_before;

    bptr = new B;
    dbv.add(bptr);
    bptr = dbv.createAndAdd();
    st = calc_sum(dbv);
    ASSERT_EQ(st , 4);

    size_before = dbv.getSize();
    remove_last_and_add(dbv);
    ASSERT_EQ(dbv.getSize() , size_before);
    ASSERT_EQ(st , 4);

    aptr = dav.createAndAdd();
    aptr = new B;
    dav.add(aptr);
    ASSERT_EQ(dav.get(0)->val(), 1);
    ASSERT_EQ(dav.get(1)->val(), 2);
    st = calc_sum(dav);
    ASSERT_EQ(st , 3);

    size_before = dav.getSize();
    remove_last_and_add(dav);
    ASSERT_EQ(dav.getSize() , size_before);
    ASSERT_EQ(dav.get(0)->val(), 1);
    ASSERT_EQ(dav.get(1)->val(), 1);
    st = calc_sum(dav);
    ASSERT_EQ(st , 2);

    dav.clear();
    dbv.clear();
}



struct NA : public NamedClass, virtual public A {
    NA(const char* name): NamedClass(name) {} 
private:
    NA(const NA&);
    NA& operator=(const NA&);
};

struct NB : public NA, public B {
    NB(const char* name): NA(name) {}
private:
    NB(const NB&);
    NB& operator=(const NB&);
};

TEST(VectorTest, Diamond) {
    NB* nbptr = new NB("B");
    NA* naptr = new NA("A");
    NA* naptr1 = nbptr;
    B* bptr = nbptr;
    A* aptr = naptr;
    A* aptr1 = nbptr;
    A* aptr2 = bptr;

    ASSERT_STREQ(nbptr->getName(), "B");
    ASSERT_EQ(nbptr->val(), 2);
    ASSERT_STREQ(naptr->getName(), "A");
    ASSERT_EQ(naptr->val(), 1);

    ASSERT_STREQ(naptr1->getName(), "B");
    ASSERT_EQ(naptr1->val(), 2);

    ASSERT_EQ(aptr->val(), 1);
    ASSERT_EQ(bptr->val(), 2);
    ASSERT_EQ(aptr1->val(), 2);
    ASSERT_EQ(aptr2->val(), 2);

    delete naptr;
    delete nbptr;
}




void print_name(NamedPVectorBase& v) {
    for(int i = 0; i < v.getSize(); i++){
        printf("%s\n", v.getName(i));
    }
}

int calc_sum_named(NamedPVector<NA>& v) {
    int sum = 0;
    for(int i = 0; i < v.getSize(); i++){
        sum += v.get(i)->val();
    }
    return sum;
}

NamedPVector<NB> nbv;
NamedPVector<NA> nav;

TEST(VectorTest, Named) {
    srand (time(NULL));
    NA* naptr;
    NB* nbptr;
    int size_before;

    nbptr = nbv.createAndAdd();
    nbptr = new NB("first");
    nbv.add(nbptr);
    ASSERT_EQ(nbv.get(0)->val(), 2);
    ASSERT_EQ(nbv.get(1)->val(), 2);
    ASSERT_STREQ(nbv.getName(1), "first");
    st = calc_sum_named(nbv);
    ASSERT_EQ(st , 4);

    size_before = nbv.getSize();
    remove_last_and_add(nbv);
    ASSERT_EQ(nbv.getSize() , size_before);
    ASSERT_EQ(nbv.get(0)->val(), 2);
    ASSERT_EQ(nbv.get(1)->val(), 2);
    ASSERT_STRNE(nbv.getName(1), "first");
    st = calc_sum_named(nbv);
    ASSERT_EQ(st , 4);

    naptr = nav.createAndAdd();
    naptr = new NB("first");
    nav.add(naptr);
    ASSERT_EQ(nav.get(0)->val(), 1);
    ASSERT_EQ(nav.get(1)->val(), 2);
    ASSERT_STREQ(nav.getName(1), "first");
    st = calc_sum_named(nav);
    ASSERT_EQ(st , 3);

    size_before = nav.getSize();
    remove_last_and_add(nav);
    ASSERT_EQ(nav.getSize() , size_before);
    ASSERT_EQ(nav.get(0)->val(), 1);
    ASSERT_EQ(nav.get(1)->val(), 1);
    ASSERT_STRNE(nbv.getName(1), "first");
    st = calc_sum_named(nav);
    ASSERT_EQ(st , 2);
    //st = calc_sum(nav);
    //ASSERT_EQ(st , 2);

    //print_name(nbv);
    //print_name(nav);
    nbv.clear();
    nav.clear();
} 


int main(int argc, char *argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}