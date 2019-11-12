#include "node-list.hpp"
#include "arg.hpp"

FieldList::FieldList(const char* name, NamedPVector<NodeBase>& children, bool named, int flags) :
    FieldNode(name, children, flags, named ? LIST_NAMED : LIST)
{}


int FieldList::set_override(const char* in, void* arg_) {
    int st;
    const char* name;
    SetArgs arg_def;
    SetArgs* arg = arg_ ? (SetArgs*)arg_ : &arg_def;

    // starting bracket
    skipWhiteSpace(in);
    if(*in != '[') {return -1;}
    in++;
    skipWhiteSpace(in);
    children.clear();

    while(!((*in == '\0') || (*in == ']'))) {
        NodeBase* child = children.createAndAdd();
        st = child->set(in);
        if (st < 0) {
            children.removelast();
        }
        skipData(in);
    }

    return 0;
}