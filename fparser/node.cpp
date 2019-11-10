#include "node.hpp"


FieldNode::FieldNode(const char* name, uint32_t flags) : 
    FieldBase(name, flags)
{}

void FieldNode::add(FieldBase* child) {
    children.add(child);
}

int FieldNode::get_override(char* out, bool dense, int print_depth, uint32_t flags) {
    int st;

    // open bracelet
    strcat(out, "{");
    printNewLine(out, (dense ? -1 : print_depth+1));

    bool at_least_one = false;
    for(int i = 0; i < children.getSize(); i++) {
        FieldBase* child = children.get(i);
        if(!(child->getFlags() & flags)) {
            if(at_least_one) {
                printNewLine(out, (dense ? -1 : print_depth+1), true);
            }
            at_least_one = true;
            strcat(out, "\"");
            strcat(out, child->getName());
            strcat(out, "\": ");
            st = child->get(out, dense, print_depth+1, flags);
            if (st < 0) {return st;}
        }
    }

    // close bracelet
    printNewLine(out, (dense ? -1 : print_depth));
    strcat(out, "}");

    return 0;
}

int FieldNode::set_override(const char* str, ParseArg* arg) {
    int st;

    while(nameAval(str)) {
        FieldBase* field = getChildByName(str);

        if(field && (*(str++) == ':')) {
            skipWhiteSpace(str);
            st = field->set(str, arg);
            if(st < 0) {
                if(arg) {arg->errWrite("Failed command: set", field->getName());} 
                return st;
            }
        }
        else {
            if(arg) {arg->errWrite("Children not found", str);} 
            return -1;    
        }

        skipData(str);
    }

    return 0;
}

int FieldNode::parse_override(const char* in, char* out, ParseArg& arg) {
    if(compNext(in, "list")) {
        strcat(out, "\"children\": [ ");
        for(int i = 0; i < children.getSize(); i++) {
            if(i > 0) {strcat(out, ", ");}
            strcat(out, "\"");
            strcat(out, children.get(i)->getName());
            strcat(out, "\"");
        }
        strcat(out, " ]");
        return 0;
    }
    else if (FieldBase* child = getChildByName(in)) {
        return child->parse(in, out, arg);  
    }
    else {
        arg.errWrite("Children not found", in);
    }

    return -1;
}

FieldBase* FieldNode::getChildByName(const char*& str) {
    if(*str == '"') {str++;}
    for(int i = 0; i < children.getSize(); i++) {
        FieldBase* child = children.get(i);
        if(compNext(str, child->getName())) {
            if(*str == '"') {str++;}
            return child;
        }
    }
    return nullptr;
}

void FieldNode::skipName(const char*& c) {
    if(*c == '"') {c++;}
    while( !((*c == ':') || (*c == '\0')) ) {c++;}
    if(*c == ':') {c++;}
    skipWhiteSpace(c);
}

void FieldNode::skipData(const char*& str) {
    if(*str == '"') {
        str++;
        while( !(((*(str-1) != '\\') && (*str == '"')) || (*str == '\0')) ) {str++;}
        skipWhiteSpace(str);
        if(*str == '"') {str++;}
    }
    else if(*str == '{') {
        str++;
        skipWhiteSpace(str);
        while( !((*str == '\0') || (*str == '}')) ) {
            skipName(str);
            skipData(str);
        }
        if(*str == '}') {str++;}
    }
    else {
        while( !((*str == ',') || (*str == '}') || (*str == '\0')) ) {str++;}
    }
    skipWhiteSpace(str);
    if(*str == ',') {str++;}
    skipWhiteSpace(str);
}

bool FieldNode::nameAval(const char*& c) {
    skipWhiteSpace(c);
    if(*c == '{') {c++;}
    skipWhiteSpace(c);
    return !((*c == '\0') || (*c == '}'));
}
