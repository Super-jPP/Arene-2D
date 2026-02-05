#include "input/Input.h"
#include <SFML/Window/Keyboard.hpp>
#include <map>

Vec2 Input::movementDirection() const
{
    Vec2 dir(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z))
        dir.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        dir.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
        dir.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        dir.x += 1.f;

    if (!dir.isZero())
        dir.normalize();

    return dir;
}

bool Input::isAttacking() const 
{
    return sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
}

Vec2 Input::getMousePosition(const sf::RenderWindow& window) const 
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    return Vec2(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
}