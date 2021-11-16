#include "GameEntityCreator.h"

GameEntityCreator::GameEntityCreator()
{
    EntityCoordinator& ec = EntityCoordinator::getInstance();
    actorArchetype = ec.GetArchetype({
        ec.GetComponentType<Transform>(),
        ec.GetComponentType<RenderComponent>(),
        ec.GetComponentType<PhysicsComponent>(),
        ec.GetComponentType<AnimationComponent>(),
        ec.GetComponentType<MovementComponent>(),
        ec.GetComponentType<StateComponent>()

        });

    platformArchetype = ec.GetArchetype({
        ec.GetComponentType<Transform>(),
        ec.GetComponentType<RenderComponent>(),
        ec.GetComponentType<PhysicsComponent>(),
        ec.GetComponentType<AnimationComponent>(),
        ec.GetComponentType<MovementComponent>(),
        ec.GetComponentType<StateComponent>()

        });

    testArchetype = ec.GetArchetype({
        ec.GetComponentType<TimerComponent>()
        });
    
    textArchetype = ec.GetArchetype({
        ec.GetComponentType<TextComponent>()
        });

    starArchetype = ec.GetArchetype({
        ec.GetComponentType<Transform>()
        });

    //sceneryArchetype = ec.GetArchetype({
    //    ec.GetComponentType<Transform>(),
    //    ec.GetComponentType<RenderComponent>()
    //    });
}

RenderComponent GameEntityCreator::standardRenderComponent(const char* spriteName, bool hasAnimation)
{
    RenderComponent rc = {
    "defaultVertShader.vs",
    "defaultFragShader.fs",
    spriteName,
    0,
    0,
    hasAnimation,
    false
    };
    return rc;
}

GameEntityCreator& GameEntityCreator::getInstance()
{
    static GameEntityCreator gec;
    return gec;
}

EntityID GameEntityCreator::CreateActor(float xPos, float yPos, float scaleX, float scaleY, const char* spriteName, std::vector<Tag> tags, bool hasAnimation, int state)
{
    EntityCoordinator& ec = EntityCoordinator::getInstance();
    EntityID ent = ec.CreateEntity(actorArchetype, spriteName, tags);

    ec.GetComponent<RenderComponent>(ent) = standardRenderComponent(spriteName,hasAnimation);
    ec.GetComponent<Transform>(ent) = Transform(xPos, yPos, 0, scaleX, scaleY);
    ec.GetComponent<PhysicsComponent>(ent) = {
        b2_dynamicBody,
        0.5f * scaleY,
        0.5f * scaleX,
        xPos,
        yPos,
        1.0f,
        0.0f,
        false
    };
    ec.GetComponent<StateComponent>(ent) = {
      state,
      true
    };
    return ent;
}

EntityID GameEntityCreator::CreatePlatform(float xPos, float yPos, float scaleX, float scaleY, const char* spriteName, std::vector<Tag> tags, int state)
{
    EntityCoordinator& ec = EntityCoordinator::getInstance();
    EntityID ent = ec.CreateEntity(actorArchetype, spriteName, tags);

    ec.GetComponent<RenderComponent>(ent) = standardRenderComponent(spriteName, false);
    ec.GetComponent<Transform>(ent) = Transform(xPos, yPos, 0, scaleX, scaleY);
    ec.GetComponent<PhysicsComponent>(ent) = {
        b2_staticBody,
        0.5f * scaleY,
        0.5f * scaleX,
        xPos,
        yPos,
        1.0f,
        0.0f,
        false
    };
    ec.GetComponent<StateComponent>(ent) = {
        state
    };
    return ent;
}

EntityID GameEntityCreator::CreateTimer(const char* spriteName, std::vector<Tag> tags)
{
    EntityCoordinator& ec = EntityCoordinator::getInstance();
    EntityID ent = ec.CreateEntity(testArchetype, spriteName, tags);
    
    ec.GetComponent<TimerComponent>(ent).ticksPassed = 0;
    return ent;
}

//EntityID GameEntityCreator::CreateScenery(float xPos, float yPos, float scaleX, float scaleY, const char* spriteName, std::vector<Tag> tags)
//{
//    EntityCoordinator& ec = EntityCoordinator::getInstance();
//    EntityID ent = ec.CreateEntity(sceneryArchetype, spriteName, tags);
//
//    ec.GetComponent<RenderComponent>(ent) = standardRenderComponent(spriteName, false);
//    ec.GetComponent<Transform>(ent) = Transform(xPos, yPos, 0, scaleX, scaleY);
//
//    return ent;
//}

EntityID GameEntityCreator::CreateText(const char* text, float x, float y, float r, float g, float b, float size, std::vector<Tag> tags)
{
    EntityCoordinator& ec = EntityCoordinator::getInstance();
    EntityID ent = ec.CreateEntity(textArchetype, "Text", tags);

    ec.GetComponent<TextComponent>(ent).value = text;
    ec.GetComponent<TextComponent>(ent).x = x;
    ec.GetComponent<TextComponent>(ent).y = y;
    //ec.GetComponent<TextComponent>(ent).size = 1.0f;
    ec.GetComponent<TextComponent>(ent).size = size;
    ec.GetComponent<TextComponent>(ent).setColor(r, g, b);
    return ent;
}

EntityID GameEntityCreator::CreateStar(float xPos, float yPos, float scaleX, float scaleY, const char* spriteName, std::vector<Tag> tags, bool hasAnimation, int state)
{
    EntityCoordinator& ec = EntityCoordinator::getInstance();
    EntityID ent = ec.CreateEntity(actorArchetype, spriteName, tags);

    ec.GetComponent<RenderComponent>(ent) = standardRenderComponent(spriteName, hasAnimation);
    ec.GetComponent<Transform>(ent) = Transform(xPos, yPos, 0, scaleX, scaleY);
    ec.GetComponent<PhysicsComponent>(ent) = {
        b2_dynamicBody,
        0.5f * scaleY,
        0.5f * scaleX,
        xPos,
        yPos,
        1.0f,
        0.0f,
        false
    };
    ec.GetComponent<StateComponent>(ent) = {
      state,
      true
    };
    return ent;
}
