#pragma once

#include "arg.hpp"

#include <stdint.h>


class FieldBase {
protected:
    const char* name;
    uint32_t flags;
    bool& set_flag;

public:
    static const uint32_t FLAG_NOT_EXPORTABLE           = (1<<0);
    static const uint32_t FLAG_NOT_VISIBLE              = (1<<1);
    static const uint32_t FLAG_READ_ONLY                = (1<<2);
    static const uint32_t FLAG_RST_TO_APPLY             = (1<<3);
    static const uint32_t FLAG_RST_TO_APPLY_SOFT        = (1<<4);
    
    static const uint32_t FLAG_SET_ERROR                = (1<<16);
    static const uint32_t FLAG_SET_CHANGED              = (1<<17);
    static const uint32_t FLAG_SET_CHANGED_EXPORTABLE   = (1<<18);
    static const uint32_t FLAG_SET_CHANGED_RST          = (1<<18);

    static const uint32_t FLAG_GET_ERROR                = (1<<24);
    
    
public:
    FieldBase(const char* name, uint32_t flags = 0, bool& set_flag = *(bool*)nullptr);
    virtual ~FieldBase();

    const char* getName();
    uint32_t getFlags();

    int set(const char* in, ParseArg* arg = nullptr);
    int get(char* out, bool dense = false, int print_depth = 0, uint32_t flags = 0);
    int parse(const char* in, char* out, ParseArg& arg);
  
protected:

    virtual int parse_override(const char* in, char* out, ParseArg& arg);
    virtual int set_override(const char* in, ParseArg* arg) = 0;
    virtual int get_override(char* out, bool dense, int print_depth, uint32_t flags) = 0;

    bool compNext(const char*& src, const char* cmp);
    void skipWhiteSpace(const char*& c);
    void printNewLine(char* str, int depth, bool comma = false);

private:
    void printOutput(char* out, ParseArg& arg);
};