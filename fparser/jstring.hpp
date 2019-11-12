#pragma once

namespace Jstr {
    int seekLen(const char* src);
    void escape(char* dst, const char* src);
    void unescape(char* dst, const char* src);
}