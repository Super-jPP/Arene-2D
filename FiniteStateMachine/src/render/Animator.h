#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include <unordered_map>
#include <vector>

namespace render
{
    struct AnimationClip
    {
        const sf::Texture* texture = nullptr;
        std::vector<sf::IntRect> frames;
        float fps = 12.f;
        bool loop = true;
    };

    class Animator
    {
    public:
        // SFML 3: sf::Sprite is not default-constructible (needs a texture).
        // We initialize it with a 1x1 dummy texture and replace it when playing a clip.
        Animator();

        void addClip(const std::string& name, AnimationClip clip);

        // Convenience for a simple 1-row sprite sheet (frames laid out horizontally)
        void addRowClip(const std::string& name,
                        const sf::Texture& texture,
                        int frameW,
                        int frameH,
                        int frameCount,
                        float fps,
                        bool loop);

        void play(const std::string& name, bool restart = false);
        void update(float dt);

        void setPosition(float x, float y);
        void setScale(float x, float y);
        void setOrigin(float x, float y);
        void setFacingLeft(bool left);

        const sf::Sprite& sprite() const { return m_sprite; }

        bool isFinished() const;
        const std::string& current() const { return m_currentName; }

    private:
        void applyFrame();

        std::unordered_map<std::string, AnimationClip> m_clips;
        std::string m_currentName;
        AnimationClip* m_current = nullptr;

        sf::Sprite m_sprite;

        float m_time = 0.f;
        int m_frameIndex = 0;
        bool m_facingLeft = false;
    };
}
