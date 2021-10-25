
/**
 * nctoolkit, realtime aplication framework, under zlib software license.
 * https://github.com/nczeroshift/nctoolkit
 */

#ifndef NCK_TILE_MAP_H
#define NCK_TILE_MAP_H

#include "nckGuiConfig.h"
#include "nckGraphics.h"
#include <map>

_GUI_BEGIN

/**
* 
*/
class TileMap
{
public:
    /// Character cell position attributes.
    class Tile
    {
    public:
        Tile();
        Tile(float l, float r,float t,float b,float rl);

        float left,right;
        float top,bottom;
        float redline;
    };

    /// Constructor.
    TileMap(Graph::Device *dev);

    /// Constructor.
    TileMap(Graph::Device *dev, float scale);

    /// Destructor.
    ~TileMap();

    /**
    * Draw a single tile.
    * @param id Tile id.
    * @param dx Current x position.
    * @param dy Current y position.
    * @return False if the tile doesn't exist in this map, true otherwise.
    */
    bool DrawTile(unsigned int id, float dx, float dy);
        
    /**
    * Load tile map configuration from file.
    * return True if the file was correctly loaded, false otherwise.
    */
    void Load(const std::string & filename);

protected:
    /// Reference to the graphics device.
    Graph::Device * m_gDevice;

    // Map of tiles 
    std::map<unsigned int, Tile> m_Map;

    // Map of extra parameters
    std::map<std::string, std::string> m_Parameters;

    // Scale factor
    float m_Scale;
    
    // Source bitmap width
    float m_Width;

    // Source bitmap height
    float m_Height;

    // Number of pixels around each tile.
    float m_Margin;
};

_GUI_END

#endif // #ifndef NCK_FONT_H
