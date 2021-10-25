
/**
 * nctoolkit, realtime aplication framework, under zlib software license.
 * https://github.com/nczeroshift/nctoolkit
 */

#ifndef NCK_FONT_H
#define NCK_FONT_H

#include "nckTileMap.h"

_GUI_BEGIN

/// Font alignment modes.
enum FontAlignment{
    FONT_ALIGNMENT_LEFT        = 1,
    FONT_ALIGNMENT_CENTER    = 2,
    FONT_ALIGNMENT_RIGHT    = 4,
    FONT_ALIGNMENT_TOP        = 8,
    FONT_ALIGNMENT_MIDDLE    = 16,
    FONT_ALIGNMENT_BOTTOM    = 32,
};

/**
* Characters map and rendering class.
*/
class Font: public TileMap
{
public:
    /// Constructor.
    Font(Graph::Device *dev);

    /// Constructor.
    Font(Graph::Device *dev, float scale);

    /// Destructor.
    ~Font();

    /**
    * Draw a text string.
    * @param x String start x position.
    * @param y String start y position.
    * @param str String to Render
    */
    void Draw(float x, float y, const std::string & str, uint8_t align = FONT_ALIGNMENT_LEFT);

    /**
    * Load character map configuration from file.
    * return True if the file was correctly loaded, false otherwise.
    */
    void Load(const std::string & filename);

    /// Get string length.
    float GetLength(const std::string & str);

private:
    Graph::Texture2D * m_Texture;
    bool m_Monospaced;
    float m_SpaceWidth;
    float m_TabWidth;
    double m_LineHeight;
};

_GUI_END

#endif
