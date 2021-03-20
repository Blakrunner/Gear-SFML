#include <SFML/Graphics.hpp>
#include "Gear.hpp"

int main()
{
    sf::VideoMode scrn(sf::VideoMode::getDesktopMode());
    sf::RenderWindow window(scrn, "Gear");
    window.setFramerateLimit(60);

    sf::Vector2f cntr(sf::Vector2f(scrn.width/2, scrn.height/2));

    sf::View view;
    view.setSize(sf::Vector2f(scrn.width,scrn.height));
    view.setCenter(cntr);

    sf::Color bkgd(sf::Color(0,0,0));

    BLAK::Gear gear({12.5f,12.5f},4.5,12);
    gear.setSolid(true);

    BLAK::Gear gear1;

    BLAK::Gear gear2({25,25},5,30);
    gear2.setStyle(1);

    BLAK::Gear gear3({37.5,37.5},5,45);
    gear3.setFillColor(200,200,225);
    gear3.setStyle(1);
    gear3.setTwoGears(25,25,5,30);
    gear3.setFillColor2nd(128,128,192);

    BLAK::Gear gear4;
    gear4.setStyle(2);

    BLAK::Gear gear5({62.5,62.5},5,75);
    gear5.setStyle(2);
    gear5.setTwoGears(12.5,12.5,4.5,12,true);

    BLAK::Gear balance({50,50},5,0);
    balance.setStyle(3);

    bool active = true;
    bool pinched = false;
    bool plus = true;

    sf::Vector2i currPos;

    float rotate = 0.f;
    float rotate1 = 0.f;
    float rotate2 = 0.f;
    float rotate3 = 0.f;
    float rotate4 = 0.f;
    float rotate5 = 0.f;
    float rotbal = 0.f;
    float zoom = 1.f;

    while (window.isOpen())
    {
        sf::Event event;
        while (active ? window.pollEvent(event) : window.waitEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::Resized:
                    view.setSize(sf::Vector2f(event.size.width,event.size.height));
                    view.setCenter(sf::Vector2f(event.size.width/2,event.size.height/2));
                    cntr = sf::Vector2f(event.size.width/2,event.size.height/2);
                    view.zoom(zoom);
                    gear.setPosition(cntr);
                    gear1.setPosition(cntr + sf::Vector2f(31.25,49));
                    gear2.setPosition(cntr + sf::Vector2f(62.25, -14.5));
                    gear3.setPosition(cntr + sf::Vector2f(30, -46.5));
                    gear4.setPosition(cntr + sf::Vector2f(-50,-70));
                    gear5.setPosition(cntr + sf::Vector2f(-114,18));
                    balance.setPosition(cntr + sf::Vector2f(150,0));
                    break;
                case sf::Event::TouchBegan:
                    sf::Vector2i touchpos = sf::Touch::getPosition(0);
                    if(touchpos.x > (int)cntr.x)
                    {
                        zoom *= 2.f;
                        view.zoom(2.f);
                    }
                    else
                    {
                        zoom *= 0.5f;
                        view.zoom(0.5f);
                    }
                    break;
            }
        }

        rotate += 5.f;
        gear.setRotation(rotate);
        rotate1 -= 1.f;
        gear1.setRotation(rotate1);
        rotate2 += 2.f;
        gear2.setRotation(rotate2);
        rotate3 -= 2.f;
        gear3.setRotation(rotate3);
        rotate4 += 1.5f;
        gear4.setRotation(rotate4);
        rotate5 -= 1.2f;
        gear5.setRotation(rotate5);

        if(plus)
            rotbal += 10.f;
        else
            rotbal -= 10.f;
        if(rotbal <= 0 || rotbal >= 359)
            plus = !plus ? true : false;
        balance.setRotation(rotbal);

       if(active)
        {
            window.clear(bkgd);
 
            window.setView(view);
            window.draw(gear);
            window.draw(gear1);
            window.draw(gear2);
            window.draw(gear3);
            window.draw(gear4);
            window.draw(gear5);
            window.draw(balance);

            window.display();
        }
        else {
            sf::sleep(sf::milliseconds(100));
        }
    }

    return 0;
}