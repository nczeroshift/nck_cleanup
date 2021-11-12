
/**
 * nctoolkit, realtime aplication framework, under zlib software license.
 * https://github.com/nczeroshift/nctoolkit
 */

#include "nckStringUtils.h"
#include <string>
#include <sstream>
#include <iomanip>

_CORE_BEGIN

int StringSplit(std::string src, const std::string & token, std::vector<std::string> * splits) {
    if (splits == NULL)
        return 0;

    while (true) {
        size_t p = src.find_first_of(token);
        if (p != src.npos) {
            if (p > 0)
                splits->push_back(src.substr(0, p));

            src = src.substr(p + token.size(), src.length());
        }
        else {
            splits->push_back(src.substr(0, src.length()));
            break;
        }

    }

    return (int)splits->size();
}

std::string StringTrimLeft(const std::string & exp) {
    for (size_t i = 0; i < exp.length(); i++)
        if (!isspace(exp[i]))
            return exp.substr(i);
    return exp;
}

std::string StringTrimRight(const std::string & exp) {
    for (int i = (int)exp.length() - 1; i >= 0; i--)
        if (!isspace(exp[i]))
            return exp.substr(0, i + 1);
    return exp;
}


std::string StringWithFloat(float value) {
    std::stringstream sstream;
    sstream << value;
    return sstream.str();
}

std::string StringWithFloat(float value, int decimal_precision) {
    std::stringstream sstream;
    sstream << std::fixed << std::setprecision(decimal_precision) << value;
    return sstream.str();
}

std::string StringWithInt(int value) {
    std::stringstream sstream;
    sstream << value;
    return sstream.str();
}

std::string StringWithInt(int value, int leading) {
    std::stringstream sstream;
    sstream << value;
    std::string v = sstream.str();
    int s = leading + 1 - v.size();
    if (s < 0)s = 0;
    return std::string(s, '0') + v;
}

int StringToInt(const std::string& value) {
    int i;
    std::istringstream(value) >> i;
    return i;
}

std::string StringWithMicroTime(int64_t t) {
    int hours = t / (3600 * 1e6);
    int minutes = (int64_t)(t / (60 * 1e6)) % 60;
    int seconds = ((int64_t)(t / 1e6)) % 60;
    int milis = ((t / 1000) % 1000) / 10;
    return StringWithInt(hours, 1) + ":" + StringWithInt(minutes, 1) + ":" + StringWithInt(seconds, 1) + "." + StringWithInt(milis, 1);
}

_CORE_END
