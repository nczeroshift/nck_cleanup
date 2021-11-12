
/**
 * nctoolkit, realtime aplication framework, under zlib software license.
 * https://github.com/nczeroshift/nctoolkit
 */

#ifndef NCK_LOG_H
#define NCK_LOG_H

#include "nckCoreConfig.h"
#include <string>
#include <vector>

_CORE_BEGIN

class Log {
public:
    void enableException(bool flag);
    void enableInfo(bool flag);
    void enableDebug(bool flag);
    static void Exception(const std::string & what);
    static void Info(const std::string & what);
    static void Debug(const std::string& what);
};
_CORE_END

#endif 
