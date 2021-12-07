#pragma once
#include "system.h"
#include "ParticleMove.h"
#include "Transform.h"
#include "EntityCoordinator.h"

class ParticleMoveSystem : public System
{
    void update() override;
};
