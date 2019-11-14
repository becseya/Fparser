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
    virtual int get_override(char* out, void* arg) override;
    virtual int set_override(const char* str, void* arg) override;
    //virtual int parse(const char* in, char* out, void* arg) override;
    
protected:
    //virtual int parse_override(const char* in, char* out, void* arg);

    //void skipName(const char*& c);
    //bool nameAval(const char*& c);
    //bool compNext(const char*& src, const char* cmp);
    const char* findNextName(const char*& str);
    void skipData(const char*& c);
    bool isWhitespace(const char* c);
    void skipWhiteSpace(const char*& c);
    void printNewLine(char* str, int depth, bool comma = false);

    //NodeBase* getChildByName(const char*& str);
    //void printOutput(char* out, ParseArg& arg);*/
};