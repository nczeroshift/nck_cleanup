
/**
 * nctoolkit, realtime aplication framework, under zlib software license.
 * https://github.com/nczeroshift/nctoolkit
 */

#ifndef NCK_GRAPHICS_MATH_H
#define NCK_GRAPHICS_MATH_H

#include "nckGraphicsConfig.h"

_GRAPHICS_BEGIN

typedef struct gMat44 { float val[16]; } gMat44;
typedef struct gPlane { float val[4]; } gPlane;

void gIdentityMat44(gMat44 & a);
void gCopyMat44(gMat44 & a, const gMat44 & b);
void gMultMat44(gMat44& mRes, const gMat44 mA, const gMat44& mB);
bool gInvMat44(gMat44 & out, gMat44 & in);
void gTransposeMat44(gMat44 & out, gMat44 & in);

_GRAPHICS_END

#endif
