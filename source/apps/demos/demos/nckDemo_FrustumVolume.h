
/**
* nctoolkit, realtime aplication framework, under zlib software license.
* https://github.com/nczeroshift/nctoolkit
*/

#ifndef _NCK_DEMO_FRUSTUM2_H_
#define _NCK_DEMO_FRUSTUM2_H_

#include "../nckDemo.h"

class Demo_FrustumVolume : public Demo{
public:
    Demo_FrustumVolume(Core::Window * wnd, Graph::Device * dev);
    ~Demo_FrustumVolume();

    void Load();
    void Render(float dt);
    void UpdateWndEvents();
    std::vector<std::string> GetKeywords();
    std::string GetDescription();

private:
    Scene::Compound_Base * models;
    Gui::Font * font;
    float time;
};

Demo * CreateDemo_FrustumVolume(Core::Window * wnd, Graph::Device * dev);

#endif
