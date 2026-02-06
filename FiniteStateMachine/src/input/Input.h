#pragma once
#include "math/Vec2.h"
#include <SFML/Graphics/RenderWindow.hpp>

class Input
{
public:
    Vec2 movementDirection() const;
    bool isAttacking() const;
    Vec2 getMousePosition(const sf::RenderWindow& window) const;
};