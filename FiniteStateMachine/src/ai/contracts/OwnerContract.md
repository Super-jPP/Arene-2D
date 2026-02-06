# Contrat Owner pour les states IA (Dev A → Dev B)

Les states IA sont des classes templates (`EnemyWander`, `EnemyChase`, `EnemyAttack`, `EnemyDead`, `EnnemyIdle`).
Ils doivent rester **génériques** :

- aucune dépendance vers SFML / rendu / ECS / gameplay
- **aucun accès direct** à une structure interne (ex: `memoireIa`)
- transitions via `EnemyStateId` (évite les includes circulaires)

## Types attendus

- `Vec2` (dans `src/math/Vec2.hpp`)
- `AttackType`, `AttackPhase` (dans `src/ai/AttackTypes.hpp`)
- `EnemyStateId` (dans `src/ai/EnemyStateId.hpp`)
- `Stats` (ou équivalent) contenant au minimum : `vitesse`, `detectRadius`, `lostRadius`, `attackRange`

## API minimale que l'Owner doit exposer

### Sensing / stats
- `Vec2 getPos() const`
- `Vec2 getPlayerPos() const`
- `float getHP() const` *(si vous utilisez Dead)*
- `const Stats& getStats() const`

### Mouvement
- `void moveDir(const Vec2& dir, float dt)`
- `void moveToward(const Vec2& target, float dt)`

### FSM bridge (StateId)
- `void changeState(EnemyStateId id)`

> L'Owner fait le mapping `id → State::instance()` et appelle la FSM.

### Debug / animation (signaux)
- `void setDebugStateName(const std::string& name)`
- `void setAnimState(const std::string& anim)`

### Mémoire Wander
- `Vec2  getWanderDir() const`
- `void  setWanderDir(const Vec2& dir)`
- `float getWanderTimer() const`
- `void  setWanderTimer(float t)`
- `void  decWanderTimer(float dt)`

### Mémoire Attack
- `bool attackReady() const`
- `AttackType getAttackType() const`
- `void setNextAttackType(AttackType t)`

- `AttackPhase getAttackPhase() const`
- `void setAttackPhase(AttackPhase p)`
- `float getAttackPhaseTimer() const`
- `void setAttackPhaseTimer(float t)`
- `void decAttackPhaseTimer(float dt)`

- `bool isHitDone() const`
- `void setHitDone(bool v)`

- `void triggerAttackHit(float cooldownSeconds)`

## Convention d'animations (proposition)

Les states envoient des strings simples ; le rendu mappe ces strings vers des clips/frames :

- Wander : `Walk`
- Chase  : `Run`
- Dead   : `Dead`
- Idle   : `Idle`
- Attack : `AttackLight_Windup`, `AttackLight_Active`, `AttackLight_Recovery` (et Heavy/Dash)
