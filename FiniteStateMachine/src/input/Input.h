#pragma once
#include "math/Vec2.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include "gameplay/Weapons.h"

class Input
{
public:
    Vec2 movementDirection() const;
    bool isAttacking() const;

    Vec2 getMousePosition(const sf::RenderWindow& window) const;

    std::optional<WeaponType> getSelectedWeapon() const;
};