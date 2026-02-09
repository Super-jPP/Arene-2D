#include "input/Input.h"
#include <SFML/Window/Keyboard.hpp>

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

std::optional<WeaponType> Input::getSelectedWeapon() const {
    using enum sf::Keyboard::Key;
    if (sf::Keyboard::isKeyPressed(Num1)) return WeaponType::PISTOL;
    if (sf::Keyboard::isKeyPressed(Num2)) return WeaponType::SHOTGUN;
    if (sf::Keyboard::isKeyPressed(Num3)) return WeaponType::MAGIC_WAND;
    return std::nullopt;
}
    const sf::Vector2i pixel = sf::Mouse::getPosition(window);
    const sf::Vector2f world = window.mapPixelToCoords(pixel);
    return Vec2(world.x, world.y);
}
