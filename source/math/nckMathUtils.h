
/**
 * nctoolkit, realtime aplication framework, under zlib software license.
 * https://github.com/nczeroshift/nctoolkit
 */

#ifndef NCK_MATHUTILS_H
#define NCK_MATHUTILS_H

#include "nckMathConfig.h"
#include <string>
#include <stdint.h>

_MATH_BEGIN

/// Get a random value.
float RandomValue(float min = 0.0f, float max = 1.0f);

/// Limit value between a minimum and maximum value.
float Clamp(float min,float max,float val);

/// Get value signal.
float Signal(float val);

/// Get the maximum value between two.
float Max(float a,float b);

/// Get the minimum value between two.
float Min(float a,float b);


_MATH_END

#endif
