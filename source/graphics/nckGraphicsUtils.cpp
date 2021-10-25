
/**
 * nctoolkit, realtime aplication framework, under zlib software license.
 * https://github.com/nczeroshift/nctoolkit
 */

#include "nckGraphicsUtils.h"

#include <math.h>
#include <string.h>

_GRAPHICS_BEGIN

void Render_Square(Graph::Device* dev, float x, float y, float w, float h, bool flipUV) {
    dev->Begin(Graph::PRIMITIVE_QUADS);

    dev->Normal(0, 0, 1.0f);
    dev->TexCoord(0.0f, flipUV ? 1.0f : 0.0f);
    dev->Vertex(x, y);

    dev->Normal(0, 0, 1.0f);
    dev->TexCoord(0, flipUV ? 0.0f : 1.0f);
    dev->Vertex(x, y + h);

    dev->Normal(0, 0, 1.0f);
    dev->TexCoord(1.0f, flipUV ? 0.0f : 1.0f);
    dev->Vertex(x + w, y + h);

    dev->Normal(0, 0, 1.0f);
    dev->TexCoord(1.0f, flipUV ? 1.0f : 0.0f);
    dev->Vertex(x + w, y);

    dev->End();
}

void Render_Cube(Graph::Device* dev) {

}

_GRAPHICS_END
