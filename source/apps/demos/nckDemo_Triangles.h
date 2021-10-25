
/**
* nctoolkit, realtime aplication framework, under zlib software license.
* https://github.com/nczeroshift/nctoolkit
*/

#ifndef _NCK_DEMO_TRIANGLES_H_
#define _NCK_DEMO_TRIANGLES_H_

#include "../nckDemo.h"

class Demo_Triangles : public Demo{
public:
    Demo_Triangles(Core::Window * wnd, Graph::Device * dev);
    ~Demo_Triangles();

    void Load();
    void Render(float dt);
    void UpdateWndEvents();
    std::vector<std::string> GetKeywords();
    std::string GetDescription();

private:
    float time;
};

Demo * CreateDemo_Triangles(Core::Window * wnd, Graph::Device * dev);

#endif
