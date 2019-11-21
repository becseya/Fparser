#pragma once

#include "node-base.hpp"
#include "vector/named-vector.hpp"

#include <string.h>

class FieldNode : public NodeBase {
    NamedPVector<NodeBase> this_children;

protected:
    NamedPVector<NodeBase>& children;
    char name_buffer[NamedClass::NAME_SIZE+1];


public:
    FieldNode(const char* name = nullptr, int flags = 0);
    virtual ~FieldNode();
protected:
    FieldNode(const char* name, NamedPVector<NodeBase>& children, int flags, field_type_t type);


public:
    void add(NodeBase* child);
    
protected:
    virtual int get_override(char* out, GetArgs* arg) override;
    virtual int set_override(const char* str, SetArgs* arg) override;
    virtual int parse_override(const char* in, char* out, ParseArg* arg) override;
    const char* findNextName(const char*& str, bool strict = true);
    void skipData(const char*& c);
    void printNewLine(char* str, int depth, bool comma = false);

private:
    void printOutput(char* out, ParseArg* arg);
};
