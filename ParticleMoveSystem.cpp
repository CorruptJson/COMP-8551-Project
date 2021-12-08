#include "ParticleMoveSystem.h"

// this system moves entities that have the "ParticleMove" component each frame
//  ParticleMove component has a velocity as well as a constant force to apply to their velocity

void ParticleMoveSystem::update()
{
    EntityCoordinator& coordinator = EntityCoordinator::getInstance();
    std::shared_ptr<EntityQuery> query = coordinator.GetEntityQuery({
        coordinator.GetComponentType<Transform>(),
        coordinator.GetComponentType<ParticleMove>()
        }, {});
    ComponentIterator<Transform> t_iterator = ComponentIterator<Transform>(query);
    ComponentIterator<ParticleMove> p_iterator = ComponentIterator<ParticleMove>(query);

    int entCount = query->totalEntitiesFound();
    for (int i = 0; i < entCount; i++)
    {
        Transform* t = t_iterator.nextComponent();
        ParticleMove* p = p_iterator.nextComponent();
        p->velocity.add(p->change);
        Position pos = t->getPosition();
        pos.x += p->velocity.x;
        pos.y += p->velocity.y;
        t->setPosition(pos.x,pos.y);
    }
}
