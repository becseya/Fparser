#include "fields.hpp"

#include <string.h>
#include <stdio.h>

BoolField::BoolField(const char* name, bool& val, bool def, uint32_t flags, bool& set_flag) :
    FieldBase(name, flags, set_flag),
    val(val)
{
    val = def;
}

int BoolField::set_override(const char* str, ParseArg* arg) {
    if((*str == '1') || compNext(str, "true")) {val = true;}
    else if((*str == '0') || compNext(str, "false")) {val = false;}
    else {return -1;}
    
    return 0;
}

int BoolField::get_override(char* str, bool dense, int print_depth, uint32_t flags) {
    if(val) {strcat(str, "true");}
    else {strcat(str, "false");}

    return 0;
}

int BoolField::parse_override(const char* in, char* out, ParseArg& arg) {
    if(compNext(in, "invert") || compNext(in, "flip")) {
        val = !val;
        arg.set_ok = true;
        return 0;
    }
    
    return -1;
}

//-------------------------------------------------------------------------------------------------

IntField::IntField(const char* name, int& val, int def, uint32_t flags, bool& set_flag) :
    FieldBase(name, flags, set_flag),
    val(val)
{
    val = def;
}

int IntField::set_override(const char* str, ParseArg* arg) {
    int tmp;
    if(sscanf(str, "%d", &tmp) == 1) {
        val = tmp;
        return 0;
    }
    return -1;
}

int IntField::get_override(char* str, bool dense, int print_depth, uint32_t flags) {
    sprintf(str+strlen(str), "%d", val);
    return 0;
}

//-------------------------------------------------------------------------------------------------

FloatField::FloatField(const char* name, float& val, float def, uint32_t flags, bool& set_flag) :
    FieldBase(name, flags, set_flag),
    val(val)
{
    val = def;
}

int FloatField::set_override(const char* str, ParseArg* arg) {
    float tmp;
    if(sscanf(str, "%g", &tmp) == 1) {
        val = tmp;
        return 0;
    }
    return -1;
}

int FloatField::get_override(char* str, bool dense, int print_depth, uint32_t flags) {
    sprintf(str+strlen(str), "%g", val);
    return 0;
}

//-------------------------------------------------------------------------------------------------

CharField::CharField(const char* name, char* val, const char* def, uint32_t flags, bool& set_flag) :
    FieldBase(name, flags, set_flag),
    val(val)
{
    strcpy(val, def);
}

int CharField::set_override(const char* str, ParseArg* arg) {
    if(*(str++) != '"') {return -1;}

    char* dst = val;
    while( !(((*(str-1) != '\\') && (*str == '"')) || (*str == '\0')) ) {
        if(*str == '\\') {
            char c = *(++str);
            if((c == '"') || (c == '\\') || (c == '/')) {*(dst++) = c;}
            else if(c == 'n') {*(dst++) = '\n';}
            else if(c == 'r') {*(dst++) = '\r';}
            else if(c == 't') {*(dst++) = '\t';}
        }
        else {
            *(dst++) = *str;
        }
        str++;
    }
    *dst = '\0';

    return 0;
}

int CharField::get_override(char* str, bool dense, int print_depth, uint32_t flags) {
    const char* src = val;
    char* dst = str + strlen(str);

    *(dst++) = '"';
    while(*src) {
        if(*src == '\n')      {*(dst++) = '\\'; *(dst++) = 'n';}
        else if(*src == '\r') {*(dst++) = '\\'; *(dst++) = 'r';}
        else if(*src == '\t') {*(dst++) = '\\'; *(dst++) = 't';}
        else {
            if((*src == '"') || (*src == '\\') || (*src == '/')) {*(dst++) = '\\';}
            *(dst++) = *src;
        }
        src++;
    }
    *(dst++) = '"';
    *dst = '\0';

    return 0;
}