#ifndef UTILS_H
#define UTILS_H

#include "nckGraphics.h"


void drawRectXY(Graph::Device * dev, float x, float y, float x2, float y2);
void drawLine(Graph::Device* dev, float x, float y, float x2, float y2);
void renderViewportBounds(Graph::Device* dev, const float width, const float height);
Core::Point updatePositionWithMouseWheel(
    Core::Point position,
    float& scale,
    short mouseWheel,
    unsigned int dimX,
    unsigned int dimY,
    Core::Point mousePosition,
    Core::Point mouseOffset);



#endif
