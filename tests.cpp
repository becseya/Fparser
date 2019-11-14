
#include "src/vector/pvector.hpp"
#include "src/vector/named-vector.hpp"
#include "src/fields.hpp"
#include "src/node-list.hpp"
#include "src/arg.hpp"
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

    /*PVector<NA> nav;
    nav.add(naptr);
    void* target = nav.get(0);
    PVector<A> vectora(nav);
    void* actual = vectora.get(0);
    //ASSERT_NE(target, actual);

    A* aptr11 = (NA*)actual;
    A* aptr22 = (A*)actual;
    //st = aptr22->val();
    //ASSERT_EQ(1, st);*/

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
    srand (0); // same names every run
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

char buff[32*1024];

bool bool1 = false;
char string_[32] = "default";
int int3 = 0;
float float1 = -3.5f;
bool changed = false;

BoolField bf1("a1", bool1, BoolField::FLAG_READ_ONLY);
IntField if1("a2", int3, 0, &changed);
FloatField ff1("a1", float1);
CharField strf("a2", string_);

TEST(Fparser, Fields) {

    st = bf1.get(buff);
    ASSERT_STREQ("false", buff);
    bool1 = true;
    st = bf1.get(buff);
    ASSERT_STREQ("true", buff);   
    st = bf1.set("false");
    ASSERT_EQ(-1, st);
    ASSERT_EQ(false, changed);
    BoolField bf2("im_bool2", bool1);
    bf2.set("false");
    ASSERT_EQ(false, bool1);
    bf2.set("invert");
    ASSERT_EQ(true, bool1);

    st = if1.get(buff);
    ASSERT_STREQ("0", buff);
    int3 = 5;
    st = if1.get(buff);
    ASSERT_STREQ("5", buff); 
    ASSERT_EQ(false, changed);
    if1.set("-33");
    ASSERT_EQ(-33, int3);
    ASSERT_EQ(true, changed);

    ff1.get(buff);
    ASSERT_STREQ("-3.5", buff);
    ff1.set("1000.1");
    ASSERT_EQ(1000.1f, float1);

    strf.get(buff);
    ASSERT_STREQ("\"default\"", buff); 
    strf.set("\"new\"");
    ASSERT_STREQ("new", string_); 
}

ParseArg arg;
GetArgs get_arg;
FieldNode root;
FieldNode subnode("sub");

TEST(Fparser, Node) {
    get_arg.dense = true;
    arg.dense = true;
    arg.import = true;

    subnode.add(&bf1);
    subnode.add(&if1);
    root.add(&subnode);
    root.add(&ff1);
    root.add(&strf);
    
    root.get(buff, &get_arg);
    //printf("%s\n", buff);
    ASSERT_STREQ("{\"sub\": {\"a1\": true, \"a2\": -33}, \"a1\": 1000.1, \"a2\": \"new\"}", buff);

    root.set("a5  :  -33    ,   a2:  \"old\"  ,  sub  :   {  dummy1:  \"asd\",,   , a2:-5,dummy2:{a2:0}},a2:0,  a1:    0.1", &arg);
    root.get(buff, &get_arg); 
    //printf("%s\n", buff);
    ASSERT_EQ(-5, int3);
    ASSERT_EQ(0.1f, float1);
    ASSERT_STREQ("old", string_);
}

struct DynamicIntField : public IntField {
    int myVal;
    DynamicIntField(const char* name, int def = 0) : IntField(name, myVal), myVal(def) {}
    operator int&() {return myVal;}
};

NamedPVector<DynamicIntField> myInts;
FieldList listf1("ints", myInts);

TEST(Fparser, List) {
    myInts.add(new DynamicIntField("name", 5));
    myInts.createAndAdd();
    ASSERT_EQ(5, (int)*myInts.get(0));
    ASSERT_EQ(0, (int)*myInts.get(1));
    ASSERT_STREQ("name", myInts.getName(0));
    ASSERT_EQ(5, (int)*myInts.getByName("name"));
    ASSERT_EQ(1, myInts.getNByName(myInts.getName(1)));

    root.add(&listf1);
    listf1.get(buff, &get_arg);
    ASSERT_STREQ("[5, 0]", buff);
    (int&)*myInts.get(0) = -33;
    (int&)*myInts.get(1) = 33;
    listf1.get(buff, &get_arg);
    ASSERT_STREQ("[-33, 33]", buff);

    listf1.set("[44,, \"asd\" ,  -4  ]");
    ASSERT_EQ(2, myInts.getSize());
    ASSERT_EQ(44, (int)*myInts.get(0));
    ASSERT_EQ(-4, (int)*myInts.get(1));
    listf1.get(buff, &get_arg);
    ASSERT_STREQ("[44, -4]", buff);
    //printf("%s\n", buff);

    //root.get(buff);
    //printf("%s\n", buff);
}

struct DynamicClass {
    int myVal;
    char myStr[64];
};

struct DynamicClassField : public FieldNode, public DynamicClass{
    DynamicClassField(const char* name) : FieldNode(name) {
        myVal = 3;
        strcpy(myStr, "hello");
        children.add(new IntField("szam", myVal));
        children.add(new CharField("str", myStr));
    }
};

NamedPVector<DynamicClassField> myClassList("tt");
NamedFieldList listf2("tables", myClassList, true);

TEST(Fparser, NamesList) {
    root.add(&listf2);

    listf2.set("alma:");
    listf2.set(":{szam:-5,str:\"chicken\"}");

    ASSERT_EQ(2, myClassList.getSize());
    ASSERT_EQ(3, myClassList.get(0)->myVal);
    ASSERT_STREQ("hello", myClassList.get(0)->myStr);
    ASSERT_EQ(-5, myClassList.get(1)->myVal);
    ASSERT_STREQ("chicken", myClassList.get(1)->myStr);
}



int main(int argc, char *argv[]) {
    int st;
    testing::InitGoogleTest(&argc, argv);
    st = RUN_ALL_TESTS();

    for (std::string line; std::getline(std::cin, line);) {
        root.parse(line.c_str(), buff);
        printf("%s\n", buff);
    }

    return st;
}