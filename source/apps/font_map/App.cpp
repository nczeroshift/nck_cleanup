#include "App.h"
#include <vector>
#include <map>
#include <algorithm>
#include "nckGraphicsUtils.h"
#include "Algorithms.h"
#include "nckLog.h"
#include "nckStringUtils.h"


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
    m_Scale = 1.0;

    m_TextScale = 1.0;
}

App::~App() {
	SafeDelete(characters_mutex);
	SafeDelete(src_img);
}

void App::SetSourceImage(const std::string & path) {
	src_img_path = path;
}

void App::DetectRows() {
    lineHeights.clear();
    
    if (m_DetectFirstLine != -1 && m_DetectLineHeight != -1) {
        
        int i = 0;
        int currentY = 0;
        do {
            currentY = m_DetectFirstLine + m_DetectLineHeight * i;
            lineHeights.push_back(currentY);
            i++;
        }while(currentY < src_img->GetHeight());

    }
    else {
        Algorithm_DetectRows(&characters, &lineHeights);
    }

}

void App::SortCharacters() {
	Algorithm_SortAndFix(&characters, lineHeights);
	m_SpaceWidth = Algorithm_GetMaxCharactersWidth(&characters)/2;

	mapped_characters.clear();
	ListFor(CharacterBounds, characters, i) {
        if (mapped_characters.find((char)i->GetCode()) != mapped_characters.end())
        {
            Core::Log::Debug("Meh");
        }

		mapped_characters.insert(std::pair<char, CharacterBounds>((char)i->GetCode(), *i));
	}

	for (int i = 0; i < lineHeights.size(); i++) {
		Core::Log::Debug("Line (" + Core::StringWithInt(i) + ") = " + Core::StringWithInt(lineHeights[i]) + "\n");
	}

	int max_height = 0;
	ListFor(CharacterBounds, characters, i) {
		max_height = MAX(i->GetLineHeight() - i->GetMinY(), max_height);
	}

	m_CharactersHeight = max_height;

	Core::Log::Debug("Characters max height = " + Core::StringWithInt(max_height) + "\n");

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
		src_img_tex->SetFilter(Graph::FILTER_MAGNIFICATION, Graph::FILTER_LINEAR);
		src_img_tex->SetFilter(Graph::FILTER_MINIFICATION, Graph::FILTER_LINEAR);
        src_img_tex->SetFilter(Graph::FILTER_MIPMAPPING, Graph::FILTER_NEAREST);

		Core::Log::Debug("Detecting characters...\n");
		DetectCharactersBounds(src_img, detect_margin, &characters);

		Core::Log::Debug("Detection rows...\n");
		DetectRows();

		Core::Log::Debug("Sorting characters...\n");
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

        float wnd_h = wnd->GetHeight() - 200;
        float wnd_w = wnd->GetWidth();

		const Core::Point cursor = wnd->GetMousePosition();
		dev->Viewport(0, 0, wnd->GetWidth(), wnd_h);

		dev->Clear();

		dev->MatrixMode(Graph::MATRIX_PROJECTION); dev->Identity();
		dev->Ortho2D((float)wnd->GetWidth(), (float)wnd->GetHeight()-200);

		dev->MatrixMode(Graph::MATRIX_VIEW); dev->Identity();
		dev->MatrixMode(Graph::MATRIX_MODEL); dev->Identity();

		dev->FillMode(Graph::FILL_SOLID);
		dev->Enable(Graph::STATE_BLEND);
		dev->BlendFunc(Graph::BLEND_SRC_ALPHA, Graph::BLEND_INV_SRC_ALPHA);

        src_img_tex->SetFilter(Graph::FILTER_MAGNIFICATION, Graph::FILTER_NEAREST);
        src_img_tex->SetFilter(Graph::FILTER_MINIFICATION, Graph::FILTER_NEAREST);
        src_img_tex->SetFilter(Graph::FILTER_MIPMAPPING, Graph::FILTER_NEAREST);

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

        renderViewportBounds(dev, wnd->GetWidth(), wnd->GetHeight() - 200);

        dev->Viewport(0, wnd->GetHeight()-200, wnd->GetWidth(),200);

        dev->MatrixMode(Graph::MATRIX_PROJECTION); dev->Identity();
        dev->Ortho2D(wnd->GetWidth(),200);

        dev->MatrixMode(Graph::MATRIX_VIEW); dev->Identity();
        dev->MatrixMode(Graph::MATRIX_MODEL); dev->Identity();
        dev->FillMode(Graph::FILL_SOLID);
        dev->Enable(Graph::STATE_BLEND);
        dev->BlendFunc(Graph::BLEND_SRC_ALPHA, Graph::BLEND_INV_SRC_ALPHA);
		
      
        dev->Color(255, 255, 255, 255);
        dev->PushMatrix();
        dev->Translate(m_TextPosition.GetX(), m_TextPosition.GetY(), 0);
        dev->Scale(m_TextScale, m_TextScale, 1.0);

        src_img_tex->SetFilter(Graph::FILTER_MAGNIFICATION, Graph::FILTER_LINEAR);
        src_img_tex->SetFilter(Graph::FILTER_MINIFICATION, Graph::FILTER_LINEAR);
        src_img_tex->SetFilter(Graph::FILTER_MIPMAPPING, Graph::FILTER_LINEAR);

        RenderText(10, 32, 16, "Veja o extravagante salto da raposa sobre o cachorro que dorme feliz!"
            "áéíóúàèìòù*+_-;,:.-~^1234567890|\\!\"#$%&/(){}[]='?»«><`´"
            "dev->Vertex(ox, oy + c_size / aspect, 0);");

        dev->PopMatrix();

        renderViewportBounds(dev, wnd->GetWidth(), 200);

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

	/*if (selection_a != characters.end()) {
		dev->FillMode(Graph::FILL_SOLID);
		dev->Color(255, 255, 255);
		src_img_tex->Enable();
		drawRectXYBounds2(dev, 10, wnd->GetHeight() - 128, 64, *selection_a, src_img_tex->GetWidth());
		src_img_tex->Disable();

		dev->FillMode(Graph::FILL_WIREFRAME);
		dev->Color(255, 0, 0);
		drawRectXYBounds(dev, 10, wnd->GetHeight() - 128, 64, *selection_a, src_img_tex->GetWidth());

		dev->FillMode(Graph::FILL_SOLID);
	}*/
}


void App::UpdateWndInput()
{
	m_UpdateCount = 30;

    Core::Point point = wnd->GetMousePosition();

    short mouseWheel = wnd->GetMouseWheel();

    const bool isOverText = point.GetY() > wnd->GetHeight() - 200;

    if (mouseWheel != 0) 
    {
        if (isOverText)
        {
            Core::Point pp = Core::Point(point.GetX(), point.GetY() - (wnd->GetHeight() - 200));

            m_TextPosition = updatePositionWithMouseWheel(
                m_TextPosition,
                m_TextScale,
                mouseWheel,
                wnd->GetWidth(),
                200,
                pp,
                Core::Point(0, 0)
            );
        }
        else
        {
            if (src_img_tex != NULL)
                m_Position = updatePositionWithMouseWheel(
                    m_Position,
                    m_Scale,
                    mouseWheel,
                    src_img_tex->GetWidth(),
                    src_img_tex->GetHeight(),
                    point,
                    Core::Point(0, 0)
                );
        }
       
       
        
    }

	if (wnd->GetKeyStatus(Core::KEY_ESCAPE) != Core::BUTTON_STATUS_UP) {
		Teardown();
	}

	if (wnd->GetKeyStatus(Core::KeyboardButton::KEY_Z) == Core::BUTTON_STATUS_UP) {
		if (wasZDown) {
            Core::Log::Debug("Save image \"" + dst_img_path + "\"\n");
			SaveOutput();
		}
		wasZDown = false;
	}
	else
		wasZDown = true;

	if (wnd->GetKeyStatus(Core::KeyboardButton::KEY_SPACE) == Core::BUTTON_STATUS_UP) {
		if (wasESCDown) {
			Core::Log::Debug("Clear Selection\n");
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
            if(isOverText)
                m_TextPosition = Core::Point(m_TextPosition.GetX() + m_MouseOffset.GetX(), m_TextPosition.GetY() + m_MouseOffset.GetY());
            else
                m_Position = Core::Point(m_Position.GetX() + m_MouseOffset.GetX(), m_Position.GetY() + m_MouseOffset.GetY());

            m_MouseOffset = Core::Point();

			characters_mutex->Lock();

			if (selection_current == characters.end() && selection_ln == -1 && selection_current_ln != -1)
				selection_ln = selection_current_ln;
			else if (selection_ln != -1 && selection_current != characters.end()) {
				lineHeights[selection_ln] = selection_current->GetMaxY();
				selection_current = characters.end();
				//DetectRows();
                SortCharacters();

				selection_ln = -1;
				selection_current_ln = -1;
			}

			if (selection_current != characters.end()) {
				if (selection_a == characters.end()) {
					selection_a = selection_current;
					Core::Log::Debug("Select 1\n");
				}
				else if (selection_current != selection_a && selection_b == characters.end()) {
					selection_b = selection_current;
					Core::Log::Debug("Select 2 - Merge\n");

					selection_a->Expand(*selection_b);
					characters.erase(selection_b);

					selection_a = characters.end();
					selection_b = characters.end();
					selection_current = characters.end();

					//DetectRows(false);
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