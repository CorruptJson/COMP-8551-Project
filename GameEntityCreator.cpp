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
    
    panelArchetype = ec.GetArchetype({
        ec.GetComponentType<UIComponent>(),
        ec.GetComponentType<Transform>()
        
        });
}

RenderComponent GameEntityCreator::standardRenderComponent(const char* spriteName, bool hasAnimation)
{
    RenderComponent rc = {
    ShaderName::DEFAULT,
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

//EntityID GameEntityCreator::CreateText(std::string text, float x, float y, float r, float g, float b, float size, std::vector<Tag> tags)
//{
//    EntityCoordinator& ec = EntityCoordinator::getInstance();
//    EntityID ent = ec.CreateEntity(textArchetype, "Text", tags);
//
    //ec.GetComponent<TextComponent>(ent).value = "Test";
    //ec.GetComponent<TextComponent>(ent).x = x;
    //ec.GetComponent<TextComponent>(ent).y = y;
    ////ec.GetComponent<TextComponent>(ent).size = 1.0f;
    //ec.GetComponent<TextComponent>(ent).size = size;
    //ec.GetComponent<TextComponent>(ent).setColor(r, g, b);
//    return ent;
//}

EntityID GameEntityCreator::CreatePanel(float x, float y, float r, float g, float b, float a, float scaleX, float scaleY, std::vector<Tag> tags)
{
    EntityCoordinator& ec = EntityCoordinator::getInstance();
    EntityID ent = ec.CreateEntity(panelArchetype, "UI", tags);

    ec.GetComponent<Transform>(ent) = Transform(x, y, 0, scaleX, scaleY);
    ec.GetComponent<UIComponent>(ent) = {
        "",
        0,0,
        glm::vec4(r, g, b, a)
    };

    return ent;
}

EntityID GameEntityCreator::CreatePanel(float x, float y, float r, float g, float b, float a, float scaleX, float scaleY, const char* spriteName, int row, int col, std::vector<Tag> tags)
{
    EntityCoordinator& ec = EntityCoordinator::getInstance();
    EntityID ent = ec.CreateEntity(panelArchetype, "UI", tags); 

    ec.GetComponent<Transform>(ent) = Transform(x, y, 0, scaleX, scaleY);
    ec.GetComponent<UIComponent>(ent) = {
        spriteName,
        row,col,
        glm::vec4( r, g, b, a)
    };
    
    return ent;
}
