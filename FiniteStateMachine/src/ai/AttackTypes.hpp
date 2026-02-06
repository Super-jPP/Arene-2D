#pragma once

// Types partagés entre l'Owner et les states IA.
// (Aucun lien avec SFML / rendu : uniquement de la logique.)

enum class AttackType { Light, Heavy, Dash };
enum class AttackPhase { Windup, Active, Recovery };
