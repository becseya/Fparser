#include "node.hpp"


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

int FieldNode::parse_override(const char* in, char* out, ParseArg* arg) {
    const char* name;

    // root
    if(arg->parse_depth == -1) {
        strcpy(out, "");
        if(*in == '!') {in++; arg->dense=true;}
        else {arg->dense = false;}
    }
    
    // step in
    arg->stepIn();

    skipWhiteSpace(in);
    name = findNextName(in);
    NodeBase* child = children.getByName(name);

    if(child) {
        if(*in == '.') {in++;}
        child->parse(in, out, arg);
    }
    else if (name) {
        if(strcmp(name, "export") == 0) {
            arg->only_exportable = true;
            get(out, arg);           
        }
        else if(strcmp(name, "list") == 0) {
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
                arg->set_ok = true;
            }
            if(strcmp(name, "create") == 0) {
                set(":");
                arg->set_ok = true;
            }
        }
        else {
            arg->errWrite("Children not found", name);
        }
    }


    // final return
    if(arg->parse_depth == 0) {printOutput(out, arg);}
    
    // step out
    arg->stepOut(getName());

    return (arg->err ? -1 : 0);
}

int FieldNode::get_override(char* out, GetArgs* arg) {
    int st;
    int print_dept;

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

int FieldNode::set_override(const char* in, SetArgs* arg) {
    int st;
    const char* name;

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

void FieldNode::printOutput(char* out, ParseArg* arg) {
    if(arg->err) {
        strcat(out, "{");
        printNewLine(out, (arg->dense ? -1 : 1));
        strcat(out, "\"status\": \"error\"");
        printNewLine(out, (arg->dense ? -1 : 1), true);
        strcat(out, "\"arg_msg\": \"");
        strcat(out, arg->arg_msg);
        strcat(out, "\"");
        printNewLine(out, (arg->dense ? -1 : 1), true);
        strcat(out, "\"arg_stack\": \"");
        strcat(out, arg->arg_stack);
        strcat(out, "\"");
        printNewLine(out, (arg->dense ? -1 : 0));
        strcat(out, "}");
    }
    else if(arg->set_ok) {
        strcat(out, "{\"status\": \"ok\"}");
    }
}
