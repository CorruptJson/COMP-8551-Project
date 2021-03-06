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
        ec.GetComponentType<TextComponent>(),
        ec.GetComponentType<Transform>()
        });

    uiArchetype = ec.GetArchetype({
        ec.GetComponentType<RenderComponent>(),
        ec.GetComponentType<Transform>()
        });

    starArchetype = ec.GetArchetype({
        ec.GetComponentType<Transform>()
        });

    physParticleArchetype = ec.GetArchetype({
        ec.GetComponentType<Transform>(),
        ec.GetComponentType<RenderComponent>(),
        ec.GetComponentType<PhysicsComponent>(),
        ec.GetComponentType<DeleteTimer>(),
        });

    particleArchetype = ec.GetArchetype({
        ec.GetComponentType<Transform>(),
        ec.GetComponentType<RenderComponent>(),
        ec.GetComponentType<DeleteTimer>(),
        ec.GetComponentType<ParticleMove>()
        });

    particleAnimated = ec.GetArchetype({
        ec.GetComponentType<Transform>(),
        ec.GetComponentType<RenderComponent>(),
        ec.GetComponentType<DeleteTimer>(),
        ec.GetComponentType<ParticleMove>(),
        ec.GetComponentType<AnimationComponent>(),
        });

    enemiesInitialStates[ROACH] = StateComponent {
        0,
        true,
        3,
        2
    };

    enemiesInitialStates[SMALL_ROACH] = StateComponent{
        0,
        true,
        5,
        1
    };
}

RenderComponent GameEntityCreator::standardRenderComponent(const char* spriteName, bool flipX)
{
    RenderComponent rc = {
    ShaderName::DEFAULT,
    spriteName,
    0,
    0,
    flipX
    };
    return rc;
}

GameEntityCreator& GameEntityCreator::getInstance()
{
    static GameEntityCreator gec;
    return gec;
}

EntityID GameEntityCreator::CreateActor(float xPos, float yPos, float scaleX, float scaleY, const char* spriteName, std::vector<Tag> tags, bool flipX, int state)
{
    EntityCoordinator& ec = EntityCoordinator::getInstance();
    EntityID ent = ec.CreateEntity(actorArchetype, spriteName, tags);

    ec.GetComponent<RenderComponent>(ent) = {
        ShaderName::DEFAULT,
        spriteName,
        0,
        0,
        flipX
    };
    ec.GetComponent<Transform>(ent) = Transform(xPos, yPos, 0, scaleX, scaleY);
    ec.GetComponent<AnimationComponent>(ent) = {
        Renderer::getInstance()->getAnimation("run", spriteName),
        0.0f, //starts off at zero for currTimeStamp
        0.0f, //starts off at zero for lastTimeStamp
        0
    };
    ec.GetComponent<PhysicsComponent>(ent) = {
        b2_dynamicBody,
        0.5f * scaleX,
        0.5f * scaleY,
        xPos,
        yPos,
        1.0f,
        0.0f,
        false
    };
    ec.GetComponent<StateComponent>(ent).state = state;
    return ent;
}


EntityID GameEntityCreator::CreateRoach(float xPos, float yPos, bool facingRight) {
    EntityCoordinator& ec = EntityCoordinator::getInstance();
    EntityID roach = CreateActor(xPos, yPos, 1, 1, "Giant_Roach.png", { Tag::ENEMY }, false, 0);
    EntityID* id = &roach;
    Notify(Event::B2BODY_ADD, (void*)id);
    //PhysicsWorld::getInstance().AddObject(roach);
    RenderComponent& rendComp = ec.GetComponent<RenderComponent>(roach);
    rendComp.flipX = !facingRight;

    StateComponent& stateComp = enemiesInitialStates[ROACH];
    MovementComponent& moveComp = ec.GetComponent<MovementComponent>(roach);
    float xVel = facingRight ? stateComp.speed : -stateComp.speed;
    moveComp.setVelocity(xVel, 0.0f);

    // copy the state comp to the entity so we can reuse it (mostly for health)
    ec.GetComponent<StateComponent>(roach) = stateComp;
    return roach;
}

EntityID GameEntityCreator::CreateSmallRoach(float xPos, float yPos, bool facingRight) {
    EntityCoordinator& ec = EntityCoordinator::getInstance();
    EntityID roach = CreateActor(xPos, yPos, 0.7, 0.7, "Giant_Roach.png", { Tag::ENEMY }, false, 0);
    EntityID* id = &roach;
    Notify(Event::B2BODY_ADD,(void*)id);
    //PhysicsWorld::getInstance().AddObject(roach);
    RenderComponent& rendComp = ec.GetComponent<RenderComponent>(roach);
    rendComp.flipX = !facingRight;
    rendComp.color = glm::vec3(1.0f, 0, 0); // red tint

    StateComponent& stateComp = enemiesInitialStates[SMALL_ROACH];
    MovementComponent& moveComp = ec.GetComponent<MovementComponent>(roach);
    float xVel = facingRight ? stateComp.speed : -stateComp.speed;
    moveComp.setVelocity(xVel, 0.0f);

    // copy the state comp to the entity so we can reuse it (mostly for health)
    ec.GetComponent<StateComponent>(roach) = stateComp;
    return roach;

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

EntityID GameEntityCreator::CreateText(std::string scoreTxt, float x, float y, float r, float g, float b, float size, std::vector<Tag> tags) {
    return CreateText(scoreTxt, x, y, r, g, b, size, TextAlign::CENTER, tags);
}

EntityID GameEntityCreator::CreateText(std::string scoreTxt, float x, float y, float r, float g, float b, float size, TextAlign align, std::vector<Tag> tags)
{
    EntityCoordinator& ec = EntityCoordinator::getInstance();
    EntityID ent = ec.CreateEntity(textArchetype, "Text", tags);

    ec.GetComponent<TextComponent>(ent) = TextComponent(
        scoreTxt,
        size,
        r,
        g,
        b,
        align
    );

    // no rotation and we will use size to determine the font size, not scales.
    Transform transform = Transform(x, y, 0, 1, 1);
    transform.setInterpolatorX(Renderer::getInstance()->getTextXInterpolator());
    transform.setInterpolatorY(Renderer::getInstance()->getTextYInterpolator());
    ec.GetComponent<Transform>(ent) = transform;
    return ent;
}

EntityID GameEntityCreator::CreateUI(float x, float y, float height, float width, float r, float g, float b, std::vector<Tag> tags) {
    EntityCoordinator& ec = EntityCoordinator::getInstance();

    // ensure we always have an UI tag
    bool hasTag = false;
    for (auto& tag : tags) {
        if (tag == Tag::UI) {
            hasTag = true;
            break;
        }
    }
    if (!hasTag) tags.push_back(Tag::UI);

    EntityID ent = ec.CreateEntity(uiArchetype, tags);

    RenderComponent renderComp = standardRenderComponent("", false);

    // customize the color values
    renderComp.colorOnly = true;
    renderComp.color.r = r;
    renderComp.color.g = g;
    renderComp.color.b = b;
    ec.GetComponent<RenderComponent>(ent) = renderComp;

    ec.GetComponent<Transform>(ent) = Transform(x, y, 0, width, height);
    return ent;
}

EntityID GameEntityCreator::CreateStar(float xPos, float yPos, float scaleX, float scaleY, const char* spriteName, std::vector<Tag> tags)
{
    EntityCoordinator& ec = EntityCoordinator::getInstance();
    EntityID ent = ec.CreateEntity(actorArchetype, spriteName, tags);

    ec.GetComponent<RenderComponent>(ent) = standardRenderComponent(spriteName, false);
    ec.GetComponent<Transform>(ent) = Transform(xPos, yPos, 0, scaleX, scaleY);
    ec.GetComponent<AnimationComponent>(ent) = {
        Renderer::getInstance()->getAnimation("flicker", spriteName),
        0.0f, //starts off at zero for currTimeStamp
        0.0f, //starts off at zero for lastTimeStamp
        0
    };
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
    return ent;
}

EntityID GameEntityCreator::CreatePhysParticle(TransformArg t, int frameLife,const char* spriteName)
{
    EntityCoordinator& ec = EntityCoordinator::getInstance();
    EntityID ent = ec.CreateEntity(physParticleArchetype, spriteName, {});
    GameManager& gm = GameManager::getInstance();
    ec.GetComponent<DeleteTimer>(ent) = { gm.getCurrGameFrame() + frameLife };
    ec.GetComponent<Transform>(ent) = Transform(t.xPos, t.yPos, 0, t.xScale, t.xScale);
    ec.GetComponent<RenderComponent>(ent) = standardRenderComponent(spriteName, false);
    ec.GetComponent<PhysicsComponent>(ent) = {
    b2_dynamicBody,
    0.5f * t.xScale,
    0.5f * t.xScale,
    t.xPos,
    t.yPos,
    1.0f,
    0.0f,
    false
    };
    return ent;
}

EntityID GameEntityCreator::CreateParticle(TransformArg t, int frameLife, const char* spriteName, float col, float row, ParticleMove move)
{
    EntityCoordinator& ec = EntityCoordinator::getInstance();
    EntityID ent = ec.CreateEntity(particleArchetype, spriteName, {});
    GameManager& gm = GameManager::getInstance();
    ec.GetComponent<DeleteTimer>(ent) = { gm.getCurrGameFrame() + frameLife };
    ec.GetComponent<Transform>(ent) = Transform(t.xPos, t.yPos, 0, t.xScale, t.xScale);
    RenderComponent& r = ec.GetComponent<RenderComponent>(ent);
    r = standardRenderComponent(spriteName, false);
    r.rowIndex = row;
    r.colIndex = col;
    ec.GetComponent<ParticleMove>(ent) = move;
    return ent;
}

EntityID GameEntityCreator::CreateSparkle(TransformArg t, int frameLife, int currFrame, ParticleMove move)
{
    const char * spriteName = "sparkles.png";
    EntityCoordinator& ec = EntityCoordinator::getInstance();
    EntityID ent = ec.CreateEntity(particleAnimated, "sparkles.png", {});
    GameManager& gm = GameManager::getInstance();
    ec.GetComponent<DeleteTimer>(ent) = { gm.getCurrGameFrame() + frameLife };
    ec.GetComponent<Transform>(ent) = Transform(t.xPos, t.yPos, 0, t.xScale, t.xScale);
    RenderComponent& r = ec.GetComponent<RenderComponent>(ent);
    r = standardRenderComponent(spriteName, false);
    ec.GetComponent<ParticleMove>(ent) = move;
    ec.GetComponent<AnimationComponent>(ent) = {
        Renderer::getInstance()->getAnimation("sparkle", spriteName),
        0.0f, //starts off at zero for currTimeStamp
        0.0f, //starts off at zero for lastTimeStamp
        currFrame
    };
    return ent;
}

void GameEntityCreator::Receive(Event e, void* args)
{
    if (e == Event::ENEMY_DESTROYED)
    {
        // create the bug skeletons particle entities when an enemy dies

        Transform* t = (Transform*)args;
        Position p = t->getPosition();
        ParticleMove move = { {0,0.05f},{0,-0.0025f} };
        CreateParticle({ p.x,p.y - 0.09f,0.6f,0.6f }, 45, "particles_01.png", 1, 0, move);
        move = { {0,0.08f},{0,-0.004f} };
        CreateParticle({p.x,p.y,0.6f,0.6f},45, "particles_01.png",0,0, move);        
    }
    else if (e == Event::INPUT_SHOOT)
    {
        // create a shell casing particle when the player shoots

        EntityCoordinator& coord = EntityCoordinator::getInstance();
        std::shared_ptr<EntityQuery> eq = coord.GetEntityQuery(
            {
                coord.GetComponentType<Transform>(),
                coord.GetComponentType<StateComponent>(),
            }, { Tag::PLAYER }
        );
        ComponentIterator<Transform> ti = ComponentIterator<Transform>(eq);
        ComponentIterator<StateComponent> si = ComponentIterator<StateComponent>(eq);
        if (eq->totalEntitiesFound() > 0)
        {
            Transform* playerTransform = ti.nextComponent();
            StateComponent* playerState = si.nextComponent();

            float face = (playerState->faceRight) ? -1 : 1;
            ParticleMove move = { {0.02f * face,0.05f},{0,-0.0045f} };
            Position p = playerTransform->getPosition();
            CreateParticle({ p.x + 0.4f * face,p.y + 0.04f,0.5f,0.5f }, 29, "particles_01.png", 0, 1, move);
        }        
    }
    else if (e == Event::STAR_PICKED_UP)
    {
        // create a sparkle effect when the player picks up a star

        Transform* t = (Transform*)args;
        for (int i = 0; i < 5; i++)
        {
            ParticleMove move = { {0,0.017f},{0,0.00025f} };
            Position p = t->getPosition();
            float xMod = 0.14f * i - (0.14f * 2.5f);
            CreateSparkle({ p.x + xMod,p.y - 0.4f,0.7f,0.7f }, 90, i % 4, move);
        }
    }
}
