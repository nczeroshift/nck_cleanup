
/**
* nctoolkit, realtime aplication framework, under zlib software license.
* https://github.com/nczeroshift/nctoolkit
*/

#include "nckDemo_RaytraceTexture3D.h"

Demo_RaytraceTexture3D::Demo_RaytraceTexture3D(Core::Window * wnd, Graph::Device * dev){
    this->dev = dev;
    this->wnd = wnd;
    time = 0;
    m_Texture = NULL;
}

Demo_RaytraceTexture3D::~Demo_RaytraceTexture3D(){
    SafeDelete(m_Texture);
}

void Demo_RaytraceTexture3D::Load(){
    dev->Enable(Graph::STATE_DEPTH_TEST);

    dev->Enable(Graph::STATE_BLEND);
    dev->BlendFunc(Graph::BLEND_SRC_ALPHA, Graph::BLEND_INV_SRC_ALPHA);

    m_Texture = dynamic_cast<Graph::Texture3D*>(dev->LoadTexture("texture://tex3d_scan.tar"));

    m_Texture3DProg = dev->LoadProgram("shader://tex3d_raytrace.cpp");

    m_Texture2 = dynamic_cast<Graph::Texture2D*>(dev->LoadTexture("texture://tex2d_linear.png"));

    m_CubeScene = new Scene::Compound_Base(dev);
    m_CubeScene->Load("model://cube.bxon");
}

void Demo_RaytraceTexture3D::Render(float dt)
{
    Scene::Camera * cam = m_CubeScene->GetCamera("Camera");
    cam->SetAspect(wnd->GetWidth() / (float)wnd->GetHeight());

    dev->Clear();
    dev->Viewport(0, 0, wnd->GetWidth(), wnd->GetHeight());
    

    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();
    cam->Enable(Graph::MATRIX_PROJECTION);


    dev->MatrixMode(Graph::MATRIX_VIEW);
    dev->Identity();
    cam->Enable(Graph::MATRIX_VIEW);


    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();
    
    float longestAxis = Math::Max(m_Texture->GetWidth(), Math::Max(m_Texture->GetHeight(), m_Texture->GetDepth()));

    float volScale[] = { 
        m_Texture->GetWidth() / longestAxis, 
        m_Texture->GetHeight() / longestAxis,
        m_Texture->GetDepth() / longestAxis 
    };

    float posValues[] = {
        cam->GetObject()->GetPosition().GetX(),
        cam->GetObject()->GetPosition().GetY(),
        cam->GetObject()->GetPosition().GetZ(),
    };

    m_Texture3DProg->Enable();
    m_Texture3DProg->SetVariable4f("eye_pos",
        posValues[0],
        posValues[1],
        posValues[2],
        1.0);

    m_Texture3DProg->SetVariable4f("volume_scale",
        volScale[0],
        volScale[1],
        volScale[2],
        1.0);

    m_Texture3DProg->SetVariable4f("volume_dims",
        m_Texture->GetWidth(),
        m_Texture->GetHeight(),
        m_Texture->GetDepth(),
        1.0);
    
    m_Texture->Enable(0);
    m_Texture2->Enable(1);

    m_CubeScene->Render();

    m_Texture2->Disable(1);
    m_Texture->Disable(0);

    m_Texture3DProg->Disable();

    /*
    float z = (time - floor(time / m_Texture->GetDepth()) *  m_Texture->GetDepth())/(m_Texture->GetDepth());

    m_Texture3DProg->Enable();
    m_Texture3DProg->SetVariable1f("depth", z);

    float scale = 2.0;

    dev->PushMatrix();
    dev->Translate(wnd->GetWidth() * 0.5 - scale * m_Texture->GetWidth()*0.5
        , wnd->GetHeight() * 0.5 - scale* m_Texture->GetWidth()*0.5, 0.0);
    dev->Scale(scale, scale, 1);

    m_Texture->Enable();
    dev->Color(255, 255, 255, 255);
    dev->Begin(Graph::PRIMITIVE_QUADS);
    dev->TexCoord(0, 0);
    dev->Vertex(0, 0);
    dev->TexCoord(0, 1);
    dev->Vertex(0, m_Texture->GetHeight());
    dev->TexCoord(1, 1);
    dev->Vertex(m_Texture->GetWidth(), m_Texture->GetHeight());
    dev->TexCoord(1, 0);
    dev->Vertex(m_Texture->GetWidth(), 0);
    dev->End();
    m_Texture->Disable();

    dev->PopMatrix();

    m_Texture3DProg->Disable();
    */

    time += dt;

    // Finish rendering and present the graphics.
    dev->PresentAll();
}

void Demo_RaytraceTexture3D::UpdateWndEvents(){
    
}

std::vector<std::string> Demo_RaytraceTexture3D::GetKeywords() {
    std::vector<std::string> ret;
    ret.push_back("Basic");
    ret.push_back("Rendering");
    ret.push_back("Textures 3D");
    return ret;
}

std::string Demo_RaytraceTexture3D::GetDescription() {
    return "Raytrace 3d Texture";
}

Demo * CreateDemo_RaytraceTexture3D(Core::Window * wnd, Graph::Device * dev){
    return new Demo_RaytraceTexture3D(wnd,dev);
}
