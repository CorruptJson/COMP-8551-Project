#include "GameEntityCreator.h"

GameEntityCreator::GameEntityCreator()
{
    EntityCoordinator& ec = EntityCoordinator::getInstance();
    actorArchetype = ec.GetArchetype({
        ec.GetComponentType<Transform>(),
        ec.GetComponentType<RenderComponent>(),
        ec.GetComponentType<PhysicsComponent>(),
        ec.GetComponentType<AnimationComponent>()
        });

    platformArchetype = ec.GetArchetype({
        ec.GetComponentType<Transform>(),
        ec.GetComponentType<RenderComponent>(),
        ec.GetComponentType<PhysicsComponent>(),
        ec.GetComponentType<AnimationComponent>()
        });
}

RenderComponent GameEntityCreator::standardRenderComponent(const char* spriteName, bool hasAnimation)
{
    RenderComponent rc = {
    "defaultVertShader.vs",
    "defaultFragShader.fs",
    spriteName,
    0,
    0,
    hasAnimation
    };
    return rc;
}

GameEntityCreator& GameEntityCreator::getInstance()
{
    static GameEntityCreator gec;
    return gec;
}

EntityID GameEntityCreator::CreateActor(float xPos, float yPos, float scaleX, float scaleY, const char* spriteName, bool hasAnimation)
{
    EntityCoordinator& ec = EntityCoordinator::getInstance();
    EntityID ent = ec.CreateEntity(actorArchetype, spriteName);

    ec.GetComponent<RenderComponent>(ent) = standardRenderComponent(spriteName,hasAnimation);
    ec.GetComponent<Transform>(ent) = Transform(xPos, yPos, 0, scaleX, scaleY);
    ec.GetComponent<PhysicsComponent>(ent) = {
        b2_dynamicBody,
        0.5f * scaleY,
        0.5f * scaleX,
        xPos,
        yPos,
        1.0f,
        0.0f
    };

    return ent;
}

EntityID GameEntityCreator::CreatePlatform(float xPos, float yPos, float scaleX, float scaleY, const char* spriteName)
{
    EntityCoordinator& ec = EntityCoordinator::getInstance();
    EntityID ent = ec.CreateEntity(actorArchetype, spriteName);

    ec.GetComponent<RenderComponent>(ent) = standardRenderComponent(spriteName, false);
    ec.GetComponent<Transform>(ent) = Transform(xPos, yPos, 0, scaleX, scaleY);
    ec.GetComponent<PhysicsComponent>(ent) = {
        b2_staticBody,
        0.5f * scaleY,
        0.5f * scaleX,
        xPos,
        yPos,
        1.0f,
        0.0f
    };

    return ent;
}
