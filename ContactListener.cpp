#include "ContactListener.h"

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
        if (tagSecond == ENEMY) {
            Notify(Event::C_START_PLAYER_ENEMY, nullptr);
        }
        if (tagSecond == PLATFORM) {
        }
        else if (tagSecond == WALL) {
        }
        else if (tagSecond == STAR) {
            ec->GetComponent<PhysicsComponent>(entSecond).isFlaggedForDelete = true;
            Notify(Event::STAR_PICKED_UP, nullptr);
        }
        else if (tagSecond == FIRE) {
            Notify(Event::C_PLAYER_FIRE, nullptr);
        }

    }
    else if (tagFirst == ENEMY) {
        if (tagSecond == PLATFORM) {

        }
        else if (tagSecond == WALL) {
            RenderComponent* renderComponent = &ec->GetComponent<RenderComponent>(entFirst);
            MovementComponent* moveComponent = &ec->GetComponent<MovementComponent>(entFirst);
            StateComponent* stateComponent = &ec->GetComponent<StateComponent>(entFirst);

            float xVel = moveComponent->getVelocity().x;
            float yVel = moveComponent->getVelocity().y;

            xVel = renderComponent->flipX ? stateComponent->speed : -stateComponent->speed;
            renderComponent->flipX = renderComponent->flipX ? false : true;
            moveComponent->setVelocity(xVel, yVel);
        }
        else if (tagSecond == BULLET) {
            Notify(Event::ENEMY_BULLET, nullptr);
            ec->GetComponent<PhysicsComponent>(entSecond).isFlaggedForDelete = true;
            StateComponent* stateComponent = &ec->GetComponent<StateComponent>(entFirst);

            ec->GetComponent<RenderComponent>(entFirst).color = glm::vec3(1.0f, 0.0f, 0.0f);

            if (--stateComponent->health == 0) {
                Notify(Event::ENEMY_DEATH, nullptr);
                ec->GetComponent<PhysicsComponent>(entFirst).isFlaggedForDelete = true;
            }

        }
        else if (tagSecond == FIRE) {
            ec->GetComponent<PhysicsComponent>(entFirst).isFlaggedForDelete = true;
        }
    }
    else if (tagFirst == BULLET) {
        if (tagSecond == PLATFORM || tagSecond == WALL) {
            ec->GetComponent<PhysicsComponent>(entFirst).isFlaggedForDelete = true;
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
