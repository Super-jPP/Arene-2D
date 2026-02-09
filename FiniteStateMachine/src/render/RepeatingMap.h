#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/VertexArray.hpp>

namespace render
{
    // Draws a large map by repeating a single texture tile
    class RepeatingMap final : public sf::Drawable, public sf::Transformable
    {
    public:
        bool loadFromFile(const char* path, int repeatX, int repeatY);

        // World size in pixels
        sf::Vector2f worldSize() const { return m_worldSize; }

    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        void rebuildGeometry();

    private:
        sf::Texture     m_texture;
        sf::VertexArray m_vertices{ sf::PrimitiveType::Triangles };

        int m_repeatX = 1;
        int m_repeatY = 1;

        sf::Vector2f m_worldSize{ 0.f, 0.f };
    };
}
