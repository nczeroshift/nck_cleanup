
/**
 * nctoolkit, realtime aplication framework, under zlib software license.
 * https://github.com/nczeroshift/nctoolkit
 */

#include "nckLog.h"
#if defined(_WIN32) || defined (_WIN64)
#include <Windows.h>
#endif

_CORE_BEGIN

void writeOutput(const std::string & type, const std::string& what) {
#if _DEBUG && ( defined(_WIN32) || defined (_WIN64))
    OutputDebugStringA((type + ": " + what).c_str());
#else
    printf("DEBUG: %s", msg.c_str());
#endif
}

void Log::Exception(const std::string & what) {
    writeOutput("EXCEPTION", what);
}

void Log::Debug(const std::string & what){
    writeOutput("DEBUG", what);
}

void Log::Info(const std::string& what) {
    writeOutput("INFO", what);
}


_CORE_END
