#pragma once

#include "base.hpp"


class BoolField : public FieldBase {
    bool& val;

public:
    BoolField(const char* name, bool& val, bool def, uint32_t flags = 0, bool& set_flag = *(bool*)nullptr);

    int set_override(const char* str, ParseArg* arg) override;
    int get_override(char* str, bool dense, int print_depth, uint32_t flags) override;
    int parse_override(const char* in, char* out, ParseArg& arg) override;
};



class IntField : public FieldBase {
    int& val;

public:
    IntField(const char* name, int& val, int def, uint32_t flags = 0, bool& set_flag = *(bool*)nullptr);

    int set_override(const char* str, ParseArg* arg) override;
    int get_override(char* str, bool dense, int print_depth, uint32_t flags) override;
};



class FloatField : public FieldBase {
    float& val;

public:
    FloatField(const char* name, float& val, float def, uint32_t flags = 0, bool& set_flag = *(bool*)nullptr);

    int set_override(const char* str, ParseArg* arg) override;
    int get_override(char* str, bool dense, int print_depth, uint32_t flags) override;
};



class CharField : public FieldBase {
    char* val;

public:
    CharField(const char* name, char* val, const char* def, uint32_t flags = 0, bool& set_flag = *(bool*)nullptr);
    
    int set_override(const char* str, ParseArg* arg) override;
    int get_override(char* str, bool dense, int print_depth, uint32_t flags) override;
};
