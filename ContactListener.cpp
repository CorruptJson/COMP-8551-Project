#include "ContactListener.h"

ContactListener::~ContactListener()
{

}

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
        else if (tagSecond == ENEMY) {
            cout << "Enemy" << endl;
        }
    }
    else if (tagFirst == ENEMY) {
        cout << "Enemy contact with: ";
        if (tagSecond == PLATFORM) {
            cout << "Platform" << endl;
            cout << "X point: " << contact->GetManifold()->localPoint.x << endl;

            RenderComponent* renderComponent = &EntityCoordinator::getInstance().GetComponent<RenderComponent>(entFirst);
            MovementComponent* moveComponent = &EntityCoordinator::getInstance().GetComponent<MovementComponent>(entFirst);

            float xVel = moveComponent->getVelocity().x;
            float yVel = moveComponent->getVelocity().y;
            if (contact->GetManifold()->localPoint.x == -0.5) {
                renderComponent->flipX = true;
                moveComponent->setVelocity(2.0f, yVel);
            }
            else if (contact->GetManifold()->localPoint.x == 0.5) {
                renderComponent->flipX = false;
                moveComponent->setVelocity(-2.0f, yVel);
            }
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
    }
}

void ContactListener::EndContact(b2Contact* contact) {
    
}

void ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {

}

void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {

}

bool ContactListener::GetFirstContact(Tag entityTag, EntityID id) {
    return EntityCoordinator::getInstance().entityHasTag(entityTag, id);
}

bool ContactListener::GetSecondContact(Tag entityTag, EntityID id) {
    return EntityCoordinator::getInstance().entityHasTag(entityTag, id);
}
