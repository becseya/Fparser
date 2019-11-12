#pragma once

#include "base.hpp"
#include "vector/named-vector.hpp"

#include <string.h>

class FieldNode : public NodeBase {
    NamedPVector<NodeBase> this_children;
    NamedPVector<NodeBase>& children;
    char name_buffer[NamedClass::NAME_SIZE+1];

public:
    FieldNode(const char* name = "root", int flags = 0);
    FieldNode(const char* name, NamedPVector<NodeBase>& children, int flags = 0);

    void add(NodeBase* child);
    virtual int get_override(char* out, void* arg) override;
    virtual int set_override(const char* str, void* arg) override;
    //virtual int parse(const char* in, char* out, void* arg) override;
    
private:
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