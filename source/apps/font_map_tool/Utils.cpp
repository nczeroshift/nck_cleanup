#include "Utils.h"


void drawRectXY(Graph::Device * dev, float x, float y, float x2, float y2) {
	dev->Begin(Graph::PRIMITIVE_QUADS);

	dev->TexCoord(0, 0);
	dev->Vertex(x, y, 0);
	dev->TexCoord(0, 1);
	dev->Vertex(x, y2, 0);
	dev->TexCoord(1, 1);
	dev->Vertex(x2, y2, 0);
	dev->TexCoord(1, 0);
	dev->Vertex(x2, y, 0);

	dev->End();
}


