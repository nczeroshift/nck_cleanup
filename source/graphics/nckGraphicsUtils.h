
/**
 * nctoolkit, realtime aplication framework, under zlib software license.
 * https://github.com/nczeroshift/nctoolkit
 */

#ifndef NCK_GRAPHICS_UTILS_H
#define NCK_GRAPHICS_UTILS_H

#include "nckGraphics.h"

_GRAPHICS_BEGIN

void Render_Square(Graph::Device * dev, float x, float y, float w, float h, bool flipUV = false);
void Render_Cube(Graph::Device* dev);

_GRAPHICS_END

#endif
