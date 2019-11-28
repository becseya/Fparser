#pragma once

#include "node-base.hpp"


class BoolField : public NodeBase {
    bool& val;

public:
    BoolField(const char* name, bool& val, int flags = 0, bool* set_flag = nullptr);

    int set_override(const char* str, SetArgs* arg) override;
    int get_override(char* str, GetArgs* arg) override;
};



class IntField : public NodeBase {
    int& val;

public:
    IntField(const char* name, int& val, int flags = 0, bool* set_flag = nullptr);

    int set_override(const char* str, SetArgs* arg) override;
    int get_override(char* str, GetArgs* arg) override;
};



class FloatField : public NodeBase {
    float& val;

public:
    FloatField(const char* name, float& val, int flags = 0, bool* set_flag = nullptr);

    int set_override(const char* str, SetArgs* arg) override;
    int get_override(char* str, GetArgs* arg) override;
};



class CharField : public NodeBase {
    char* val;
    const char*& val_const;

public:
    CharField(const char* name, char* val, int flags = 0, bool* set_flag = nullptr);
    CharField(const char* name, const char*& val, int flags = 0, bool* set_flag = nullptr);
    
    int set_override(const char* str, SetArgs* arg) override;
    int get_override(char* str, GetArgs* arg) override;
};
