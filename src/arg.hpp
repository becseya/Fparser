#pragma once

struct GetArgs {
    int depth = -1;
    bool dense = false;
    bool only_exportable = false;
};

struct SetArgs {
    bool import = false;
    bool err_not_found = false;
    bool err_set = false;
};

class ParseArg : public SetArgs, public GetArgs {
public:
    static const int ERR_MSG_SIZE = 128;
    static const int ERR_STACK_SIZE = 1024;
    
    bool err;
    bool set_ok;
    bool dense;
    char arg_msg[ERR_MSG_SIZE + 1];
    char arg_stack[ERR_STACK_SIZE + 1];

    void stepIn();
    void stepOut(const char* name);
    void errWrite(const char* msg, const char* stack = nullptr);
    void stackAdd(const char* add);
};
