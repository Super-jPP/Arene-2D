#pragma once

// Stats used by AI states (no gameplay/render dependency).
struct AIStats
{
    float vitesse      = 120.f;
    float detectRadius = 220.f;
    float lostRadius   = 300.f;
    float attackRange  = 60.f;
};
