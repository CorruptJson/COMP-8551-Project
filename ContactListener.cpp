#include "ContactListener.h"

int health = 3;
ContactListener::~ContactListener()
{

}

// IMPORTANT: You cannot create/destroy Box2D entities inside these callbacks.

void ContactListener::BeginContact(b2Contact* contact) {

    PhysicsComponent* physicsComponentA = reinterpret_cast<PhysicsComponent*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
    PhysicsComponent* physicsComponentB = reinterpret_cast<PhysicsComponent*>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);

    Tag tagA = EntityCoordinator::getInstance().getTagsForEntity(physicsComponentA->entityID)[0];
    Tag tagB = EntityCoordinator::getInstance().getTagsForEntity(physicsComponentB->entityID)[0];

    Tag tagFirst, tagSecond;
    EntityID entFirst, entSecond;

    if (tagA < tagB) {
        tagFirst = tagA;
        tagSecond = tagB;
        entFirst = physicsComponentA->entityID;
        entSecond = physicsComponentB->entityID;
    }
    else {
        tagFirst = tagB;
        tagSecond = tagA;
        entFirst = physicsComponentB->entityID;
        entSecond = physicsComponentA->entityID;
    }


    if (tagFirst == PLAYER) {
        cout << "Player contact with: ";
        if (tagSecond == PLATFORM) {
            cout << "Platform" << endl;
        }
        else if (tagSecond == WALL) {
            cout << "Platform" << endl;
        }
        else if (tagSecond == ENEMY) {
            cout << "Enemy" << endl;
            Notify(Event::C_START_PLAYER_ENEMY, nullptr);
            EntityCoordinator* ec = &EntityCoordinator::getInstance();
            std::shared_ptr<EntityQuery> eq = ec->GetEntityQuery({
                ec->GetComponentType<TextComponent>()
                }, {Tag::HEALTH_NUM});
            
            ComponentIterator<TextComponent> tci(eq);

            //Set score text
            health--;
            if (health == 0) health = 3;
            std::string healthTxt = "X ";
            tci.nextComponent()->setText(healthTxt + std::to_string(health));
        }
        else if (tagSecond == STAR) {
            cout << "Star" << endl;
            EntityCoordinator::getInstance().GetComponent<PhysicsComponent>(entSecond).isFlaggedForDelete = true;
            Notify(Event::STAR_PICKED_UP, nullptr);
        }
        else if (tagSecond == FIRE) {
            cout << "Fire" << endl;
            Transform* transformComponent = &EntityCoordinator::getInstance().GetComponent<Transform>(entFirst);
            PhysicsComponent* physComponent = &EntityCoordinator::getInstance().GetComponent<PhysicsComponent>(entFirst);
              // Error: calling inside a Box2D callback, as it happen inside the step, during which the World is locked.
            //physComponent->box2dBody->SetTransform(b2Vec2(0, 0), physComponent->box2dBody->GetAngle()); 
        }
        else if (tagSecond == ENEMYSPAWNER) {
            cout << "EnemySpawner" << endl;
        }
        else
        {
            cout << endl;
        }
    }
    else if (tagFirst == ENEMY) {
        cout << "Enemy contact with: ";
        if (tagSecond == PLATFORM) {
            /*cout << "Platform" << endl;
            cout << "X point: " << contact->GetManifold()->localPoint.x << endl;
            cout << "Y point: " << contact->GetManifold()->localPoint.y << endl;

            RenderComponent* renderComponent = &EntityCoordinator::getInstance().GetComponent<RenderComponent>(entFirst);
            MovementComponent* moveComponent = &EntityCoordinator::getInstance().GetComponent<MovementComponent>(entFirst);
            StateComponent* stateComponent = &EntityCoordinator::getInstance().GetComponent<StateComponent>(entFirst);

            float xVel = moveComponent->getVelocity().x;
            float yVel = moveComponent->getVelocity().y;*/


            /*if (contact->GetManifold()->localPoint.x == -0.5) {
                renderComponent->flipX = false;
                moveComponent->setVelocity(2.0f, yVel);
            }
            else if (contact->GetManifold()->localPoint.x == 0.5) {
                renderComponent->flipX = true;
                moveComponent->setVelocity(-2.0f, yVel);
            }*/
        }
        else if (tagSecond == WALL) {
            cout << "Wall" << endl;
            cout << "X point: " << contact->GetManifold()->localPoint.x << endl;
            cout << "Y point: " << contact->GetManifold()->localPoint.y << endl;

            RenderComponent* renderComponent = &EntityCoordinator::getInstance().GetComponent<RenderComponent>(entFirst);
            MovementComponent* moveComponent = &EntityCoordinator::getInstance().GetComponent<MovementComponent>(entFirst);
            StateComponent* stateComponent = &EntityCoordinator::getInstance().GetComponent<StateComponent>(entFirst);

            float xVel = moveComponent->getVelocity().x;
            float yVel = moveComponent->getVelocity().y;

            xVel = renderComponent->flipX ? 2.0 : -2.0;
            renderComponent->flipX = renderComponent->flipX ? false : true;
            moveComponent->setVelocity(xVel, yVel);
        }
        else if (tagSecond == BULLET) {
            cout << "Bullet" << endl;
            EntityCoordinator::getInstance().GetComponent<PhysicsComponent>(entFirst).isFlaggedForDelete = true;
            EntityCoordinator::getInstance().GetComponent<PhysicsComponent>(entSecond).isFlaggedForDelete = true;
        }
        else if (tagSecond == FIRE) {
            cout << "Fire" << endl;
            EntityCoordinator::getInstance().GetComponent<PhysicsComponent>(entFirst).isFlaggedForDelete = true;
        }
    }
    else if (tagFirst == BULLET) {
        cout << "Bullet contact with: ";
        if (tagSecond == PLATFORM) {
            cout << "Platform" << endl;
            EntityCoordinator::getInstance().GetComponent<PhysicsComponent>(entFirst).isFlaggedForDelete = true;
        }
        else
        {
            cout << endl;
        }
    }
}

void ContactListener::EndContact(b2Contact* contact) {
    PhysicsComponent* physicsComponentA = reinterpret_cast<PhysicsComponent*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
    PhysicsComponent* physicsComponentB = reinterpret_cast<PhysicsComponent*>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);

    Tag tagA = EntityCoordinator::getInstance().getTagsForEntity(physicsComponentA->entityID)[0];
    Tag tagB = EntityCoordinator::getInstance().getTagsForEntity(physicsComponentB->entityID)[0];

    Tag tagFirst, tagSecond;
    EntityID entFirst, entSecond;

    if (tagA < tagB) {
        tagFirst = tagA;
        tagSecond = tagB;
        entFirst = physicsComponentA->entityID;
        entSecond = physicsComponentB->entityID;
    }
    else {
        tagFirst = tagB;
        tagSecond = tagA;
        entFirst = physicsComponentB->entityID;
        entSecond = physicsComponentA->entityID;
    }


    if (tagFirst == PLAYER) {
        if (tagSecond == PLATFORM) {

        }
        else if (tagSecond == ENEMY) {
            Notify(Event::C_END_PLAYER_ENEMY, nullptr);
        }
    }
}

void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {

    PhysicsComponent* physicsComponentA = reinterpret_cast<PhysicsComponent*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
    PhysicsComponent* physicsComponentB = reinterpret_cast<PhysicsComponent*>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);

    Tag tagA = EntityCoordinator::getInstance().getTagsForEntity(physicsComponentA->entityID)[0];
    Tag tagB = EntityCoordinator::getInstance().getTagsForEntity(physicsComponentB->entityID)[0];

    Tag tagFirst, tagSecond;
    EntityID entFirst, entSecond;

    if (tagA < tagB)
    {
        tagFirst = tagA;
        tagSecond = tagB;
        entFirst = physicsComponentA->entityID;
        entSecond = physicsComponentB->entityID;
    }
    else
    {
        tagFirst = tagB;
        tagSecond = tagA;
        entFirst = physicsComponentB->entityID;
        entSecond = physicsComponentA->entityID;
    }


    if (tagFirst == PLAYER)
    {
        if (tagSecond == ENEMY || tagSecond == STAR)
        {
            contact->SetEnabled(false);
        }
    }
}

void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {

}

void ContactListener::Notify(Event e, void* args)
{
    for (IObserver* o : observerList) {
        o->Receive(e, args);
    }
}

bool ContactListener::GetFirstContact(Tag entityTag, EntityID id) {
    return EntityCoordinator::getInstance().entityHasTag(entityTag, id);
}

bool ContactListener::GetSecondContact(Tag entityTag, EntityID id) {
    return EntityCoordinator::getInstance().entityHasTag(entityTag, id);
}
