
/**
 * NCtoolKit © 2007-2015 Luís F.Loureiro, under zlib software license.
 * https://github.com/nczeroshift/nctoolkit
 */

#include "nckGraphics_glx2.h"
#include "nckUtils.h"

_GRAPHICS_BEGIN

/**
* Shader Program Proxy Manager Class
* Avoids shader programs resource duplication and shares the same GPU resource with 
* different properties (variables states).
*/ 
class ProgramProxyManager_GLX2{
public:
    /**
    * Program proxy manager constructor.
    * @param resName Resource name (filename for example).
    * @param dev Rendering Device Reference.
    */
    ProgramProxyManager_GLX2(const std::string & resName);

    /**
    * Release non-dealocated textures.
    */
    ~ProgramProxyManager_GLX2();

    /**
    * Add program to queue.
    */
    void Add(Program_GLX2 * tex);

    /**
    * Remove program from queue.
    * @return Returns true if the program isn't shared, false otherwise.
     */
    bool Remove(Program_GLX2 * tex);

    /**
    * Get queue size.
    */
    int GetSize();

    /**
    * Get the first program in the queue to serve as a reference.
    */ 
    Program_GLX2 * GetReference();

    /**
    *
    */
    std::string GetResourceName();
    
private:
    /**
    * Unique name to identify the common GPU object. 
    */
    std::string m_ResourceName;

    /**
    * List of resources sharing a common GPU object.
    */
    std::list<Program_GLX2*> m_Collection;
};


/**
* Texture Proxy Manager Class.
* Avoids texture resource duplication and shares the same GPU resource with 
* different properties (filtering for example).
*/ 
class TextureProxyManager_GLX2{
public:
    /**
    * Texture proxy manager constructor.
    * @param resName Resource name (filename for example).
    * @param dev Rendering Device Reference.
    */
    TextureProxyManager_GLX2(const std::string & resName);

    /**
    * Release non-dealocated textures.
    */
    ~TextureProxyManager_GLX2();

    /**
    * Add texture to queue.
    */
    void Add(Texture_GLX2 * tex);

    /**
    * Remove texture from queue.
    * @return Returns true if the texture isn't shared, false otherwise.
     */
    bool Remove(Texture_GLX2 * tex);

    /**
    * Get queue size.
    */
    int GetSize();

    /**
    * Get the first texture in the queue to serve as a reference.
    */ 
    Texture_GLX2 * GetReference();

    /**
    *
    */
    std::string GetResourceName();

private:
    /**
    * Unique name to identify the common GPU object. 
    */
    std::string m_ResourceName;

    /**
    * List of resources sharing a common GPU object.
    */
    std::list<Texture_GLX2*> m_Collection;
};

_GRAPHICS_END
