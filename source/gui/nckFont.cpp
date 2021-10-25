
/**
 * nctoolkit, realtime aplication framework, under zlib software license.
 * https://github.com/nczeroshift/nctoolkit
 */

#include "nckFont.h"
#include "nckUtils.h"
#include "nckException.h"
#include "nckDataIO.h"
#include <stdio.h>

_GUI_BEGIN

/// Constructor.
Font::Font(Graph::Device *dev) : TileMap(dev){
    m_Monospaced = false;
    m_LineHeight = 16;
    m_SpaceWidth = 16;
    m_TabWidth = 16;
    m_Texture = NULL;
}

/// Constructor.
Font::Font(Graph::Device *dev, float scale) : TileMap(dev, scale) {

}

/// Destructor.
Font::~Font() {
    SafeDelete(m_Texture);
}

void Font::Load(const std::string & fontName) {

    std::string font_map = "script://map_" + fontName + ".txt";
    std::string texture_file = "texture://tex2d_" + fontName + ".png";

    TileMap::Load(font_map);
    m_Texture = dynamic_cast<Graph::Texture2D*>(m_gDevice->LoadTexture(texture_file));

    MapFor(std::string, std::string, m_Parameters, i) {
        if (i->first == "space")
            m_SpaceWidth = atof(i->second.c_str());
        else if (i->first == "tab")
            m_TabWidth = atof(i->second.c_str());
        else if (i->first == "height")
            m_LineHeight = atof(i->second.c_str());
        else if (i->first == "monspaced")
            m_Monospaced = i->second == "true";
    }
}

void Font::Draw(float x, float y, const std::string & str, uint8_t align)
{
    m_Texture->Enable();

    x *= m_Scale;
    y *= m_Scale;

    float dx = x;
    float dy = y;

    std::string temp_str = str;

    if ((align & FONT_ALIGNMENT_CENTER) == FONT_ALIGNMENT_CENTER)
    {
        if (temp_str.find("\n") != std::string::npos)
            dx -= (int)(GetLength(temp_str.substr(0, temp_str.find("\n"))) / 2);
        else
            dx -= (int)(GetLength(temp_str) / 2);
    }

    if ((align & FONT_ALIGNMENT_RIGHT) == FONT_ALIGNMENT_RIGHT)
    {
        if (temp_str.find("\n") != std::string::npos)
            dx -= GetLength(temp_str.substr(0, temp_str.find("\n")));
        else
            dx -= GetLength(temp_str);
    }
    
    const bool align_top = (align & FONT_ALIGNMENT_TOP) == FONT_ALIGNMENT_TOP;
    const bool align_bottom = (align & FONT_ALIGNMENT_BOTTOM) == FONT_ALIGNMENT_BOTTOM;
    const bool align_middle = (align & FONT_ALIGNMENT_MIDDLE) == FONT_ALIGNMENT_MIDDLE;

    for (unsigned int i = 0; i<temp_str.length(); i++)
    {
        char ch = str[i];
        

        if (ch == '\n')
        {
            dx = x;

            if (i + 1 < temp_str.length()) {
                std::string sStr = temp_str.substr(i + 1);

                if ((align & FONT_ALIGNMENT_CENTER) == FONT_ALIGNMENT_CENTER)
                {
                    if (sStr.find("\n") != std::string::npos)
                        dx -= GetLength(sStr.substr(i, sStr.find("\n"))) / 2;
                    else
                        dx -= GetLength(sStr) / 2;
                }

                if ((align & FONT_ALIGNMENT_RIGHT) == FONT_ALIGNMENT_RIGHT)
                {
                    if (sStr.find("\n") != std::string::npos)
                        dx -= GetLength(sStr.substr(i, sStr.find("\n")));
                    else
                        dx -= GetLength(sStr);
                }
            }

            dy += m_LineHeight;

            continue;
        }
        
        if (ch == ' ') {
            dx += m_SpaceWidth;
            continue;
        }

        if (ch == '\t') {
            dx += m_TabWidth;
            continue;
        }

        std::map<unsigned int, Tile>::iterator entry = m_Map.find((int)ch);

        if (entry == m_Map.end())
            continue;

        float ddx = dx;

        float char_width = (entry->second.right + m_Margin) - (entry->second.left - m_Margin);

        if(m_Monospaced)
            ddx += (int)(m_SpaceWidth / 2) - (int)(char_width / 2);
            
        float ddy = dy;

        if(align_middle)
            ddy += m_LineHeight / 2;

        if (align_bottom)
            ddy += m_LineHeight;

        DrawTile(ch, ddx, ddy);

        if (m_Monospaced)
            dx += m_SpaceWidth + 1;
        else
            dx += (int)(entry->second.right - entry->second.left) + 1;
    
    }

    m_Texture->Disable();
}

float Font::GetLength(const std::string & str)
{
    float dx = 0;
    float maxWidth = 0;

    for (unsigned int i = 0; i<str.length(); i++)
    {
        unsigned int ch = (unsigned int)str[i];

        TileMap::Tile c;

        if (ch == ' ') {
            dx += m_SpaceWidth;
            continue;
        }

        if (ch == '\t') {
            dx += m_TabWidth;
            continue;
        }

        if (ch == '\n') {
            maxWidth = MAX(maxWidth, dx);
            dx = 0;
            continue;
        }

        std::map<unsigned int, Tile>::iterator entry;

        entry = m_Map.find(ch);

        if (entry == m_Map.end())
            continue;

        c = entry->second;

        dx += (int)(c.right - c.left) + 1;
    }

    return MAX(dx, maxWidth);
}

_GUI_END
