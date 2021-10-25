
/**
 * nctoolkit, realtime aplication framework, under zlib software license.
 * https://github.com/nczeroshift/nctoolkit
 */

#ifndef NCK_MATHCONFIG_H
#define NCK_MATHCONFIG_H

#include "../nckConfig.h"

#ifdef NCK_OPTIM_SSE
#include <immintrin.h>
#endif

#ifdef NCK_WINDOWS
    #define _USE_MATH_DEFINES
#endif

#include <math.h>

#define _MATH_BEGIN namespace Math{
#define _MATH_END }

#endif
