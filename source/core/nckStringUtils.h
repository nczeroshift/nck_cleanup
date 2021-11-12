
/**
 * nctoolkit, realtime aplication framework, under zlib software license.
 * https://github.com/nczeroshift/nctoolkit
 */

#ifndef NCK_STRING_UTILS_H
#define NCK_STRING_UTILS_H

#include "nckCoreConfig.h"
#include <string>
#include <vector>

_CORE_BEGIN

/// Split string in substrings at the tokens occurences(no regex support).
int StringSplit(const std::string src,
    const std::string & token,
    std::vector<std::string>* splits);

/// Remove spaces from left.
std::string StringTrimLeft(const std::string& exp);

/// Remove spaces from right.
std::string StringTrimRight(const std::string& exp);

/// Convert single precision float to string.
std::string StringWithFloat(float value);

/// Convert single precision float with a specific decimal precision to string.
std::string StringWithFloat(float value, int decimal_precision);

/// Convert integer to string.
std::string StringWithInt(int value);

/// Convert integer to string with leading zeros.
std::string StringWithInt(int value, int leading);

/// Convert microseconds timestamp to string (HH:mm:ss.MMM)
std::string StringWithMicroTime(int64_t v);

/// Convert string to integer
int StringToInt(const std::string & value);

_CORE_END

#endif 
