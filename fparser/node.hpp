#pragma once

#include "base.hpp"
#include "fields.hpp"
#include "vector/pvector.hpp"

#include <string.h>

class FieldNode : public FieldBase {
    PVector<FieldBase> children;

public:
    FieldNode(const char* name, uint32_t flags = 0);
    void add(FieldBase* child);

    virtual int get_override(char* out, bool dense, int print_depth, uint32_t flags) override;
    virtual int set_override(const char* str, ParseArg* arg) override;
    virtual int parse_override(const char* in, char* out, ParseArg& arg) override;
    
private:
    FieldBase* getChildByName(const char*& str);
    void skipName(const char*& c);
    void skipData(const char*& str);
    bool nameAval(const char*& c);
};