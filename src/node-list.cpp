#include "node-list.hpp"
#include "arg.hpp"

FieldList::FieldList(const char* name, NamedPVector<NodeBase>& children, int flags) :
    FieldNode(name, children, flags, LIST)
{}

NamedFieldList::NamedFieldList(const char* name, NamedPVector<NodeBase>& children, int flags) :
    FieldNode(name, children, flags, LIST_NAMED)
{}


int FieldList::set_override(const char* in, void* arg_) {
    int st;
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
        st = child->set(in, arg_, arg->import);
        if (st < 0) {
            children.removelast();
        }
        skipData(in);
    }

    return 0;
}

int NamedFieldList::set_override(const char* in, void* arg_) {
    int st;
    const char* name;
    NodeBase* child;
    SetArgs arg_def;
    SetArgs* arg = arg_ ? (SetArgs*)arg_ : &arg_def;
    
    while((name = findNextName(in))) {
        child = children.getByName(name);
        if(*in == ':') {in++;}
        skipWhiteSpace(in);

        if(child) {
            st = child->set(in, arg, arg->import);
            if(st < 0) {
                arg->err_set = true;
                if(!arg->import) {return -1;} 
            }
        }
        else {
            if(*name == '\0') {
                child = children.createAndAdd();
            }
            else {
                child = children.createAndAddNamed(name);
            }
            
            st = child->set(in, arg_, arg->import);
            if (st < 0) {
                children.removelast();
                arg->err_set = true;
                if(!arg->import) {return -1;} 
            }
        }

        skipData(in);
    }

    return 0;
}