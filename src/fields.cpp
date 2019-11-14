#include "fields.hpp"
#include "jstring.hpp"

#include <string.h>
#include <stdio.h>

#define compx(x,y) (strncmp(x,y,strlen(y))==0)

BoolField::BoolField(const char* name, bool& val, int flags, bool* set_flag) :
    NodeBase(name, flags, set_flag),
    val(val)
{}

int BoolField::set_override(const char* str, void* /*arg*/) {
    if((*str == '1') || compx(str, "true")) {val = true;}
    else if((*str == '0') || compx(str, "false")) {val = false;}
    else if(compx(str, "invert") || compx(str, "flip")) {val = !val;;}
    else {return -1;}
    
    return 0;
}

int BoolField::get_override(char* str, void* /*arg*/) {
    if(val) {strcpy(str, "true");}
    else {strcpy(str, "false");}

    return 0;
}

//-------------------------------------------------------------------------------------------------

IntField::IntField(const char* name, int& val, int flags, bool* set_flag) :
    NodeBase(name, flags, set_flag),
    val(val)
{}

int IntField::set_override(const char* str, void* /*arg*/) {
    int tmp;
    if(sscanf(str, "%d", &tmp) == 1) {
        val = tmp;
        return 0;
    }
    return -1;
}

int IntField::get_override(char* str, void* /*arg*/) {
    sprintf(str, "%d", val);
    return 0;
}

//-------------------------------------------------------------------------------------------------

FloatField::FloatField(const char* name, float& val, int flags, bool* set_flag) :
    NodeBase(name, flags, set_flag),
    val(val)
{}

int FloatField::set_override(const char* str, void* /*arg*/) {
    float tmp;
    if(sscanf(str, "%g", &tmp) == 1) {
        val = tmp;
        return 0;
    }
    return -1;
}

int FloatField::get_override(char* str, void* /*arg*/) {
    sprintf(str, "%g", val);
    return 0;
}

//-------------------------------------------------------------------------------------------------

CharField::CharField(const char* name, char* val, int flags, bool* set_flag) :
    NodeBase(name, flags, set_flag),
    val(val)
{}

int CharField::set_override(const char* str, void* /*arg*/) {
    //test format
    if(Jstr::seekLen(str) < 0) {return -1;}
    
    Jstr::unescape(val, str);
    return 0;
}

int CharField::get_override(char* str, void* /*arg*/) {
    Jstr::escape(str, val);
    return 0;
}