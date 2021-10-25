#ifndef APP_H
#define APP_H

#include "nckWindow.h"
#include "nckException.h"
#include "nckThread.h"
#include "nckGraphics.h"
#include "nckUtils.h"
#include "nckColor4.h"
#include "nckQuadtree.h"
#include "nckDataIO.h"
#include "nckMathUtils.h"
#include "nckImage.h"
#include "nckFont.h"
#include "nckVec2.h"

#include "CharacterBounds.h"

#include "Utils.h"

class App : public virtual Core::Threadable
{
private:
	std::list<CharacterBounds>::iterator selection_a;
	std::list<CharacterBounds>::iterator selection_b;
	std::list<CharacterBounds>::iterator selection_current;
	std::vector<int> lineHeights;
	int selection_current_ln = -1, selection_ln = -1;

	bool wasLMBDown;
	bool wasESCDown;
	bool wasZDown;

	std::string src_img_path;
	Graph::Texture2D * src_img_tex;
	Core::Image * src_img;

	std::string dst_map_path;
	std::string dst_img_path;

	int dst_img_width;
	int dst_img_height;
	int dst_characters_margin;

	int detect_margin;
	Core::Mutex * characters_mutex;
	std::list<CharacterBounds> characters;

	Graph::Device * dev;
	Core::Window * wnd;

	std::map<int, CharacterBounds> mapped_characters;

	//Gui::FontMap * fMap;

	int m_UpdateCount;

	int m_DetectFirstLine;
	int m_DetectLineHeight;
	bool m_Monospaced;
	int m_SpaceWidth;
	int m_CharactersHeight;
	int m_ExtraMargin;
    float m_Scale;
    Core::Point m_MouseOffset;
    Core::Point m_MouseDownStart;
    Core::Point m_Position;
public:

	App(Core::Window * window);

	~App();

	void SetSourceImage(const std::string & path);

	void DetectRows();

	void SortCharacters();

	void RenderText(float x, float y, float size, const std::string & text);

	void OpenMap(const std::string & path);
	
	void SaveOutput();

	void Run();

	void RenderGraphics(const Math::Vec2 & position, const Core::Point & cursor, float scale);

	void UpdateWndInput();

	void SetFixedLine(int first, int height);
	void SetMonospace(bool monospaced);
	void SetOutputMargin(int margin);
	void SetDetectMargin(int margin);
	void SetOutputSize(int width, int height);
	void SetOutputFiles(const std::string & imageFile, const std::string & mapFile);
	void SetInputFile(const std::string & fileName);
	void SetSpaceWidth(int width);
	void SetCharactersHeight(int height);
	void SetExtraMargin(int margin);
};

#endif