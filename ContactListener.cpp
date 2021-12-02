#include "ContactListener.h"

int health = 3;
ContactListener::ContactListener() {
    ec = &EntityCoordinator::getInstance();
}

ContactListener::~ContactListener()
{

}

// IMPORTANT: You cannot create/destroy Box2D entities inside these callbacks.

void ContactListener::BeginContact(b2Contact* contact) {

    PhysicsComponent* physicsComponentA = reinterpret_cast<PhysicsComponent*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
    PhysicsComponent* physicsComponentB = reinterpret_cast<PhysicsComponent*>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);

    Tag tagA = ec->getTagsForEntity(physicsComponentA->entityID)[0];
    Tag tagB = ec->getTagsForEntity(physicsComponentB->entityID)[0];

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
        //cout << "Player contact with: ";
        if (tagSecond == ENEMY) {
            cout << "Enemy" << endl;
            Notify(Event::C_START_PLAYER_ENEMY, nullptr);
        }
        if (tagSecond == PLATFORM) {
            //cout << "Platform" << endl;
        }
        else if (tagSecond == WALL) {
            //cout << "Platform" << endl;
        }
        else if (tagSecond == STAR) {
            cout << "Star" << endl;
            ec->GetComponent<PhysicsComponent>(entSecond).isFlaggedForDelete = true;
            Notify(Event::STAR_PICKED_UP, nullptr);
        }
        else if (tagSecond == FIRE) {
            cout << "Fire" << endl;
            Notify(Event::C_PLAYER_FIRE, nullptr);
        }
        //else if (tagSecond == ENEMYSPAWNER) {
        //    cout << "EnemySpawner" << endl;
        //}
        else
        {
            cout << endl;
        }
    }
    else if (tagFirst == ENEMY) {
        //cout << "Enemy contact with: ";
        if (tagSecond == PLATFORM) {
            cout << "Platform" << endl;
            cout << "X point: " << contact->GetManifold()->localPoint.x << endl;
            cout << "Y point: " << contact->GetManifold()->localPoint.y << endl;

            RenderComponent* renderComponent = &EntityCoordinator::getInstance().GetComponent<RenderComponent>(entFirst);
            MovementComponent* moveComponent = &EntityCoordinator::getInstance().GetComponent<MovementComponent>(entFirst);
            StateComponent* stateComponent = &EntityCoordinator::getInstance().GetComponent<StateComponent>(entFirst);

            float xVel = moveComponent->getVelocity().x;
            float yVel = moveComponent->getVelocity().y;
            if (moveComponent->getVelocity().x <=0) {
                if (!renderComponent->flipX) {
                    moveComponent->setVelocity(2.0f, yVel);
                }
                else {
                    moveComponent->setVelocity(-2.0f, yVel);
                }
            }

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
            //cout << "Wall" << endl;
            //cout << "X point: " << contact->GetManifold()->localPoint.x << endl;
            //cout << "Y point: " << contact->GetManifold()->localPoint.y << endl;

            RenderComponent* renderComponent = &ec->GetComponent<RenderComponent>(entFirst);
            MovementComponent* moveComponent = &ec->GetComponent<MovementComponent>(entFirst);
            StateComponent* stateComponent = &ec->GetComponent<StateComponent>(entFirst);

            float xVel = moveComponent->getVelocity().x;
            float yVel = moveComponent->getVelocity().y;

            xVel = renderComponent->flipX ? stateComponent->speed + 2 : -stateComponent->speed - 2;
            renderComponent->flipX = renderComponent->flipX ? false : true;
            moveComponent->setVelocity(xVel, yVel);
            cout << "testing x velocity touching wall: "<<xVel << endl;
        }
        else if (tagSecond == BULLET) {
            cout << "Bullet" << endl;
            ec->GetComponent<PhysicsComponent>(entSecond).isFlaggedForDelete = true;
            StateComponent* stateComponent = &ec->GetComponent<StateComponent>(entFirst);

            ec->GetComponent<RenderComponent>(entFirst).color = glm::vec3(1.0f, 0.0f, 0.0f);

            if (--stateComponent->health == 0) {
                ec->GetComponent<PhysicsComponent>(entFirst).isFlaggedForDelete = true;
            }

        }
        else if (tagSecond == FIRE) {
            cout << "Fire" << endl;
            ec->GetComponent<PhysicsComponent>(entFirst).isFlaggedForDelete = true;
        }
    }
    else if (tagFirst == BULLET) {
        //cout << "Bullet contact with: ";
        if (tagSecond == PLATFORM) {
            //cout << "Platform" << endl;
            ec->GetComponent<PhysicsComponent>(entFirst).isFlaggedForDelete = true;
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

    Tag tagA = ec->getTagsForEntity(physicsComponentA->entityID)[0];
    Tag tagB = ec->getTagsForEntity(physicsComponentB->entityID)[0];

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

    //if (tagFirst == ENEMY) {
    //    if (tagSecond == BULLET)
    //        //ec->GetComponent<RenderComponent>(entFirst).color = glm::vec3(1.0f, 1.0f, 1.0f);
    //}
}

void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {

    PhysicsComponent* physicsComponentA = reinterpret_cast<PhysicsComponent*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
    PhysicsComponent* physicsComponentB = reinterpret_cast<PhysicsComponent*>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);

    Tag tagA = ec->getTagsForEntity(physicsComponentA->entityID)[0];
    Tag tagB = ec->getTagsForEntity(physicsComponentB->entityID)[0];

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
    else if (tagFirst == ENEMY)
    {
        if (tagSecond == BULLET) contact->SetEnabled(false);
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
    return ec->entityHasTag(entityTag, id);
}

bool ContactListener::GetSecondContact(Tag entityTag, EntityID id) {
    return ec->entityHasTag(entityTag, id);
}
