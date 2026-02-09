#include "render/Animator.h"

#include <algorithm>
#include <cmath>

namespace render
{
    static const sf::Texture& dummyTexture()
    {
        static sf::Texture tex;
        static bool init = false;
        if (!init)
        {
            // SFML 3: sf::Image::create() was removed; use constructor
            sf::Image img({ 1u, 1u }, sf::Color::White);
            tex.loadFromImage(img);
            init = true;
        }
        return tex;
    }

    Animator::Animator()
        : m_sprite(dummyTexture())
    {
        // Keep a sane default scale/origin
        m_sprite.setOrigin({ 0.f, 0.f });
        m_sprite.setScale({ 1.f, 1.f });
    }

    void Animator::addClip(const std::string& name, AnimationClip clip)
    {
        m_clips[name] = std::move(clip);
    }

    void Animator::addRowClip(const std::string& name,
                              const sf::Texture& texture,
                              int frameW,
                              int frameH,
                              int frameCount,
                              float fps,
                              bool loop)
    {
        AnimationClip clip;
        clip.texture = &texture;
        clip.fps = fps;
        clip.loop = loop;
        clip.frames.reserve(static_cast<size_t>(frameCount));

        for (int i = 0; i < frameCount; ++i)
        {
            clip.frames.emplace_back(
                sf::Vector2i{ i * frameW, 0 },
                sf::Vector2i{ frameW, frameH }
            );
        }

        addClip(name, std::move(clip));
    }

    void Animator::play(const std::string& name, bool restart)
    {
        auto it = m_clips.find(name);
        if (it == m_clips.end())
            return;

        if (!restart && m_currentName == name)
            return;

        m_currentName = name;
        m_current = &it->second;
        m_time = 0.f;
        m_frameIndex = 0;

        if (m_current->texture)
            m_sprite.setTexture(*m_current->texture, true);

        applyFrame();
    }

    void Animator::update(float dt)
    {
        if (!m_current || m_current->frames.empty())
            return;

        const float spf = (m_current->fps <= 0.f) ? 1.f : (1.f / m_current->fps);
        m_time += dt;

        while (m_time >= spf)
        {
            m_time -= spf;
            ++m_frameIndex;

            const int frameCount = static_cast<int>(m_current->frames.size());

            if (m_frameIndex >= frameCount)
            {
                if (m_current->loop)
                    m_frameIndex = 0;
                else
                    m_frameIndex = frameCount - 1; // hold last frame
            }

            applyFrame();
        }
    }

    void Animator::setPosition(float x, float y)
    {
        m_sprite.setPosition({ x, y });
    }

    void Animator::setScale(float x, float y)
    {
        // If facing left, we keep x sign negative
        const float sx = m_facingLeft ? -std::abs(x) : std::abs(x);
        m_sprite.setScale({ sx, y });
    }

    void Animator::setOrigin(float x, float y)
    {
        m_sprite.setOrigin({ x, y });
    }

    void Animator::setFacingLeft(bool left)
    {
        if (m_facingLeft == left)
            return;
        m_facingLeft = left;

        // Preserve magnitude of scale when flipping
        const sf::Vector2f s = m_sprite.getScale();
        m_sprite.setScale({ left ? -std::abs(s.x) : std::abs(s.x), s.y });
    }

    bool Animator::isFinished() const
    {
        if (!m_current)
            return true;
        if (m_current->loop)
            return false;
        return m_frameIndex >= static_cast<int>(m_current->frames.size()) - 1;
    }

    void Animator::applyFrame()
    {
        if (!m_current || m_current->frames.empty())
            return;

        const int idx = std::clamp(m_frameIndex, 0, static_cast<int>(m_current->frames.size()) - 1);
        m_sprite.setTextureRect(m_current->frames[idx]);
    }
}
