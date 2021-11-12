#include "Utils.h"

#include "nckUtils.h"

#include <iostream>
#include <string>
#include <initializer_list>
#include <sstream>
#include <iomanip>

void drawLine(Graph::Device* dev, float x, float y, float x2, float y2) {
    dev->Begin(Graph::PRIMITIVE_LINES);
    dev->Vertex(x, y, 0);
    dev->Vertex(x2, y2, 0);
    dev->End();
}

void drawRectXY(Graph::Device * dev, float x, float y, float x2, float y2) {
	dev->Begin(Graph::PRIMITIVE_QUADS);

	dev->TexCoord(0, 0);
	dev->Vertex(x, y, 0);
	dev->TexCoord(0, 1);
	dev->Vertex(x, y2, 0);
	dev->TexCoord(1, 1);
	dev->Vertex(x2, y2, 0);
	dev->TexCoord(1, 0);
	dev->Vertex(x2, y, 0);

	dev->End();
}

void renderViewportBounds(Graph::Device* dev, const float width, const float height)
{
    dev->MatrixMode(Graph::MATRIX_PROJECTION); dev->Identity();
    dev->Ortho2D(width, height);

    dev->MatrixMode(Graph::MATRIX_VIEW); dev->Identity();
    dev->MatrixMode(Graph::MATRIX_MODEL); dev->Identity();

    dev->FillMode(Graph::FILL_SOLID);
    dev->Enable(Graph::STATE_BLEND);
    dev->BlendFunc(Graph::BLEND_SRC_ALPHA, Graph::BLEND_INV_SRC_ALPHA);

    dev->Color(40, 40, 40, 255);
    drawLine(dev, 1, 1, width, 1);
    drawLine(dev, 1, 1, 1, height);
    drawLine(dev, 0, height, width, height);
    drawLine(dev, width, 0, width, height);

    dev->Color(160, 160, 160, 255);
    drawLine(dev, 2, 2, width - 2, 1);
    drawLine(dev, 2, 2, 2, height - 2);

    dev->Color(80, 80, 80, 255);
    drawLine(dev, 2, height - 1, width - 1, height - 1);
    drawLine(dev, width - 1, 0, width - 1, height - 2);
}


Core::Point updatePositionWithMouseWheel(
    Core::Point position,
    float& scale,
    short mouseWheel,
    unsigned int dimX,
    unsigned int dimY,
    Core::Point mousePosition,
    Core::Point mouseOffset)
{
    float next_scale = scale + mouseWheel / 10.0;

    if (next_scale > 0.01 && next_scale < 10)
    {
        float s_x = scale * dimX;
        float s_y = scale * dimY;

        float a_x = position.GetX() + mouseOffset.GetX();
        float a_y = position.GetY() + mouseOffset.GetY();

        float b_x = position.GetX() + mouseOffset.GetX() + s_x;
        float b_y = position.GetY() + mouseOffset.GetY() + s_y;

        float m_p_x = mousePosition.GetX();
        float m_p_y = mousePosition.GetY();

        float alpha_x = (m_p_x - a_x) / (b_x - a_x);
        float alpha_y = (m_p_y - a_y) / (b_y - a_y);

        float x = m_p_x - dimX * next_scale * alpha_x;
        float y = m_p_y - dimY * next_scale * alpha_y;

        scale = next_scale;

        return Core::Point(x, y);
    }

    return position;
}


/*
template<typename T>
std::string formatValue(const std::string& type, T t) {
    std::stringstream tmp;
    tmp << t;
    return tmp.str();
}

template<typename T, typename... Args>
std::string LogInt(const std::string& format, T t)
{
    size_t b1 = format.find_first_of("{");
    size_t b2 = format.find_first_of("}");

    if (b1 != -1 && b2 != -1) {
        std::string sub = format.substr(b1 + 1, b2 - b1 - 1);
        return format.substr(0, b1) + formatValue(sub, t) + format.substr(b2 + 1);
    }
    else
        return format;
}

template<typename T, typename... Args>
std::string LogInt(const std::string& format, T t, Args... args) {
    std::string ret = format;

    size_t b1 = format.find_first_of("{");
    size_t b2 = format.find_first_of("}");

    if (b1 != -1 && b2 != -1) {
        ret = format.substr(0, b1);

        std::string sub = format.substr(b1 + 1, b2 - b1 - 1);

        std::string rest = format.substr(b2 + 1);

        ret += formatValue(sub, t) + LogInt(rest, args...);
    }
    return ret;
}


template<typename T, typename... Args>
void Log(const std::string& format, T t, Args... args)
{
    std::string ret = LogInt(format, t, args...);
    Core::Log::Debug(ret);
    //std::stringstream ret; 
    //ret << t << std::endl;
    // std::cout << ret << std::endl;
    //std::cout << t << Log(args...) << std::endl;
}*/