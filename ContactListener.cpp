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

    
    switch (tagFirst) {
        case PLAYER:
            cout << "Player contact with: ";
            if (tagSecond == PLATFORM) {
                cout << "Platform" << endl;
            }
            break;
        case ENEMY:
            cout << "Enemy contact with: ";
            if (tagSecond == PLATFORM) {
                cout << "Platform" << endl;
            }
            else if (tagSecond == BULLET) {
                cout << "Bullet" << endl;
                EntityCoordinator::getInstance().GetComponent<PhysicsComponent>(entFirst).isFlaggedForDelete = true;
                EntityCoordinator::getInstance().GetComponent<PhysicsComponent>(entSecond).isFlaggedForDelete = true;
            }
            break;
        case BULLET:
            cout << "Bullet contact with: ";
            if (tagSecond == PLATFORM) {
                cout << "Platform" << endl;
                EntityCoordinator::getInstance().GetComponent<PhysicsComponent>(entFirst).isFlaggedForDelete = true;
            }
            break;
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
