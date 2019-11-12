#include "node.hpp"
#include "arg.hpp"


FieldNode::FieldNode(const char* name, int flags) : 
    NodeBase(name, flags),
    children(this_children)
{}

FieldNode::FieldNode(const char* name, NamedPVector<NodeBase>& children, int flags) : 
    NodeBase(name, flags),
    children(children)
{}

void FieldNode::add(NodeBase* child) {
    children.add(child);
}

int FieldNode::get_override(char* out, void* arg_) {
    int st;
    GetArgs arg_def;
    GetArgs* arg = arg_ ? (GetArgs*)arg_ : &arg_def;
    
    // open bracket
    arg->depth++;
    strcpy(out, "{");
    printNewLine(out, (arg->dense ? -1 : arg->depth+1));

    bool at_least_one = false;
    for(int i = 0; i < children.getSize(); i++) {
        NodeBase* child = children.get(i);
        
        // skip if necesary
        if(arg->only_exportable) {
            if(!child->isExportable()) {continue;}
        }
        else {
            if(!child->isVisible()) {continue;}
        }

        // print
        if(at_least_one) {
            printNewLine(out, (arg->dense ? -1 : arg->depth+1), true);
        }
        at_least_one = true;
        strcat(out, "\"");
        strcat(out, child->getName());
        strcat(out, "\": ");
        st = child->get(out + strlen(out), arg);
        if (st < 0) {return st;}
    }

    // close bracket
    printNewLine(out, (arg->dense ? -1 : arg->depth));
    strcat(out, "}");
    arg->depth--;

    return 0;
}

int FieldNode::set_override(const char* in, void* arg_) {
    int st;
    const char* name;
    SetArgs arg_def;
    SetArgs* arg = arg_ ? (SetArgs*)arg_ : &arg_def;

    while((name = findNextName(in))) {
        NodeBase* child = children.getByName(name);
 
        if(child) {
            if(*in == ':') {in++;}
            skipWhiteSpace(in);
            st = child->set(in, arg, arg->import);

            if(st < 0) {
                arg->err_set = true;
                if(!arg->import) {return -1;}
            }
        }
        else {
            arg->err_not_found = true;
            if(!arg->import) {return -1;} 
        }

        skipData(in);
    }

    return 0;
}

void FieldNode::skipData(const char*& str) {
    bool inide_text = false;
    int depth = 0;

    while (*str) {
        if(inide_text) {
            if((*str == '"') && (*(str-1) != '\\')) {inide_text = false;}
        }
        else{
            if((*str == ',') && (depth == 0)) {break;}
            if(*str == '"') {inide_text = true;}
            if((*str == '{') || (*str == '[')) {depth++;}
            if((*str == '}') || (*str == ']')) {depth--;}
        }
        str++;  
    }

    if(*str == ',') {str++;}
    skipWhiteSpace(str);
}

bool FieldNode::isWhitespace(const char* c){
    return ((*c == ' ') || (*c == '\t') || (*c == '\n') || (*c == '\r'));
}

void FieldNode::skipWhiteSpace(const char*& c){
    while(isWhitespace(c)) {c++;}
}

void FieldNode::printNewLine(char* str, int depth, bool comma) {
    if(comma) {strcat(str, ", ");}
    if(!(depth < 0)) {
        strcat(str, "\n");
        for (int i = 0; i < depth; i++) {strcat(str, "   ");}
    }
}

const char* FieldNode::findNextName(const char*& str) {
    int n = 0;

    skipWhiteSpace(str);

    while(*str) {
        if((*str == '{') || (*str == '}') || (*str == '"')) {
            skipWhiteSpace(str);
        }
        else if((*str == ':') || (*str == ',') || isWhitespace(str)) {
            break;
        }
        else if(n < NamedClass::NAME_SIZE) {
            name_buffer[n] = *str;        
            n++;
        }
        str++;
    }

    skipWhiteSpace(str);
    name_buffer[n] = '\0';
    return (*str ? name_buffer : nullptr);
}