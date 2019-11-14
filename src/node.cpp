#include "node.hpp"
#include "arg.hpp"


FieldNode::FieldNode(const char* name, int flags) : 
    NodeBase(name, flags, OBJECT),
    children(this_children)
{}

FieldNode::~FieldNode() {}

FieldNode::FieldNode(const char* name, NamedPVector<NodeBase>& children, int flags, field_type_t type) : 
    NodeBase(name, flags, type),
    children(children)
{}

void FieldNode::add(NodeBase* child) {
    children.add(child);
}

int FieldNode::parse_override(const char* in, char* out, void* arg_) {
    int st;
    const char* name;
    ParseArg arg_def;
    ParseArg* arg = arg_ ? (ParseArg*)arg_ : &arg_def;

    // root
    if(arg->depth == -1) {
        strcpy(out, "");
        if(*in == '!') {in++; arg->dense=true;}
        else {arg->dense = false;}
    }
    
    // step in
    arg->stepIn();

    name = findNextName(in);
    NodeBase* child = children.getByName(name);

    if(child) {
        if(*in == '.') {in++;}
        child->parse(in, out, arg);
    }
    else if (name) {
        if(strcmp(name, "list") == 0) {
            strcat(out, "{\"children\": [ ");
            for(int i = 0; i < children.getSize(); i++) {
                if(i > 0) {strcat(out, ", ");}
                strcat(out, "\"");
                strcat(out, children.get(i)->getName());
                strcat(out, "\"");
            }
            strcat(out, " ]}");            
        }
        else if(type == LIST_NAMED) {
            if((strcmp(name, "remove") == 0) && (*in == ':')) {
                in++;
                name = findNextName(in);
                children.removeByName(name);
            }
            if(strcmp(name, "create") == 0) {
                set(":");
            }
        }
    }


    // final return
    if(arg->depth == 0) {/*printOutput(out, arg);*/}
    
    // step out
    //arg->stepOut(name);

    return st;
}

int FieldNode::get_override(char* out, void* arg_) {
    int st;
    int print_dept;
    GetArgs arg_def;
    GetArgs* arg = arg_ ? (GetArgs*)arg_ : &arg_def;


    // open bracket
    arg->depth++;
    print_dept = ((arg->dense || (type == LIST)) ? -2 : arg->depth);
    if(type == OBJECT) {strcpy(out, "{");}
    else {strcpy(out, "[");}
    printNewLine(out, print_dept+1);

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
            printNewLine(out, print_dept+1, true);
        }
        at_least_one = true;
        if(type != LIST) {
            strcat(out, "\"");
            strcat(out, child->getName());
            strcat(out, "\": ");
        }
        st = child->get(out + strlen(out), arg);
        if (st < 0) {break;}
    }

    // close bracket
    printNewLine(out, print_dept);
    if(type == OBJECT) {strcat(out, "}");}
    else {strcat(out, "]");}
    arg->depth--;

    return st;
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
    
    skipWhiteSpace(str);
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
        if(isWhitespace(str) || strchr("{}\"[]", *str)) {
            ; // don't care
        }
        else if((*str == ':') || (*str == ',') || (*str == '.')) {
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
    return ((*str || (n>0)) ? name_buffer : nullptr);
}