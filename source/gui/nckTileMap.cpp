
/**
 * nctoolkit, realtime aplication framework, under zlib software license.
 * https://github.com/nczeroshift/nctoolkit
 */

#include "nckTileMap.h"
#include "nckUtils.h"
#include "nckException.h"
#include "nckDataIO.h"
#include <stdio.h>

_GUI_BEGIN

TileMap::Tile::Tile()
{
    left = 0;
    right = 0;
    top = 0;
    bottom = 0;
    redline = 0;
}

TileMap::Tile::Tile(float l, float r,float t,float b,float rl)
{
    left = l;
    right = r;
    top = t;
    bottom = b;
    redline = rl;
}

TileMap::TileMap(Graph::Device *dev)
{
    m_gDevice = dev;
    m_Scale = 1.0;
    m_Margin = 2;
    //m_SpaceWidth = 16;
    //m_TabWidth = 16;
    //m_LineHeight = 16;
    //m_Monospaced = false;
}

TileMap::TileMap(Graph::Device *dev, float scale) {
    m_gDevice = dev;
    m_Scale = scale;
    m_Margin = 2;
}

TileMap::~TileMap(){
    
}

void TileMap::Load(const std::string & filename)
{
    char ch;
    Core::DataReader * reader = Core::DataReader::Open(filename);
    
    if(!reader)
        THROW_EXCEPTION("Unable to open the tile map file : \"" + filename + "\"");

    std::string line_buffer = "";

    int mode = 0;

    while(reader->Read(&ch,1) > 0)
    {
        if(ch == '\r')
            continue;

        if(ch == '\n')
        {
            if(line_buffer.size() > 1)
            {
                // Comment, ignore.
                if(line_buffer.substr(0,2) == "//"){
                    line_buffer = "";
                    continue;
                }

                std::string first_tag = line_buffer.substr(0,line_buffer.find(","));
                std::string right_str = line_buffer.substr(line_buffer.find(",")+1,line_buffer.length());
                                
                if (first_tag == "size")
                    sscanf_s(right_str.c_str(), "%f,%f", &m_Width, &m_Height);
                else if (first_tag == "margin")
                    m_Margin = atof(right_str.c_str());
                else if (first_tag == "cmap")
                    mode = 1;
                else if (first_tag == "imap")
                    mode = 2;
                else if (mode == 1)
                {
                    char ch_id;
                    float redline;
                    float left;
                    float right;
                    float top;
                    float bottom;
                    sscanf_s(line_buffer.c_str(), "%f,%f,%f,%f,%f,%c", &left, &top, &right, &bottom, &redline, &ch_id);

                    m_Map.insert(std::pair<unsigned int, Tile>((unsigned int)ch_id,
                        Tile(left,
                            right,
                            top,
                            bottom,
                            redline)));
                }
                else if (mode == 2)
                {
                    int tile_id;
                    float redline;
                    float left;
                    float right;
                    float top;
                    float bottom;
                    sscanf_s(line_buffer.c_str(), "%f,%f,%f,%f,%f,%d", &left, &top, &right, &bottom, &redline, &tile_id);

                    m_Map.insert(std::pair<unsigned int, Tile>(tile_id,
                        Tile(left,
                            right,
                            top,
                            bottom,
                            redline)));
                }
                else
                    m_Parameters.insert(std::pair<std::string, std::string>(first_tag, right_str));
                
                
                line_buffer = "";
            }    
        }
        else
        {
            // Remove tabs or spaces
            if(ch != '\t' && ch != ' ')
                line_buffer += ch;
        }
    }

    SafeDelete(reader);
}

bool TileMap::DrawTile(unsigned int tileId, float dx, float dy)
{
    TileMap::Tile c;
    
    std::map<unsigned int,Tile>::iterator entry = m_Map.find(tileId);

    if (entry == m_Map.end())
        return false;

    c = entry->second;
    
    float left = c.left;
    float top = c.top;
    float right = c.right;
    float bottom = c.bottom;

    float uv_s_x = (left - m_Margin) / (float)m_Width;
    float uv_s_y = (top - m_Margin) / (float)m_Height;

    float uv_e_x = (right + m_Margin) / (float)m_Width;
    float uv_e_y = (bottom + m_Margin) / (float)m_Height;

    float x_size = (right + m_Margin) - (left - m_Margin);
    float y_size = (bottom + m_Margin) - (top - m_Margin);

    float aspect = x_size / y_size;

    float c_size = x_size;

    float ox = dx;
    float oy = dy - c.redline;

    m_gDevice->Begin(Graph::PRIMITIVE_QUADS);

    m_gDevice->TexCoord(uv_s_x, uv_s_y);
    m_gDevice->Vertex(ox, oy, 0);

    m_gDevice->TexCoord(uv_s_x, uv_e_y);
    m_gDevice->Vertex(ox, oy + c_size / aspect, 0);

    m_gDevice->TexCoord(uv_e_x, uv_e_y);
    m_gDevice->Vertex(ox + c_size, oy + c_size / aspect, 0);

    m_gDevice->TexCoord(uv_e_x, uv_s_y);
    m_gDevice->Vertex(ox + c_size, oy, 0);

    m_gDevice->End();

    return true;
}


_GUI_END
