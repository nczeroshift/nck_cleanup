
#include "CharacterBounds.h"
#include "nckUtils.h"
#include "nckException.h"
#include "nckMathUtils.h"

CharacterBounds::CharacterBounds() {
	min_x = min_y = max_x = max_y = 0;
	line_height = 0;
}

CharacterBounds::CharacterBounds(int x, int y) {
	min_x = x;
	min_y = y;
	max_x = x;
	max_y = y;
	line_height = 0;
}

CharacterBounds::CharacterBounds(int min_x, int min_y, int max_x, int max_y) {
	this->min_x = min_x;
	this->min_y = min_y;
	this->max_x = max_x;
	this->max_y = max_y;
}

bool CharacterBounds::IsNear(int x, int y, int margin) {
	return  (x >= (min_x - margin)) && (x <= (max_x + margin))
		&& (y >= (min_y - margin)) && (y <= (max_y + margin));
}

void CharacterBounds::Expand(const CharacterBounds & bounds) {
	min_x = MIN(min_x, bounds.min_x);
	min_y = MIN(min_y, bounds.min_y);
	max_x = MAX(max_x, bounds.max_x);
	max_y = MAX(max_y, bounds.max_y);
}

void CharacterBounds::ApplyOffset(int x, int y) {
	int w = max_x - min_x;
	int h = max_y - min_y;
	int ln = line_height - min_y;

	min_x = x;
	max_x = min_x + w;
	min_y = y;
	max_y = min_y + h;
	line_height = ln; //+ min_y;
}


void CopyArea(Core::Image * input, Core::Image * output, int l_x, int l_y, CharacterBounds bb) {
	int src_bpp = 4;
	int dst_bpp = 4;

	for (int y = bb.GetMinY(); y <= bb.GetMaxY(); y++) {
		for (int x = bb.GetMinX(); x <= bb.GetMaxX(); x++) {
			uint8_t * srcData = &(input->GetData())[((y)* input->GetWidth() + x) * src_bpp];

			uint8_t * dstData = &((uint8_t*)output->GetData())[
				(((l_y + y - bb.GetMinY())) * output->GetWidth() + l_x + x - bb.GetMinX()) * dst_bpp
			];

			dstData[0] = srcData[0];
			dstData[1] = srcData[1];
			dstData[2] = srcData[2];
			dstData[3] = srcData[3];
		}
	}
}


void DetectCharactersBounds(Core::Image * src_img, int margin, std::list<CharacterBounds> * characters) {
	const int tmp_src_bbp = 4;

	int last_one = -1;
	for (int y = 0; y < src_img->GetHeight(); y++)
	{
		for (int x = 0; x < src_img->GetWidth(); x++)
		{
			uint8_t * cData = &src_img->GetData()[(y * src_img->GetWidth() + x) * tmp_src_bbp];
			bool hasColor = cData[3] > 10;

			if (hasColor) {
				bool didFind = false;

				CharacterBounds tmp(x, y, x, y+1);

				ListWhile(CharacterBounds, (*characters), i) {
					if (i->IsNear(x, y, margin)) {
						tmp.Expand(*i);
						
						i = (*characters).erase(i);
					}
					else
						i++;
				}

				characters->push_back(tmp);
			}
		}
	}

	ListFor(CharacterBounds, (*characters), i) {
		CharacterBounds bb(i->GetMinX(), i->GetMinY(), i->GetMaxX()+1, i->GetMaxY());
		i->Expand(bb);
	}

}


void SaveCharactersMap(int dst_img_width, 
	int dst_img_height, 
	const std::string & path, 
	bool monoSpaced,
	int spaceWidth, 
	int charHeight,
	int margin,
	std::list<CharacterBounds> * in
) {
	Core::FileWriter * fw = Core::FileWriter::Open(path);

	if (fw == NULL)
		THROW_EXCEPTION("Unable to open output map");

	std::string params = "version,2\n";
	params += "size," + Math::IntToString(dst_img_width) + "," + Math::IntToString(dst_img_height) + "\n";

	if (monoSpaced) 
		params += "monospaced,true\n";

	params += "height," + Math::IntToString(charHeight) + "\n";
	params += "space," + Math::IntToString(spaceWidth)+"\n";
	params += "tab," + Math::IntToString(spaceWidth * 4) + "\n";
	params += "margin," + Math::IntToString(margin / 2) + "\n";
	params += "cmap\n";

	fw->Write((uint8_t*)params.c_str(), params.length());

	int count = 0;
	ListFor(CharacterBounds, (*in), i) {
		std::string line = "";
		char buffer[] = { (char)i->GetCode() ,'\0' };

		line += Math::IntToString(i->GetMinX()) + "," +
			Math::IntToString(i->GetMinY()) + "," +
			Math::IntToString(i->GetMaxX()) + "," +
			Math::IntToString(i->GetMaxY()) + "," +
			Math::IntToString(i->GetLineHeight()) + "," +
			std::string(buffer) +
			"\n";

		fw->Write((uint8_t*)line.c_str(), line.length());
		count++;
	}

	SafeDelete(fw);
}

void SaveCharactersBitmap(Core::Image * src_img, 
	int dst_img_width, int dst_img_height, 
	std::string dst_img_path, 
	int dst_characters_margin, 
	std::list<CharacterBounds> * in,
	std::list<CharacterBounds> * out)
{
	Core::Image * img = Core::Image::Create(dst_img_width, dst_img_height, Core::PIXEL_FORMAT_RGBA_8B);
	memset(img->GetData(), 0xFFFFFF00, dst_img_width * dst_img_height * 4);

	int current_x = dst_characters_margin;
	int current_y = dst_characters_margin;
	int last_height = 0;


	ListFor(CharacterBounds, (*in), i) {
		if (current_x + (i->GetMaxX() - i->GetMinX()) + dst_characters_margin > dst_img_width) {
			current_x = dst_characters_margin;
			current_y += last_height + dst_characters_margin;
			last_height = 0;
		}

		CopyArea(src_img, img, current_x, current_y, *i);

		CharacterBounds nC = *i;
		nC.ApplyOffset(current_x, current_y);

		(*out).push_back(nC);

		current_x += (i->GetMaxX() - i->GetMinX()) + dst_characters_margin;

		last_height = MAX(last_height, i->GetMaxY() - i->GetMinY());
	}

	img->Save(dst_img_path, Core::IMAGE_TYPE_PNG);
	SafeDelete(img);
	
	/*characters_mutex->Lock();
	OpenMap(dst_map_path);
	characters_mutex->Unlock();

	m_LoadOutput = true;*/
}


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
	const std::map<int, CharacterBounds> & mapped_characters) 
{
	float offset_x = x;
	float offset_y = y;

	for (int i = 0; i < text.size(); i++) {
		char c = text[i];

		if (offset_x + spaceWidth * 2 > maxWidth) {
			offset_x = x;
			offset_y += height;
		}

		if (c == ' ') {
			offset_x += spaceWidth;
		}
		else if (c == '\n') {
			offset_y += height;
			offset_x = x;
		}
		else if (mapped_characters.find(c) != mapped_characters.end()) {
			CharacterBounds bb = mapped_characters.find(c)->second;

			float uv_s_x = (bb.GetMinX() - margin) / (float)src_img_width;
			float uv_s_y = (bb.GetMinY() - margin) / (float)src_img_height;

			float uv_e_x = (bb.GetMaxX() + margin) / (float)src_img_width;
			float uv_e_y = (bb.GetMaxY() + margin) / (float)src_img_height;

			float x_size = (bb.GetMaxX() + margin) - (bb.GetMinX() - margin);
			float y_size = (bb.GetMaxY() + margin) - (bb.GetMinY() - margin);

			float aspect = x_size / y_size;

			float c_size = x_size;

			float ox = offset_x+extraMargin;

			if (monospaced)
				ox += (int)(spaceWidth / 2) - (int)(x_size / 2);
			else
				ox -= margin;

			float oy = offset_y + (bb.GetMinY() - bb.GetLineHeight());

			dev->Begin(Graph::PRIMITIVE_QUADS);
			dev->Color(255, 255, 255);

			dev->TexCoord(uv_s_x, uv_s_y);
			dev->Vertex(ox, oy, 0);

			dev->TexCoord(uv_s_x, uv_e_y);
			dev->Vertex(ox, oy + c_size / aspect, 0);

			dev->TexCoord(uv_e_x, uv_e_y);
			dev->Vertex(ox + c_size, oy + c_size / aspect, 0);

			dev->TexCoord(uv_e_x, uv_s_y);
			dev->Vertex(ox + c_size, oy, 0);

			dev->End();

			if (monospaced)
				offset_x += spaceWidth;
			else
				offset_x += x_size - margin * 2;

			//offset_x += extraMargin;
		}
	}
}