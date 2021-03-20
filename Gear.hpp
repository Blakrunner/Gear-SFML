/*****************************************************************************************/
/*  Gear using 
/*  SFML - Simple and Fast Multimedia Library
/*  Copyright (C) 2007-2020 Laurent Gomila (laurent@sfml-dev.org)
/*
/*  This software is provided 'as-is', without any express or implied warranty.
/*  In no event will the authors be held liable for any damages arising from the use of this software.
/*
/*  Permission is granted to anyone to use this software for any purpose,
/*  including commercial applications, and to alter it and redistribute it freely,
/*  subject to the following restrictions:
/*
/*  1. The origin of this software must not be misrepresented;
/*     you must not claim that you wrote the original software.
/*     If you use this software in a product, an acknowledgment
/*     in the product documentation would be appreciated but is not required.
/*
/*  2. Altered source versions must be plainly marked as such,
/*     and must not be misrepresented as being the original software.
/* 
/*  3. This notice may not be removed or altered from any source distribution.
/*
/*  created by Blakrunner (blakrunner66@gmail.com)
/*****************************************************************************************/
#ifndef __BLAK_GEAR_HPP__
#define __BLAK_GEAR_HPP__

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <cstring>

namespace BLAK
{

class Gear : public sf::Drawable , public sf::Transformable 
{
public:

    Gear(const sf::Vector2f size = {50.f,50.f}, const float mesh = 5.f, const std::size_t teeth = 60);
    virtual ~Gear();

    void setFillColor(const sf::Color& color);
    void setFillColor(const int r, const int g, const int b, const int a = 255);
    void setFillColor2nd(const sf::Color& color);
    void setFillColor2nd(const int r, const int g, const int b, const int a = 255);
    void setHighlightColor(const sf::Color& color);
    void setHighlightColor(const int r, const int g, const int b, const int a = 255);
    void setNumberTeeth(const std::size_t& teeth);
    void setSize(const sf::Vector2f& size);
    void setSize(const float x, const float y);
    void setSmooth(const bool& smooth);
    void setSolid(const bool& solid);
    void setTwoGears(const sf::Vector2f& size, const float mesh, const std::size_t teeth, const bool ontop = false, const sf::Color color = {175,175,0});
    void setTwoGears(const float x, const float y, const float mesh, const std::size_t teeth, const bool ontop = false, const sf::Color color = {175,175,0});
    void setStyle(const int& style);

protected:

    void update();
    void updateCircle(const std::string part, const sf::Vector2f size, const std::size_t count);
    void updateColors();
    void updateGear(const std::string part, const sf::Vector2f innersize, const sf::Vector2f outersize, const float mesh, const std::size_t count);
    void updateRectangle(const std::string part, const sf::Vector2f size);
    void updateRing(const std::string part, const sf::Vector2f innersize, const sf::Vector2f outersize);
    void updateSecondGear();
    void updateSecondGearColor();
    void translatePart(const std::string part, sf::Vector2f pos);

protected:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:

    bool m_solid;
    bool m_smooth;
    bool m_twogears;
    bool m_twogeartop;

    int m_highlights;
    int m_style;

    float m_mesh;
    float m_mesh2nd;

    std::size_t m_teeth;
    std::size_t m_teeth2nd;
    std::size_t m_pointCount;
    std::size_t m_pointCount2nd;

    sf::Color m_fillColor;
    sf::Color m_fillColor2nd;
    sf::Color m_highlightColor;

    sf::FloatRect m_bounds;
    sf::FloatRect m_insideBounds;

    sf::Vector2f m_center;
    sf::Vector2f m_size;
    sf::Vector2f m_size2nd;

    std::unordered_map< std::string , sf::VertexArray > m_vertices;
};

} // end namespace BLAK

#endif // __BLAK_GEAR_HPP__