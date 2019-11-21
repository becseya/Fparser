#include "node-list.hpp"
#include "arg.hpp"

FieldList::FieldList(const char* name, NamedPVector<NodeBase>& children, int flags) :
    FieldNode(name, children, flags, LIST)
{}

NamedFieldList::NamedFieldList(const char* name, NamedPVector<NodeBase>& children, int flags) :
    FieldNode(name, children, flags, LIST_NAMED)
{}


int FieldList::set_override(const char* in, SetArgs* arg) {
    int st;

    // starting bracket
    skipWhiteSpace(in);
    if(*in != '[') {return -1;}
    in++;
    skipWhiteSpace(in);

    children.clear();

    while(!((*in == '\0') || (*in == ']'))) {
        NodeBase* child = children.createAndAdd();
        st = child->set(in, arg, arg->import);
        if (st < 0) {
            children.removelast();
        }
        skipData(in);
    }

    return 0;
}

int NamedFieldList::set_override(const char* in, SetArgs* arg) {
    int st;
    const char* name;
    
    // starting bracket
    skipWhiteSpace(in);
    if(*in != '{') {return -1;}
    in++;
    skipWhiteSpace(in);

    children.clear();

    while((name = findNextName(in))) {
        NodeBase* child = children.createAndAddNamed(name);
        st = child->set(in, arg, arg->import);
        if (st < 0) {
            children.removelast();
        }
        skipData(in);
    }

    return 0;
}