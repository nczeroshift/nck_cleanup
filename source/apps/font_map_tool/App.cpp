#include "App.h"
#include <vector>
#include <map>
#include <algorithm>
#include "nckGraphicsUtils.h"
#include "Algorithms.h"


void drawRectXYBounds(Graph::Device * dev, float x, float y, float width, const CharacterBounds & bounds, float srcSize) {
	float x2 = x + width;
	
	int chW = bounds.GetMaxX() - bounds.GetMinX();
	int chH = bounds.GetMaxY() - bounds.GetMinY();

	float scale = width / chW;

	float scaledMarginX = 0;// scale;
	float scaledMarginY = 0;// scale;
	
	float y2 = y + chH * width / (float)chW;

	dev->Begin(Graph::PRIMITIVE_QUADS);

	float u0 = (bounds.GetMinX()) / srcSize;
	float u1 = (bounds.GetMaxX()) / srcSize;

	float v0 = (bounds.GetMinY()) / srcSize;
	float v1 = (bounds.GetMaxY()) / srcSize;

	dev->TexCoord(u0, v0);
	dev->Vertex(x, y, 0);
	
	dev->TexCoord(u0, v1);
	dev->Vertex(x, y2, 0);
	
	dev->TexCoord(u1, v1);
	dev->Vertex(x2, y2, 0);
	
	dev->TexCoord(u1, v0);
	dev->Vertex(x2, y, 0);

	dev->End();
}



void drawRectXYBounds2(Graph::Device * dev, float x, float y, float width, const CharacterBounds & bounds, float srcSize) {
	float u0 = (bounds.GetMinX()) / srcSize;
	float u1 = (bounds.GetMaxX()) / srcSize;

	float v0 = (bounds.GetMinY()) / srcSize;
	float v1 = (bounds.GetMaxY()) / srcSize;

	float uW = bounds.GetMaxX() - bounds.GetMinX();
	float vH = bounds.GetMaxY() - bounds.GetMinY();
	
	float extra = 2;

	float x2 = x + width + extra * width / uW;

	float scale = width / uW;

	float scaledMarginX = 0;// scale;
	float scaledMarginY = 0;// scale;

	float height = vH * width / (float)uW;
	float y2 = y + height + extra * height / vH;

	dev->Begin(Graph::PRIMITIVE_QUADS);
		
	u0 = (bounds.GetMinX()) / srcSize;
	u1 = (bounds.GetMaxX()+extra) / srcSize;

	v0 = (bounds.GetMinY()) / srcSize;
	v1 = (bounds.GetMaxY()+ extra) / srcSize;

	dev->TexCoord(u0, v0);
	dev->Vertex(x, y, 0);

	dev->TexCoord(u0, v1);
	dev->Vertex(x, y2, 0);

	dev->TexCoord(u1, v1);
	dev->Vertex(x2, y2, 0);

	dev->TexCoord(u1, v0);
	dev->Vertex(x2, y, 0);

	dev->End();
}




App::App(Core::Window * window) {
	wnd = window;
	dev = NULL;

	characters_mutex = Core::CreateMutex();

	wasLMBDown = false;
	wasESCDown = false;
	wasZDown = false;
	m_CharactersHeight = 28;
	selection_a = characters.end();
	selection_b = characters.end();
	selection_current = characters.end();
	detect_margin = 2;

	dst_characters_margin = 4;

	src_img = NULL;

	src_img_path = ""; 
	dst_img_path = "";
	dst_map_path = "";

	dst_img_height = 256;
	dst_img_width = 256;

	m_DetectFirstLine = -1;
	m_DetectLineHeight = -1;
	m_SpaceWidth = 11;
	m_Monospaced = false;
	m_ExtraMargin = 0;
    m_Scale += 1.0;
}

App::~App() {
	SafeDelete(characters_mutex);
	SafeDelete(src_img);
}

void App::SetSourceImage(const std::string & path) {
	src_img_path = path;
}

void App::DetectRows() {
	Algorithm_DetectRows(&characters, &lineHeights);

	if (m_DetectFirstLine != -1 && m_DetectLineHeight != -1) {
		int count = lineHeights.size();
		lineHeights.clear();
		for (int i = 0; i < count; i++) {
			lineHeights.push_back(m_DetectFirstLine + m_DetectLineHeight * i);
		}
	}
}

void App::SortCharacters() {
	Algorithm_SortAndFix(&characters, lineHeights);
	m_SpaceWidth = Algorithm_GetMaxCharactersWidth(&characters)/2;

	mapped_characters.clear();
	ListFor(CharacterBounds, characters, i) {
		mapped_characters.insert(std::pair<char, CharacterBounds>((char)i->GetCode(), *i));
	}

	for (int i = 0; i < lineHeights.size(); i++) {
		Core::DebugLog("Line (" + Math::IntToString(i) + ") = " + Math::IntToString(lineHeights[i]) + "\n");
	}

	int max_height = 0;
	ListFor(CharacterBounds, characters, i) {
		max_height = MAX(i->GetLineHeight() - i->GetMinY(), max_height);
	}

	m_CharactersHeight = max_height;

	Core::DebugLog("Characters max height = " + Math::IntToString(max_height) + "\n");

}

void App::RenderText(float x, float y, float size, const std::string & text) {
	if (src_img_tex == NULL || mapped_characters.empty())
		return;

	src_img_tex->Enable();

	RenderTextExt(dev,
		src_img->GetWidth(), src_img->GetHeight(),
		x, y,
		m_SpaceWidth,
		m_CharactersHeight,
		dst_characters_margin,
		m_Monospaced,
		wnd->GetWidth(),
		text,
		m_ExtraMargin,
		mapped_characters);

	src_img_tex->Disable();
}

void App::OpenMap(const std::string & path) {
	Core::FileReader * fw = Core::FileReader::Open(path);

	char tmpChar;
	std::string line;
	int state = 0;

	while (fw->Read(&tmpChar, 1) > 0) {
		if (tmpChar != '\n') {
			line += tmpChar;
			continue;
		}

		if (line.find("version") == 0) {
			std::vector<std::string> tokens;
			int count = Core::StringSplit(line, ",", &tokens);
			int version = atoi(tokens[1].c_str());
		}
		else if (line.find("size") == 0) {
			std::vector<std::string> tokens;
			int count = Core::StringSplit(line, ",", &tokens);
			int width = atoi(tokens[0].c_str());
			int height = atoi(tokens[1].c_str());
		}
		else if (line.find("map") == 0) {
			state = 1;
		}
		else if (state == 1) {
			std::vector<std::string> tokens;
			int count = Core::StringSplit(line, ",", &tokens);
			if (count >= 5) {
				int min_x = atoi(tokens[0].c_str());
				int min_y = atoi(tokens[1].c_str());
				int max_x = atoi(tokens[2].c_str());
				int max_y = atoi(tokens[3].c_str());
				int line_height = atoi(tokens[4].c_str());
				int code = (int)tokens[5][0];

				CharacterBounds cb(min_x, min_y, max_x, max_y);
				cb.SetLineHeight(line_height);
				cb.SetCode(code);

				mapped_characters.insert(std::pair<int, CharacterBounds>(code, cb));
			}
		}

		line = "";
	}

	SafeDelete(fw);
}

void App::SaveOutput() {
	characters_mutex->Lock();
	std::list<CharacterBounds> tmp;
	SaveCharactersBitmap(src_img, dst_img_width, dst_img_height, dst_img_path, dst_characters_margin, &characters, &tmp);
	SaveCharactersMap(dst_img_width, dst_img_height, dst_map_path,m_Monospaced,m_SpaceWidth,m_CharactersHeight, dst_characters_margin, &tmp);
	characters_mutex->Unlock();
}

void App::Run() {
	try {
		//if (!Core::FileReader::Exists(src_img_path))
		//	THROW_EXCEPTION("Input file not found");

		src_img = Core::Image::Load(src_img_path);

		dev = Graph::CreateDevice(wnd, Graph::DEVICE_AUTO, wnd->GetWidth(), wnd->GetHeight());

		dev->ClearFlags(Graph::BUFFER_COLOR_BIT | Graph::BUFFER_DEPTH_BIT);
		dev->ClearColor(0.5, 0.5, 0.5);

		src_img_tex = dynamic_cast<Graph::Texture2D*>(dev->LoadTexture(src_img_path));
		src_img_tex->SetAdressMode(Graph::ADRESS_CLAMP);
		src_img_tex->SetFilter(Graph::FILTER_MAGNIFICATION, Graph::FILTER_NEAREST);
		src_img_tex->SetFilter(Graph::FILTER_MINIFICATION, Graph::FILTER_NEAREST);

		Core::DebugLog("Detecting characters...\n");
		DetectCharactersBounds(src_img, detect_margin, &characters);

		Core::DebugLog("Detection rows...\n");
		DetectRows();

		Core::DebugLog("Sorting characters...\n");
		SortCharacters();
	}
	catch (const Core::Exception & e) {
		e.PrintStackTrace();
		SafeDelete(dev);
		return;
	}

	while (!IsTearingDown())
	{
		if (m_UpdateCount == 0) {
			Core::Thread::Wait(64);
		}
		else
			m_UpdateCount--;

		const Core::Point cursor = wnd->GetMousePosition();
		dev->Viewport(0, 0, wnd->GetWidth(), wnd->GetHeight());

		dev->Clear();

		dev->MatrixMode(Graph::MATRIX_PROJECTION); dev->Identity();
		dev->Ortho2D((float)wnd->GetWidth(), (float)wnd->GetHeight());

		dev->MatrixMode(Graph::MATRIX_VIEW); dev->Identity();
		dev->MatrixMode(Graph::MATRIX_MODEL); dev->Identity();

		dev->FillMode(Graph::FILL_SOLID);
		dev->Enable(Graph::STATE_BLEND);
		dev->BlendFunc(Graph::BLEND_SRC_ALPHA, Graph::BLEND_INV_SRC_ALPHA);


		RenderGraphics(Math::Vec2(m_Position.GetX() + m_MouseOffset.GetX(), m_Position.GetY() + m_MouseOffset.GetY()),cursor, m_Scale);//Math::Vec2(
			//wnd->GetWidth() / 2 - scale * src_img->GetWidth() / 2,
			//wnd->GetHeight() / 2 - scale*src_img->GetHeight() / 2),
			//cursor, scale);


		dev->FillMode(Graph::FILL_SOLID);
		dev->Color(255, 200, 0, 100);
		{
			float x = (float)cursor.GetX();
			float y = (float)cursor.GetY();
			dev->Color(255, 0, 0, 255);
			drawRectXY(dev, x - 10, y - 1, x + 10, y + 2);
			drawRectXY(dev, x - 1, y - 10, x + 2, y + 10);
		}


		dev->Color(255, 255, 255, 255);
		dev->FillMode(Graph::FILL_SOLID);

		// Um Pangrama
		RenderText(10, 10, 16, "Veja o extravagante salto da raposa sobre o cachorro que dorme feliz!"
			"áéíóúàèìòù*+_-;,:.-~^1234567890|\\!\"#$%&/(){}[]='?»«><`´"
			"dev->Vertex(ox, oy + c_size / aspect, 0);");

		dev->PresentAll();
	}

	SafeDelete(dev);
}

void App::RenderGraphics(const Math::Vec2 & position, const Core::Point & cur, float scale) {
	dev->PushMatrix();
	dev->Translate(position.GetX(), position.GetY(), 0);

	float s_x = scale * src_img_tex->GetWidth();
	float s_y = scale * src_img_tex->GetHeight();

	Core::Point cursor(cur.GetX() - position.GetX(), cur.GetY() - position.GetY());

	src_img_tex->Enable();
	dev->Color(255, 255, 255);
	//drawRectXY(dev, 0, 0, s_x, s_y);
    Graph::Render_Square(dev,0,0,s_x,s_y);
	src_img_tex->Disable();

	characters_mutex->Lock();

	selection_current_ln = -1;
	for (int i = 0; i < lineHeights.size(); i++) {
		int y = scale * lineHeights[i];

		if (cursor.GetY() - 5 < y && cursor.GetY() + 5 > y) {
			selection_current_ln = i;
			dev->Color(255, 0, 255, 255);
		}
		else
			if (selection_ln == i)
				dev->Color(255, 255, 0, 255);
			else
				dev->Color(0, 255, 0, 255);

		dev->Begin(Graph::PRIMITIVE_LINES);
		dev->Vertex(0, y, 0);
		dev->Vertex(s_x, y, 0);
		dev->End();
	}


	{
		selection_current = characters.end();
		ListFor(CharacterBounds, characters, i) {
			float x = scale * i->GetMinX();
			float y = scale * i->GetMinY();
			float x2 = scale * (i->GetMaxX());
			float y2 = scale * (i->GetMaxY());

			float cx = (x + x2) / 2;
			float cy = (y + y2) / 2;

			bool fill = false;

			if (selection_a == i || selection_b == i) {
				fill = true;
				dev->FillMode(Graph::FILL_SOLID);
				dev->Color(0, 0, 255, 50);
				drawRectXY(dev, x, y, x2, y2);

				dev->Color(0, 0, 255, 200);
				dev->FillMode(Graph::FILL_WIREFRAME);
				drawRectXY(dev, x, y, x2, y2);
			}
			else if (cursor.GetX() >= x && cursor.GetX() <= x2 && cursor.GetY() >= y && cursor.GetY() <= y2) {
				dev->FillMode(Graph::FILL_SOLID);
				dev->Color(0, 255, 0, 50);
				drawRectXY(dev, x, y, x2, y2);

				dev->Color(0, 255, 0, 200);
				selection_current = i;
				dev->FillMode(Graph::FILL_WIREFRAME);
				drawRectXY(dev, x, y, x2, y2);
			}
			else {
				dev->FillMode(Graph::FILL_SOLID);
				if (i->GetLineHeight() == 0)
					dev->Color(255, 0, 255, 155);
				else
					dev->Color(255, 0, 0, 50);

				drawRectXY(dev, x, y, x2, y2);

				dev->Color(255, 0, 0, 200);
				dev->FillMode(Graph::FILL_WIREFRAME);
				drawRectXY(dev, x, y, x2, y2);
			}

			dev->Begin(Graph::PRIMITIVE_LINES);
			dev->Vertex(cx, cy, 0);
			dev->Vertex(cx, i->GetLineHeight()*scale, 0);
			dev->End();

		}	
	}

	characters_mutex->Unlock();
	dev->PopMatrix();

	if (selection_a != characters.end()) {
		dev->FillMode(Graph::FILL_SOLID);
		dev->Color(255, 255, 255);
		src_img_tex->Enable();
		drawRectXYBounds2(dev, 10, wnd->GetHeight() - 128, 64, *selection_a, src_img_tex->GetWidth());
		src_img_tex->Disable();

		dev->FillMode(Graph::FILL_WIREFRAME);
		dev->Color(255, 0, 0);
		drawRectXYBounds(dev, 10, wnd->GetHeight() - 128, 64, *selection_a, src_img_tex->GetWidth());

		dev->FillMode(Graph::FILL_SOLID);
	}
}




void App::UpdateWndInput()
{
	m_UpdateCount = 30;

    Core::Point point = wnd->GetMousePosition();

    short mouseWheel = wnd->GetMouseWheel();
    if (mouseWheel != 0) {
        
        float next_scale = m_Scale + mouseWheel / 10.0;

        if(src_img_tex != NULL){
            float s_x = m_Scale * src_img_tex->GetWidth();
            float s_y = m_Scale * src_img_tex->GetHeight();

            float a_x = m_Position.GetX() + m_MouseOffset.GetX();
            float a_y = m_Position.GetY() + m_MouseOffset.GetY();

            float b_x = m_Position.GetX() + m_MouseOffset.GetX() + s_x;
            float b_y = m_Position.GetY() + m_MouseOffset.GetY() + s_y;

            float m_p_x = wnd->GetMousePosition().GetX();
            float m_p_y = wnd->GetMousePosition().GetY();

            float alpha_x = (m_p_x - a_x) / (b_x - a_x);
            float alpha_y = (m_p_y - a_y) / (b_y - a_y);

            float x = m_p_x - src_img_tex->GetWidth() * next_scale * alpha_x;
            float y = m_p_y - src_img_tex->GetHeight() * next_scale * alpha_y;

            m_Position = Core::Point(x,y);
        }

        m_Scale = next_scale;

    }

	if (wnd->GetKeyStatus(Core::KEY_ESCAPE) != Core::BUTTON_STATUS_UP) {
		Teardown();
	}

	if (wnd->GetKeyStatus(Core::KeyboardButton::KEY_Z) == Core::BUTTON_STATUS_UP) {
		if (wasZDown) {
            Core::DebugLog("Save image \"" + dst_img_path + "\"\n");
			SaveOutput();
		}
		wasZDown = false;
	}
	else
		wasZDown = true;

	if (wnd->GetKeyStatus(Core::KeyboardButton::KEY_SPACE) == Core::BUTTON_STATUS_UP) {
		if (wasESCDown) {
			Core::DebugLog("Clear Selection\n");
			characters_mutex->Lock();
			selection_a = characters.end();
			selection_a = characters.end();
			characters_mutex->Unlock();
		}
		wasESCDown = false;
	}
	else
		wasESCDown = true;

	if (wnd->GetMouseButtonStatus(Core::MouseButton::MOUSE_BUTTON_LEFT) == Core::BUTTON_STATUS_UP)
	{
		if (wasLMBDown)
		{
            m_Position = Core::Point(m_Position.GetX()+m_MouseOffset.GetX(), m_Position.GetY() + m_MouseOffset.GetY());
            m_MouseOffset = Core::Point();

			characters_mutex->Lock();

			if (selection_current == characters.end() && selection_ln == -1 && selection_current_ln != -1)
				selection_ln = selection_current_ln;
			else if (selection_ln != -1 && selection_current != characters.end()) {
				lineHeights[selection_ln] = selection_current->GetMaxY();
				selection_current = characters.end();
				DetectRows();
				selection_ln = -1;
				selection_current_ln = -1;
			}

			if (selection_current != characters.end()) {
				if (selection_a == characters.end()) {
					selection_a = selection_current;
					Core::DebugLog("Select 1\n");
				}
				else if (selection_current != selection_a && selection_b == characters.end()) {
					selection_b = selection_current;
					Core::DebugLog("Select 2 - Merge\n");

					selection_a->Expand(*selection_b);
					characters.erase(selection_b);

					selection_a = characters.end();
					selection_b = characters.end();
					selection_current = characters.end();

					DetectRows();
					SortCharacters();
				}
			}
			characters_mutex->Unlock();
		}
		wasLMBDown = false;
	}
	else{
        if(!wasLMBDown){
            m_MouseDownStart = Core::Point(wnd->GetMousePosition().GetX(), wnd->GetMousePosition().GetY());
        }

		wasLMBDown = true;

        m_MouseOffset = Core::Point(wnd->GetMousePosition().GetX() - m_MouseDownStart.GetX(), wnd->GetMousePosition().GetY() - m_MouseDownStart.GetY());
    }
}

void App::SetFixedLine(int first, int height) {
	m_DetectFirstLine = first;
	m_DetectLineHeight = height;
}

void App::SetMonospace(bool monospaced) {
	m_Monospaced = monospaced;
}

void App::SetOutputMargin(int margin) {
	dst_characters_margin = margin;
}

void App::SetDetectMargin(int margin) {
	detect_margin = margin;
}

void App::SetOutputSize(int width, int height) {
	dst_img_width = width;
	dst_img_height = height;
}

void App::SetOutputFiles(const std::string & imageFile, const std::string & mapFile) {
	dst_img_path = imageFile;
	dst_map_path = mapFile;
}


void App::SetInputFile(const std::string & fileName) {
	src_img_path = fileName;
}

void App::SetSpaceWidth(int width) {
	m_SpaceWidth = width;
}

void App::SetCharactersHeight(int height) {
	m_CharactersHeight = height;
}

void App::SetExtraMargin(int margin) {
	m_ExtraMargin = margin;
}