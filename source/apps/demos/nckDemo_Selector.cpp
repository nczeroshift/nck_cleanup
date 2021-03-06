
/**
* nctoolkit, realtime aplication framework, under zlib software license.
* https://github.com/nczeroshift/nctoolkit
*/

#include "nckDemo_Selector.h"
#include "nckVec4.h"

class DemoDetails{
public:
    DemoDetails();
    ~DemoDetails();

    void SetKeywords(const std::vector<std::string> & keywords);
    void AddKeyword(const std::string & keyword);
    //void SetPlatform(int p);
    void SetDescription(const std::string & description);
    bool SupportsCurrentPlatform();
    void SetScreenshotFilename(const std::string & filename);

    //std::vector<Platform> GetPlatforms() const;
    std::string GetDescription() const;
    std::vector<std::string> GetKeywords() const;
    std::string GetScreenshotFilename() const;
private:
    std::vector<std::string> m_Keywords;
    int m_Platforms;
    std::string m_Description;
    std::string m_ScreenshotFilename;
};

DemoDetails::DemoDetails(){

}

DemoDetails::~DemoDetails(){

}

std::string DemoDetails::GetDescription() const{
    return m_Description;
}

std::vector<std::string> DemoDetails::GetKeywords() const{
    return m_Keywords;
}

//std::vector<DemoDetails::Platform> DemoDetails::GetPlatforms() const{
//    return m_Platforms;
//}

void DemoDetails::SetKeywords(const std::vector<std::string> & keywords){
    m_Keywords = keywords;
}

void DemoDetails::SetDescription(const std::string & description){
    m_Description = description;
}

void DemoDetails::AddKeyword(const std::string & keyword){
    m_Keywords.push_back(keyword);
}

void DemoDetails::SetScreenshotFilename(const std::string & filename){
    m_ScreenshotFilename = filename;
}

std::string DemoDetails::GetScreenshotFilename() const{
    return m_ScreenshotFilename;
}

bool DemoDetails::SupportsCurrentPlatform(){
    //if(m_Platforms.size() == 0)
    //    return true;

    /*for(uint32_t i = 0;i<m_Platforms.size();i++){
        Platform p = m_Platforms[i];
        if(p == ALL)
            return true;
#if defined(NCK_WINDOWS)
        if(p == WINDOWS)
            return true;
#elif defined(NCK_LINUX)
        if(p == LINUX)
            return true;
#elif defined(NCK_MACOSX)
        if(p == MACOS)
            return true;
#endif
    */

    return false;
}

DemoSelector::DemoSelector(Core::Window * wnd, Graph::Device * dev){
    this->dev = dev;
    this->wnd = wnd;

    m_Font = NULL;
    m_Callback = NULL;
    m_IconNck = NULL;

    m_ScrollPosition = 0;
    m_ScrollVelocity = 0;
    m_ScrollOffset = 0;
    m_ActiveDemoId = 0;
    m_TotalDeslocation = 0;

    for (int i = 0; i < DemoCount(); i++) {
        DemoDetails * dd = new DemoDetails();

        Demo * demo = DemoCreate(i, wnd, dev);
        dd->SetDescription(demo->GetDescription());
        std::vector<std::string> keywords = demo->GetKeywords();
        for (size_t k = 0; k < keywords.size(); k++) {
            dd->AddKeyword(keywords[k]);
        }
        SafeDelete(demo);

        m_DemoDetails.push_back(dd);
    }
}

DemoSelector::~DemoSelector(){
    while(m_DemoDetails.size()>0){
        delete m_DemoDetails.back();
        m_DemoDetails.pop_back();
    }
    SafeDelete(m_Font);
    SafeDelete(m_IconNck);
}

void DemoSelector::Load(){    
    m_Font = new Gui::Font(dev);
    m_Font->Load("sans_serif_11");
    m_IconNck = dynamic_cast<Graph::Texture2D*>(dev->LoadTexture("texture://icons/tex2d_icon_nck.png"));
}

void DemoSelector::SetCallback(DemoSelector_Callback * callback){
    m_Callback = callback;
}

void DemoSelector::RenderSquare(float x,float y,float w, float h, const Math::Color4ub & col1){
    dev->Color(col1.GetR(),col1.GetG(),col1.GetB(),col1.GetA());
    dev->Begin(Graph::PRIMITIVE_QUADS);
    dev->TexCoord(0,0);
    dev->Vertex(x,y);
    dev->TexCoord(0,1);
    dev->Vertex(x,y+h);
    dev->TexCoord(1,1);
    dev->Vertex(x+w,y+h);
    dev->TexCoord(1,0);
    dev->Vertex(x+w,y);
    dev->End();
}

void DemoSelector::Render(float dt){
    dev->Enable(Graph::STATE_BLEND);
    dev->BlendFunc(Graph::BLEND_SRC_ALPHA, Graph::BLEND_INV_SRC_ALPHA);
    dev->Disable(Graph::STATE_DEPTH_TEST);

    dev->ClearColor(0.0, 0.0, 0.0, 1.0);

    dev->Viewport(0,0,wnd->GetWidth(),wnd->GetHeight());
    dev->Clear();
    
    dev->MatrixMode(Graph::MATRIX_PROJECTION);
    dev->Identity();
    dev->Ortho2D((float)wnd->GetWidth(),(float)wnd->GetHeight());

    dev->MatrixMode(Graph::MATRIX_MODEL);
    dev->Identity();

    Core::Point p = wnd->GetMousePosition();

    m_ScrollPosition += m_ScrollOffset + m_ScrollVelocity;

    m_ScrollVelocity *= 0.95f;

    const float cellHeight = 50.0f;
    const float cellSplitHeight = 1.0f;
    const float margin = 10.0f;
    const float demoSectionWidth = 110.0f;

    if(m_ScrollPosition < -(cellHeight+cellSplitHeight)*m_DemoDetails.size() + (float)wnd->GetHeight()-margin*2){
        m_ScrollPosition = -(cellHeight+cellSplitHeight)*m_DemoDetails.size() + (float)wnd->GetHeight()-margin*2;
    }

    if(m_ScrollPosition > 0){
        m_ScrollPosition = 0;
    }

    for(uint32_t i = 0;i<m_DemoDetails.size();i++)
    {
        dev->PushMatrix();
        dev->Translate(margin,m_ScrollPosition+i*(cellHeight+cellSplitHeight)+margin,0);

        Math::Color4ub bgColor = Math::Color4ub(80,80,80,150);

        if(p.GetY() > (m_ScrollPosition+i*(cellHeight+cellSplitHeight)+margin) && 
            p.GetY() < (m_ScrollPosition+i*(cellHeight+cellSplitHeight)+margin+cellHeight) && 
            p.GetX() > margin && 
            p.GetX() < wnd->GetWidth() - margin)
        {
            m_ActiveDemoId = i;
            bgColor = Math::Color4ub(50,50,50,150);
        }
        
        float demoTitleX = wnd->GetWidth()-margin*2-demoSectionWidth;
        RenderSquare(0,0,cellHeight,cellHeight,bgColor);
        RenderSquare(demoTitleX,0,demoSectionWidth,cellHeight,bgColor);
        RenderSquare(cellHeight+cellSplitHeight,0,wnd->GetWidth()-margin*2-(demoSectionWidth+cellSplitHeight*2+cellHeight),cellHeight,bgColor);

        std::string text = m_DemoDetails[i]->GetDescription();

        dev->Color(125,125,125);
        m_Font->Draw(cellHeight+margin,18,text);

        dev->Color(180, 100,40);
        m_Font->Draw(demoTitleX + 10, 25, "Demo " + Math::IntToString(i+1), Gui::FONT_ALIGNMENT_LEFT);

        std::vector<std::string> keywords = m_DemoDetails[i]->GetKeywords();
        for(int j = 0,x = 0;j<(int)keywords.size();j++){
            dev->Color(149,53,19);
            m_Font->Draw(cellHeight+margin+x,cellHeight/2+16/2+3,keywords[j]);
            x += (int)m_Font->GetLength(keywords[j])+10;
        }
        
        m_IconNck->Enable();
        RenderSquare(9,9,32,32,Math::Color4ub());
        m_IconNck->Disable();

        dev->PopMatrix();
    }

    dev->PresentAll();
}

void DemoSelector::UpdateWndEvents()
{
    static bool didFirstPress = true;
    
    if(wnd->GetMouseButtonStatus(Core::MOUSE_BUTTON_LEFT) == Core::BUTTON_STATUS_DOWN)
    {
        if(didFirstPress){
            m_LastCursorPosition = wnd->GetMousePosition();
            didFirstPress = false;
        }
    }
    else
    {
        didFirstPress = true;
        if(m_LastCursorPosition.GetY() > 0)
        {
            m_LastCursorPosition = Core::Point(-1,-1);
            m_ScrollOffset = 0;
            m_ScrollVelocity = 0;

            float weight = 1.0;
        
            while(m_ScrollVelocitySamples.size() > 0)
            {
                m_ScrollVelocity += m_ScrollVelocitySamples.back() * weight;
                weight *= 0.5;
                m_ScrollVelocitySamples.pop_back();
            }
            
            m_ScrollVelocitySamples.clear();
            
            if(m_TotalDeslocation == 0){
                if(m_Callback != NULL){
                    wnd->SetTitle("Demo #" + Math::IntToString(m_ActiveDemoId+1) + " - " + m_DemoDetails[m_ActiveDemoId]->GetDescription());
                    m_Callback->SelectDemo(m_ActiveDemoId);
                }
            }

            m_TotalDeslocation = 0;
        }
    }
    
    if(m_LastCursorPosition.GetY() > 0)
    {
        Core::Point pos = wnd->GetMousePosition();
        
        m_ScrollOffset = pos.GetY() - m_LastCursorPosition.GetY();
        m_TotalDeslocation += ABS(m_ScrollOffset);
        m_ScrollVelocitySamples.push_back(m_ScrollOffset);

        if(m_ScrollVelocitySamples.size()>5)
            m_ScrollVelocitySamples.pop_back();

        m_LastCursorPosition = pos;
    }
    else
    {
        m_ScrollOffset = 0;
    }
    
}

std::vector<std::string> DemoSelector::GetKeywords() {
    return std::vector<std::string>();
}

std::string DemoSelector::GetDescription() {
    return "";
}

Demo * CreateDemo_Selector(Core::Window * wnd, Graph::Device * dev){
    return new DemoSelector(wnd,dev);
}
