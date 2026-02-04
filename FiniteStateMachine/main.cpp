#include "StateMachine/NPC.h"
#include <SFML/Graphics.hpp>
int main()
{
    //Npc npc;

    //npc.Init();

    //while (true)
    //{
    //    npc.Update();
    //}

    sf::RenderWindow window(sf::VideoMode({ 200, 200 }), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }
}


/*
Template
FSM
fichiers inl

*/