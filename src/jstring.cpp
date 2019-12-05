#include "jstring.hpp"

namespace Jstr {

    namespace {
        bool isEnd(const char* ptr){
            return (((*ptr == '"') && (*(ptr-1) != '\\')) || (*ptr == '\0'));
        }
    }

    int seekLen(const char* src){
        int n = 0;

        if(*src != '"') {return -1;} //missing starter quote mark
        src++;

        //iterate
        while(!isEnd(src)) {n++; src++;}

        if(*src == '\0') {return -1;}  // missing terminator quote mark
        else {return n+2;}
    }

    void escape(char* dst, const char* src){
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
    }

    void unescape(char* dst, const char* src){
        if(*src == '"') {src++;}

        while( !isEnd(src) ) {
            if(*src == '\\') {
                char c = *(++src);
                if((c == '"') || (c == '\\') || (c == '/')) {*(dst++) = c;}
                else if(c == 'n') {*(dst++) = '\n';}
                else if(c == 'r') {*(dst++) = '\r';}
                else if(c == 't') {*(dst++) = '\t';}
            }
            else {
                *(dst++) = *src;
            }
            src++;
        }

        *dst = '\0';
    }
}