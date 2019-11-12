#pragma once

#include "vector/named.hpp"



class NodeBase : public NamedClass {
public:
    enum field_type_t {FIELD, LIST, LIST_NAMED, OBJECT};
    static const int FLAG_NOT_EXPORTABLE       = (1<<0);
    static const int FLAG_NOT_VISIBLE          = (1<<1);
    static const int FLAG_RST_TO_APPLY         = (1<<2);
    static const int FLAG_RST_TO_APPLY_SOFT    = (1<<3);
    static const int FLAG_READ_ONLY            = (1<<4);

protected:
    const field_type_t type;

private:
    int flags;
    bool* set_flag;    
    

public:
    NodeBase(const char* name, int flags = 0, bool* set_flag = nullptr);
    virtual ~NodeBase();

    int getFlags();
    bool isExportable();
    bool isVisible();
    int get(char* out, void* arg = nullptr);
    int set(const char* in, void* arg = nullptr, bool import = false);
    virtual int parse(const char* in, char* out, void* arg = nullptr);
    
protected:
    virtual int get_override(char* out, void* arg);
    virtual int set_override(const char* in, void* arg);
};