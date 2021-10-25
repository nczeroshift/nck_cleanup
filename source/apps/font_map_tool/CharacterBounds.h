#ifndef CHARACTER_BOUNDS_H
#define CHARACTER_BOUNDS_H

#include <stdint.h>
#include "nckImage.h"
#include "nckGraphics.h"
#include <map>
#include <list>

class CharacterBounds {
public:
	CharacterBounds();
	CharacterBounds(int x, int y);
	CharacterBounds(int min_x, int min_y, int max_x, int max_y);

	bool IsNear(int x, int y, int margin);

	void Expand(const CharacterBounds & bounds);

	int GetMinX() const { return min_x; }

	int GetMinY() const { return min_y; }

	int GetMaxX() const { return max_x; }

	int GetMaxY() const { return max_y; }

	void SetCode(int code) { this->code = code; }

	int GetCode() const { return code; }

	void SetLineHeight(int lh) { this->line_height = lh; }

	int GetLineHeight() const { return line_height; }

	void ApplyOffset(int x, int y);

private:
	uint32_t code;
	int min_x, min_y, max_x, max_y;
	int line_height;
};

void SaveCharactersMap(int dst_img_width,
	int dst_img_height,
	const std::string & path,
	bool monoSpaced,
	int spaceWidth,
	int charHeight,
	int margin,
	std::list<CharacterBounds> * in
);

void SaveCharactersBitmap(Core::Image * src_img,
	int dst_img_width, int dst_img_height,
	std::string dst_img_path,
	int dst_characters_margin,
	std::list<CharacterBounds> * in,
	std::list<CharacterBounds> * out);


void CopyArea(Core::Image * input, Core::Image * output, int l_x, int l_y, CharacterBounds bb);
void DetectCharactersBounds(Core::Image * src_img, int margin, std::list<CharacterBounds> * characters);

void RenderTextExt(Graph::Device * dev,
	float src_img_width, float src_img_height,
	float x, float y,
	float spaceWidth,
	float height,
	float margin,
	bool monospaced,
	int maxWidth,
	const std::string & text,
	int extraMargin,
	const std::map<int, CharacterBounds> & mapped_characters);

#endif