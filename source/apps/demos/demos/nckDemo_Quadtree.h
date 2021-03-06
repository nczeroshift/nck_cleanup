
/**
* nctoolkit, realtime aplication framework, under zlib software license.
* https://github.com/nczeroshift/nctoolkit
*/

#ifndef _NCK_DEMO_QUADTREE_H_
#define _NCK_DEMO_QUADTREE_H_

#include "../nckDemo.h"

class Demo_Quadtree : public Demo{
public:
    Demo_Quadtree(Core::Window * wnd, Graph::Device * dev);
    ~Demo_Quadtree();

    void Load();
    void Render(float dt);
    void UpdateWndEvents();
    std::vector<std::string> GetKeywords();
    std::string GetDescription();

private:
    float time;
};

Demo * CreateDemo_Quadtree(Core::Window * wnd, Graph::Device * dev);

#endif
