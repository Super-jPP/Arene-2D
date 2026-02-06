#include "render/RepeatingMap.h"

namespace render
{
    bool RepeatingMap::loadFromFile(const char* path, int repeatX, int repeatY)
    {
        if (!m_texture.loadFromFile(path))
            return false;

        m_texture.setSmooth(false);

        m_repeatX = repeatX;
        m_repeatY = repeatY;

        const auto texSize = m_texture.getSize();
        m_worldSize = {
            static_cast<float>(texSize.x * repeatX),
            static_cast<float>(texSize.y * repeatY)
        };

        rebuildGeometry();
        return true;
    }

    void RepeatingMap::rebuildGeometry()
    {
        m_vertices.clear();
        m_vertices.resize(m_repeatX * m_repeatY * 6);

        const auto ts = m_texture.getSize();

        std::size_t index = 0;

        for (int y = 0; y < m_repeatY; ++y)
        {
            for (int x = 0; x < m_repeatX; ++x)
            {
                const float px = static_cast<float>(x * ts.x);
                const float py = static_cast<float>(y * ts.y);

                sf::Vertex* quad = &m_vertices[index];

                quad[0].position = { px, py };
                quad[1].position = { px + ts.x, py };
                quad[2].position = { px + ts.x, py + ts.y };

                quad[3].position = { px, py };
                quad[4].position = { px + ts.x, py + ts.y };
                quad[5].position = { px, py + ts.y };

                quad[0].texCoords = { 0.f, 0.f };
                quad[1].texCoords = { static_cast<float>(ts.x), 0.f };
                quad[2].texCoords = { static_cast<float>(ts.x), static_cast<float>(ts.y) };

                quad[3].texCoords = { 0.f, 0.f };
                quad[4].texCoords = { static_cast<float>(ts.x), static_cast<float>(ts.y) };
                quad[5].texCoords = { 0.f, static_cast<float>(ts.y) };

                index += 6;
            }
        }
    }

    void RepeatingMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();
        states.texture = &m_texture;
        target.draw(m_vertices, states);
    }
}
