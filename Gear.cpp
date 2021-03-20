#include "Gear.hpp"
#include <cmath>

namespace
{

    sf::Vector2f getPoint(std::size_t index, std::size_t count, sf::Vector2f radius, sf::Vector2f offset = {0.f,0.f})
    {
        static float pi = 3.141592654f;

        float angle = index * 2 * pi / count - pi / 2;
        float x = std::cos(angle) * radius.x;
        float y = std::sin(angle) * radius.y;

        return sf::Vector2f(radius.x + x + offset.x, radius.y + y + offset.y);
    }

    sf::Vector2f getRectanglePoint(std::size_t index, sf::Vector2f size, sf::Vector2f offset = {0.f,0.f})
    {
    switch (index)
    {
        default:
        case 0: return sf::Vector2f(offset.x, offset.y);
        case 1: return sf::Vector2f(size.x + offset.x, offset.y);
        case 2: return sf::Vector2f(size.x + offset.x, size.y + offset.y);
        case 3: return sf::Vector2f(offset.x, size.y + offset.y);
    }
    }

    sf::Vector2f computeNormal(const sf::Vector2f& p1, const sf::Vector2f& p2)
    {
        sf::Vector2f normal(p1.y - p2.y, p2.x - p1.x);
        float length = std::sqrt(normal.x * normal.x + normal.y * normal.y);
        if (length != 0.f)
            normal /= length;
        return normal;
    }

    float dotProduct(const sf::Vector2f& p1, const sf::Vector2f& p2)
    {
        return p1.x * p2.x + p1.y * p2.y;
    }
}

namespace BLAK
{

Gear::~Gear()
{
}

void Gear::setFillColor(const sf::Color& color)
{
    m_fillColor = color;
    m_highlightColor = color;
    updateColors();
}

void Gear::setFillColor(const int r, const int g, const int b, const int a)
{
    m_fillColor = sf::Color(r,g,b,a);
    m_highlightColor = sf::Color(r,g,b,a);
    updateColors();
}

void Gear::setFillColor2nd(const sf::Color& color)
{
    m_fillColor2nd = color;
    updateSecondGearColor();
}

void Gear::setFillColor2nd(const int r, const int g, const int b, const int a)
{
    m_fillColor2nd = sf::Color(r,g,b,a);
    updateSecondGearColor();
}

void Gear::setHighlightColor(const sf::Color& color)
{
    m_highlightColor = color;
    updateColors();
}

void Gear::setHighlightColor(const int r, const int g, const int b, const int a)
{
    m_highlightColor = sf::Color(r,g,b,a);
    updateColors();
}

void Gear::setNumberTeeth(const std::size_t& teeth)
{
    m_teeth = teeth;
    m_pointCount = teeth * 2;
    if(teeth == 0) m_pointCount = 36;
    m_vertices.clear();
    update();
}

void Gear::setSize(const sf::Vector2f& size)
{
    m_size = size;
    m_vertices.clear();
    update();
}

void Gear::setSize(const float x, const float y)
{
    m_size = sf::Vector2f(x,y);
    m_vertices.clear();
    update();
}

void Gear::setSmooth(const bool& smooth)
{
    m_smooth = smooth;
    m_vertices.clear();
    update();
}

void Gear::setSolid(const bool& solid)
{
    m_solid = solid;
    m_vertices.clear();
    update();
}

void Gear::setStyle(const int& style)
{
    m_style = style;
    m_vertices.clear();
    update();
}

void Gear::setTwoGears(const sf::Vector2f& size, const float mesh, const std::size_t teeth, const bool ontop, const sf::Color color)
{
    m_twogears = true;
    m_twogeartop = ontop;
    m_size2nd = size;
    m_mesh2nd = mesh;
    m_teeth2nd = teeth;
    m_fillColor2nd = color;
    m_pointCount2nd = teeth * 2;
    updateSecondGear();
}

void Gear::setTwoGears(const float x, const float y, const float mesh, const std::size_t teeth, const bool ontop, const sf::Color color)
{
    m_twogears = true;
    m_twogeartop = ontop;
    m_size2nd = sf::Vector2f(x,y);
    m_mesh2nd = mesh;
    m_teeth2nd = teeth;
    m_fillColor2nd = color;
    m_pointCount2nd = teeth * 2;
    updateSecondGear();
}

Gear::Gear(const sf::Vector2f size, const float mesh, const std::size_t teeth)
: m_size(size)
, m_size2nd({0,0})
, m_teeth(teeth)
, m_teeth2nd(0)
, m_mesh(mesh)
, m_mesh2nd(0)
, m_highlights(1)
, m_style(0)
, m_fillColor({192,192,0})
, m_fillColor2nd({162,162,0})
, m_highlightColor({192,255,192})
, m_bounds()
, m_insideBounds()
, m_solid(false)
, m_smooth(false)
, m_twogears(false)
, m_twogeartop(false)
{
    m_pointCount = m_teeth * 2;
    if(m_teeth == 0)
    {
        m_pointCount = 30;
        m_smooth = true;
    }
    
    update();
}

void Gear::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = NULL;

    for(auto part : m_vertices )
        target.draw(part.second, states);
    if(m_twogeartop)
        target.draw(m_vertices.at("gear2nd"), states);
}

void Gear::update()
{
    std::size_t count = m_pointCount * 2;
    sf::Vector2f innersize = sf::Vector2f(m_size.x - (m_mesh * 2), m_size.y - (m_mesh * 2));
    updateCircle("pin", sf::Vector2f(2.5f,2.5f), 30);
    if(m_smooth)
    {
        innersize = sf::Vector2f(m_size.x - m_mesh, m_size.y - m_mesh);
        updateRing("gear", innersize, m_size);
    }
    else
        updateGear("gear", innersize, m_size, m_mesh, count);
    
    if(m_solid)
    {
        updateCircle("center", innersize, m_pointCount);
    }
    else
    {
        if(m_style == 0)
        {
            updateCircle("vertical", sf::Vector2f(m_size.x * 0.25f, m_size.y - m_mesh), 4);
            updateCircle("horizontal", sf::Vector2f(m_size.x - m_mesh, m_size.y * 0.25f), 4);
        }
        else if(m_style == 1)
        {
            updateCircle("center", sf::Vector2f(m_mesh * 1.5f,m_mesh * 1.5f), 30);
            updateRectangle("vertical", sf::Vector2f(m_mesh, m_size.y * 2 - (m_mesh * 2)));
            updateRectangle("horizontal", sf::Vector2f(m_size.x * 2 - (m_mesh * 2), m_mesh));
        } 
        else if(m_style == 2)
        {
            updateCircle("center", sf::Vector2f(m_mesh * 1.5f,m_mesh * 1.5f), 30);
            updateRectangle("vertical", sf::Vector2f(m_mesh, m_size.y * 2 - (m_mesh * 2)));
            updateRectangle("horizontal", sf::Vector2f(m_size.x * 2 - (m_mesh * 2), m_mesh));
            updateRing("ring", sf::Vector2f(m_size.x * 0.5f - (m_mesh / 2), m_size.y * 0.5f - (m_mesh / 2)), sf::Vector2f(m_size.x * 0.5f + (m_mesh / 2), m_size.y * 0.5f + (m_mesh / 2)));
        }
        else if(m_style == 3)
        {
            updateCircle("center", sf::Vector2f(m_mesh * 1.5f,m_mesh * 1.5f), 30);
            updateRectangle("vertical", sf::Vector2f(m_mesh, m_size.y * 2 - (m_mesh * 2)));
            updateRectangle("horizontal", sf::Vector2f(m_size.x * 2 - (m_mesh * 2), m_mesh));
            updateRing("ring", sf::Vector2f(m_size.x * 0.66f - (m_mesh / 2), m_size.y * 0.66f - (m_mesh / 2)), sf::Vector2f(m_size.x * 0.66f + (m_mesh / 2), m_size.y * 0.66f + (m_mesh / 2)));
            updateRing("ring1", sf::Vector2f(m_size.x * 0.33f - (m_mesh / 2), m_size.y * 0.33f - (m_mesh / 2)), sf::Vector2f(m_size.x * 0.33f + (m_mesh / 2), m_size.y * 0.33f + (m_mesh / 2)));
        }
    }
    updateColors();
}

void Gear::updateCircle(const std::string part, const sf::Vector2f size, const std::size_t count)
{
    sf::Vector2f offset = m_size - size;
    sf::VertexArray temp(sf::TriangleFan);
    temp.resize(count+2);
    for(std::size_t i = 0; i < count; ++i)
        temp[i+1].position = getPoint(i,count,size);
    temp[count+1].position = temp[1].position;
    temp[0] = temp[1];
    sf::FloatRect bounds = temp.getBounds();
    temp[0].position.x = bounds.left + bounds.width / 2;
    temp[0].position.y = bounds.top + bounds.height / 2;

    if(part == "pin")
    {
        m_center = temp[0].position;
        setOrigin(m_center);
    }

    m_vertices[part] = temp;
    temp.clear();
    translatePart(part, temp[0].position);
}

void Gear::updateColors()
{
    for(auto &part : m_vertices)
    {
        if(part.first == "pin")
        {
            for(std::size_t i = 0; i < part.second.getVertexCount(); ++i)
            {
                if(i == 0)
                    part.second[i].color = sf::Color(255,255,255);
                else
                    part.second[i].color = sf::Color(200,200,200);
            }
        }
        else
        {
            for(std::size_t i = 0; i < part.second.getVertexCount(); ++i)
            {
                if(!m_solid && (i % (part.second.getVertexCount()/m_highlights) == 0) && 
                    (part.first != "vertical" && part.first != "horizontal"))
                    part.second[i].color = m_highlightColor;
                else
                    part.second[i].color = m_fillColor;
            }
            
        }
    }
}

void Gear::updateGear(const std::string part, const sf::Vector2f innersize, const sf::Vector2f outersize, const float mesh, const std::size_t count)
{
    sf::Vector2f depth = sf::Vector2f(outersize.x - mesh, outersize.y - mesh);
    sf::Vector2f inneroffset = outersize - innersize;
    sf::Vector2f offset = outersize - depth;
    sf::VertexArray temp(sf::TriangleStrip);
    temp.resize(count+2);
    bool once = false;

    for(std::size_t i = 0; i < count; ++i)
    {
        if((i % 2) == 0)
        {
            if(!m_smooth && once)
                temp[i].position = getPoint(i,count,depth,offset);
            else
                temp[i].position = getPoint(i,count,outersize);
            once = !once ? true : false;
        }
        else
            temp[i].position = getPoint(i,count,innersize,inneroffset);
    }
    temp[count].position = temp[0].position;
    temp[count+1].position = temp[1].position;
    m_insideBounds = temp.getBounds();
    sf::Vector2f position;
    position.x = m_insideBounds.left + m_insideBounds.width / 2;
    position.y = m_insideBounds.top + m_insideBounds.height / 2;

    m_vertices[part] = temp;
    temp.clear();
    translatePart(part, position);
}

void Gear::updateRectangle(const std::string part, const sf::Vector2f size)
{
    std::size_t count = 4;
    sf::Vector2f offset = m_size - size;
    sf::VertexArray temp(sf::TriangleFan);
    temp.resize(count+2);
    for(std::size_t i = 0; i < count; ++i)
        temp[i+1].position = getRectanglePoint(i,size,offset);
    temp[count+1].position = temp[1].position;
    temp[0] = temp[1];
    m_insideBounds = temp.getBounds();
    temp[0].position.x = m_insideBounds.left + m_insideBounds.width / 2;
    temp[0].position.y = m_insideBounds.top + m_insideBounds.height / 2;

    m_vertices[part] = temp;
    temp.clear();
    translatePart(part, temp[0].position);
}

void Gear::updateRing(const std::string part, const sf::Vector2f innersize, const sf::Vector2f outersize)
{
    std::size_t count = m_pointCount*2;
    sf::Vector2f offset = outersize - innersize;
    sf::VertexArray temp(sf::TriangleStrip);
    temp.resize(count+2);
    bool once = false;

    for(std::size_t i = 0; i < count; ++i)
    {
        if((i % 2) == 0)
            temp[i].position = getPoint(i,count,outersize);
        else
            temp[i].position = getPoint(i,count,innersize,offset);
    }
    temp[count].position = temp[0].position;
    temp[count+1].position = temp[1].position;
    m_insideBounds = temp.getBounds();
    sf::Vector2f position;
    position.x = m_insideBounds.left + m_insideBounds.width / 2;
    position.y = m_insideBounds.top + m_insideBounds.height / 2;

    m_vertices[part] = temp;
    temp.clear();
    translatePart(part, position);
}

void Gear::updateSecondGear()
{
    std::size_t count = m_pointCount2nd * 2;
    sf::Vector2f innersize = sf::Vector2f(m_size2nd.x - (m_mesh2nd * 2), m_size2nd.y - (m_mesh2nd * 2));
    updateGear("gear2nd", innersize, m_size2nd, m_mesh2nd, count);
    updateSecondGearColor();
}

void Gear::updateSecondGearColor()
{
    for(std::size_t i = 0; i < m_vertices.at("gear2nd").getVertexCount(); ++i)
        m_vertices.at("gear2nd")[i].color = m_fillColor2nd;

}

void Gear::translatePart(const std::string part, sf::Vector2f pos)
{
    sf::Transform translate;
    translate.translate(m_center - pos);
    sf::Transform trfm = translate;
    for(std::size_t i = 0; i < m_vertices.at(part).getVertexCount(); ++i)
        m_vertices.at(part)[i].position = trfm.transformPoint(m_vertices.at(part)[i].position);
}

} // end namespace BLAK
