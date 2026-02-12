#include "Enemy.h"

#include <cstdint>

namespace
{
    constexpr const char* kBatSheet = "assets/enemies/Bat.png";

    // Skeleton sheets for the Wanderer (Green)
    constexpr const char* kSkelIdleSheet   = "assets/enemies/Skeleton_Idle.png";
    constexpr const char* kSkelWanderSheet = "assets/enemies/Skeleton_Wander.png";
    constexpr const char* kSkelAttackSheet = "assets/enemies/Skeleton_Attack.png";

    // IMPORTANT: keep the bat texture in static storage.
    // Enemies are stored in std::vector and may be moved when the vector grows.
    // Our Animator stores a pointer to the texture; if the texture is a member,
    // that pointer can become invalid after moves. Using a static texture keeps
    // the address stable and prevents SFML's "texture destroyed" assert.
    static sf::Texture s_batTexture;
    static bool s_batLoaded = false;

    // Same reasoning for the Wanderer (Green): Enemy instances can be moved inside std::vector,
    // so we keep textures in static storage to keep pointers stable.
    static sf::Texture s_skelIdleTexture;
    static sf::Texture s_skelWanderTexture;
    static sf::Texture s_skelAttackTexture;
    static bool s_skelLoaded = false;

    // Bat.png is a 1-column (vertical) strip.
    // In this project: Run = 4 frames, Atk = 5 frames, Death = 6 frames.
    static render::AnimationClip makeVerticalClip(sf::Texture& tex,
                                                  int startIndex,
                                                  int frameCount,
                                                  int frameW,
                                                  int frameH,
                                                  float fps,
                                                  bool loop)
    {
        render::AnimationClip clip;
        clip.texture = &tex;
        clip.fps = fps;
        clip.loop = loop;
        clip.frames.reserve(frameCount);

        for (int i = 0; i < frameCount; ++i)
        {
            const int y = (startIndex + i) * frameH;
            clip.frames.emplace_back(sf::IntRect({ 0, y }, { frameW, frameH }));
        }
        return clip;
    }

    // 1-row horizontal strip helper (frames laid out left->right)
    static render::AnimationClip makeHorizontalClip(sf::Texture& tex,
                                                    int startIndex,
                                                    int frameCount,
                                                    int frameW,
                                                    int frameH,
                                                    float fps,
                                                    bool loop)
    {
        render::AnimationClip clip;
        clip.texture = &tex;
        clip.fps = fps;
        clip.loop = loop;
        clip.frames.reserve(frameCount);

        for (int i = 0; i < frameCount; ++i)
        {
            const int x = (startIndex + i) * frameW;
            clip.frames.emplace_back(sf::IntRect({ x, 0 }, { frameW, frameH }));
        }
        return clip;
    }

    // (only one makeHorizontalClip helper is needed; keep definition above)
}

static Vec2 normalizeSafe(const Vec2& v)
{
    const float len = std::sqrt(v.x * v.x + v.y * v.y);
    if (len <= 0.00001f) return Vec2{0.f, 0.f};
    return Vec2{ v.x / len, v.y / len };
}

Enemy::Enemy()
: Enemy(Vec2{200.f, 200.f}, 120.f, 450.f, 60.f, 0.8f, EnemyKind::Blue)
{
}

Enemy::Enemy(const Vec2& pos, float speed, float detectRadius, float attackRange, float attackCooldown, EnemyKind kind)
{
    m_pos = pos;
    m_kind = kind;

    if (m_kind == EnemyKind::Green)
    {
        m_hp = 20; // Le Vert est un "Tank", difficile à tuer
    }
    else if (m_kind == EnemyKind::Bat)
    {
        m_hp = 6; // Bat: un peu plus fragile que le Green
    }
    else
    {
        m_hp = 3;  // Le Bleu est fragile (standard)
    }

    // Init per-enemy RNG with a simple hash of position + kind.
    // (Deterministic but "random enough" for gameplay.)
    const std::uint32_t px = static_cast<std::uint32_t>(std::fabs(pos.x) * 1000.f);
    const std::uint32_t py = static_cast<std::uint32_t>(std::fabs(pos.y) * 1000.f);
    const std::uint32_t k  = (m_kind == EnemyKind::Green) ? 0x9E3779B9u : (m_kind == EnemyKind::Bat ? 0xBADC0FFEu : 0x7F4A7C15u);
    m_rngState = (px * 2654435761u) ^ (py * 2246822519u) ^ k;
    if (m_rngState == 0u) m_rngState = 0xA341316Cu;

    m_stats.vitesse = speed;
    m_stats.detectRadius = detectRadius;
    m_stats.lostRadius = detectRadius * 1.4f; // simple default
    m_stats.attackRange = attackRange;

    m_attackCooldownTimer = attackCooldown;

    m_shape.setRadius(14.f);
    m_shape.setOrigin({14.f, 14.f});
    m_shape.setPosition({ m_pos.x, m_pos.y });

    // Visual tweak per enemy kind
    if (m_kind == EnemyKind::Green)
    {
        m_shape.setOutlineThickness(2.f);
        m_shape.setOutlineColor(sf::Color(20, 120, 20));
    }

    // --- Sprite/animation setup for the Bat chaser (EnemyKind::Bat) ---
    if (m_kind == EnemyKind::Bat)
    {
        if (!s_batLoaded)
        {
            s_batLoaded = s_batTexture.loadFromFile(kBatSheet);
        }

        m_useSprite = s_batLoaded;
        if (m_useSprite)
        {
            constexpr int fw = 16;
            constexpr int fh = 16;

            // Per your request: bats always play ONLY their run animation (first 4 vertical frames), looped.
            m_anim.addClip("Run", makeVerticalClip(s_batTexture, 0, 4, fw, fh, 12.f, true));

            m_anim.play("Run", true);      // démarre une seule fois
            m_batRunStarted = true;

            // force l’application du premier frame (évite le “full spritesheet” si rect pas encore set)
            m_anim.update(0.f);

            m_anim.setOrigin(fw * 0.5f, fh * 0.9f);
            m_anim.setScale(4.f, 4.f);
            m_anim.setPosition(m_pos.x, m_pos.y);

        }
    }

    // --- Sprite/animation setup for the Wanderer (Green) using the Skeleton sheets ---
    if (m_kind == EnemyKind::Green)
    {
        if (!s_skelLoaded)
        {
            const bool a = s_skelIdleTexture.loadFromFile(kSkelIdleSheet);
            const bool b = s_skelWanderTexture.loadFromFile(kSkelWanderSheet);
            const bool c = s_skelAttackTexture.loadFromFile(kSkelAttackSheet);
            s_skelLoaded = (a && b && c);
        }

        if (s_skelLoaded)
        {
            // Sheets are 1-row horizontal strips: 4 frames (Idle/Wander) and 8 frames (Attack).
            constexpr int fw = 150;
            constexpr int fh = 150;

            m_useSprite = true;

            m_anim.addClip("Idle",   makeHorizontalClip(s_skelIdleTexture,   0, 4, fw, fh, 10.f, true));
            m_anim.addClip("Wander", makeHorizontalClip(s_skelWanderTexture, 0, 4, fw, fh, 12.f, true));
            m_anim.addClip("Attack", makeHorizontalClip(s_skelAttackTexture, 0, 8, fw, fh, 14.f, false));

            // No dedicated death sheet provided: freeze on the last attack frame as a simple "death" pose.
            m_anim.addClip("Death",  makeHorizontalClip(s_skelAttackTexture, 7, 1, fw, fh, 1.f, false));

            m_anim.play("Idle", true);
            m_anim.setOrigin(fw * 0.5f, fh * 0.5f);  // pivot presque aux pieds
            m_anim.setScale(3.0f, 3.0f);
            m_anim.setPosition(m_pos.x, m_pos.y);

            // Keep the circle as collision/debug only
            m_shape.setFillColor(sf::Color(0, 0, 0, 0));
        }
    }

    // Initial state depends on the agent "type".
    // - Green uses the full Wanderer behavior (Idle/Wander/Chase/Attack/Dead)
    // - Bat is a pure chaser (Chase/Attack/Dead)
    if (m_kind == EnemyKind::Bat)
        m_fsm.setInitial(EnemyChase<Enemy>::instance(), *this);
    else
        m_fsm.setInitial(EnnemyIdle<Enemy>::instance(), *this);
    applyStateColor();
}

float Enemy::rand01()
{
    // xorshift32
    std::uint32_t x = m_rngState;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    m_rngState = x;

    // 24-bit mantissa-ish normalization -> [0,1)
    return static_cast<float>(x & 0x00FFFFFFu) / 16777216.f;
}

void Enemy::update(float dt, const Vec2& playerPos)
{
    m_playerPos = playerPos;

    if (m_attackCooldownTimer > 0.f)
        m_attackCooldownTimer -= dt;

    m_fsm.update(*this, dt);

    m_shape.setPosition({ m_pos.x, m_pos.y });

    // Visual tweak per enemy kind
    if (m_kind == EnemyKind::Green)
    {
        m_shape.setOutlineThickness(2.f);
        m_shape.setOutlineColor(sf::Color(20, 120, 20));
    }

    // --- Bat animation (EnemyKind::Bat) ---
    // Per your request: bats ALWAYS play their Run loop, no matter what the FSM state is.
    if (m_kind == EnemyKind::Bat && m_useSprite)
    {
        m_anim.setFacingLeft(m_playerPos.x < m_pos.x);

        // sécurité : si un move/realloc a cassé l’état interne, on relance UNE fois
        if (!m_batRunStarted)
        {
            m_anim.play("Run", true);
            m_batRunStarted = true;
            m_anim.update(0.f);
        }

        m_anim.setPosition(m_pos.x, m_pos.y);
        m_anim.update(dt);
    }


    // --- Skeleton animation for the Wanderer (EnemyKind::Green) ---
    if (m_kind == EnemyKind::Green && m_useSprite)
    {
        m_anim.setFacingLeft(m_playerPos.x < m_pos.x);

        const std::string n = m_fsm.getStateName();
        const char* desired = "Idle";

        if (n == "Dead")
            desired = "Death";
        else if (n == "Attack")
            desired = "Attack";
        else if (n == "Wander" || n == "Chase")
            desired = "Wander";
        else
            desired = "Idle";

        // Avoid restarting loops every frame
        if (m_anim.currentName() != desired)
            m_anim.play(desired, true);

        m_anim.setPosition(m_pos.x, m_pos.y);
        m_anim.update(dt);
    }
    applyStateColor();
}

void Enemy::draw(sf::RenderTarget& rt) const
{
    // Draw sprite visuals when enabled (Bat + Green). Otherwise fallback to the debug circle.
    if (m_useSprite)
        rt.draw(m_anim.sprite());
    else
        rt.draw(m_shape);
}

void Enemy::moveDir(Vec2 dir, float dt)
{
    dir = normalizeSafe(dir);
    m_pos.x += dir.x * m_stats.vitesse * dt;
    m_pos.y += dir.y * m_stats.vitesse * dt;
}

void Enemy::moveToward(Vec2 target, float dt)
{
    const Vec2 d = Vec2{ target.x - m_pos.x, target.y - m_pos.y };
    moveDir(d, dt);
}

void Enemy::changeState(EnemyStateId id)
{
    switch (id)
    {
        case EnemyStateId::Wander: m_fsm.changeState(EnemyWander<Enemy>::instance(), *this); break;
        case EnemyStateId::Chase:  m_fsm.changeState(EnemyChase<Enemy>::instance(),  *this); break;
        case EnemyStateId::Attack: m_fsm.changeState(EnemyAttack<Enemy>::instance(), *this); break;
        case EnemyStateId::Dead:   m_fsm.changeState(EnemyDead<Enemy>::instance(),   *this); break;
        case EnemyStateId::Idle:   m_fsm.changeState(EnnemyIdle<Enemy>::instance(), *this); break;
        default: break;
    }
}

void Enemy::takeDamage(int amount) {
    if (isDead()) return;

    m_hp -= amount;

    // Petit effet visuel (clignote rouge)
    m_shape.setFillColor(sf::Color::Red);

    if (m_hp <= 0) {
        // Déclenche l'état mort de la FSM
        m_fsm.changeState(EnemyDead<Enemy>::instance(), *this);
    }
}

int Enemy::extractPendingDamage()
{
    int dmg = m_pendingDamageToPlayer;
    m_pendingDamageToPlayer = 0;
    return dmg;
}

// Update this existing method
void Enemy::triggerAttackHit(float cooldown)
{
    m_attackCooldownTimer = cooldown;

    // Check distance to player to validate the hit
    float dist = Vec2::distance(m_pos, m_playerPos);

    // If player is within attack range + small buffer, apply damage
    if (dist <= m_stats.attackRange * 1.2f)
    {
        m_pendingDamageToPlayer = 10; // Value could be in AIStats
    }
}

void Enemy::applyStateColor()
{
    const std::string n = m_fsm.getStateName();

    // Two enemy "types" so the CDC requirements can be validated without sprites:
    // - Blue: classic palette
    // - Green: alternate palette
    if (m_kind == EnemyKind::Green)
    {
        if (n == "Wander")       m_shape.setFillColor(sf::Color(80, 220, 120));
        else if (n == "Chase")   m_shape.setFillColor(sf::Color(40, 190, 90));
        else if (n == "Attack")  m_shape.setFillColor(sf::Color(20, 150, 70));
        else if (n == "Dead")    m_shape.setFillColor(sf::Color(40, 40, 40));
        else                     m_shape.setFillColor(sf::Color(140, 210, 160));
        return;
    }

    // Default (Blue) palette
    if (n == "Wander")       m_shape.setFillColor(sf::Color(80, 140, 255));
    else if (n == "Chase")   m_shape.setFillColor(sf::Color(255, 160, 60));
    else if (n == "Attack")  m_shape.setFillColor(sf::Color(255, 70, 70));
    else if (n == "Dead")    m_shape.setFillColor(sf::Color(40, 40, 40));
    else                     m_shape.setFillColor(sf::Color(180, 180, 180));
}
