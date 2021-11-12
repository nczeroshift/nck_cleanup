
/**
 * nctoolkit, realtime aplication framework, under zlib software license.
 * https://github.com/nczeroshift/nctoolkit
 */

#include "nckMathUtils.h"
#include <stdlib.h>
#include <sstream>
#include <iomanip>

_MATH_BEGIN

float RandomValue(float min, float max){
    float fRandNum = (float)rand () / RAND_MAX;
    return min + (max - min) * fRandNum;
}

float Clamp(float min,float max,float val){
    if(val<min)
        return min;
    if(val>max)
        return max;
    return val;
}

float Signal(float val){
    if(val>0)
        return 1;
    else 
        return -1;
}

float Max(float a,float b){
    if(a>b)
        return a;
    else 
        return b;
}

float Min(float a,float b){
    if(a>b)
        return b;
    else 
        return a;
}


_MATH_END
