
/**
* nctoolkit, realtime aplication framework, under zlib software license.
* https://github.com/nczeroshift/nctoolkit
*/

#ifndef _NCK_DEMO_RAYTRACE_TEXTURE_3D_H_
#define _NCK_DEMO_RAYTRACE_TEXTURE_3D_H_

#include "../nckDemo.h"

class Demo_RaytraceTexture3D : public Demo{
public:
    Demo_RaytraceTexture3D(Core::Window * wnd, Graph::Device * dev);
    ~Demo_RaytraceTexture3D();

    void Load();
    void Render(float dt);
    void UpdateWndEvents();
    std::vector<std::string> GetKeywords();
    std::string GetDescription();

private:
    Scene::Compound_Base * m_CubeScene;
    Graph::Program * m_Texture3DProg;
    Graph::Texture3D * m_Texture;
    Graph::Texture2D * m_Texture2;
    float time;
};

Demo * CreateDemo_RaytraceTexture3D(Core::Window * wnd, Graph::Device * dev);

#endif
