#include "ContactListener.h"

ContactListener::~ContactListener()
{
    //delete userDataA;
    //delete userDataB;
}

void ContactListener::BeginContact(b2Contact* contact) {
    //std::cout << "Contact made!" << std::endl;

    // use the ID for entity queries
    //EntityUserData* entUserDataA = reinterpret_cast<EntityUserData*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
    //EntityUserData* entUserDataB = reinterpret_cast<EntityUserData*>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);
    PhysicsComponent* physicsComponentA = reinterpret_cast<PhysicsComponent*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
    PhysicsComponent* physicsComponentB = reinterpret_cast<PhysicsComponent*>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);

    //userDataA = entUserDataA;
    //userDataB = entUserDataB;

    if (GetFirstContact(PLAYER, physicsComponentA->entityID)) {

        cout << "Player contact with: ";
        if (GetSecondContact(PLATFORM, physicsComponentB->entityID)) {
            cout << "platform" << endl;
        }
        else if (GetSecondContact(ENEMY, physicsComponentB->entityID)) {
            // testing enemy move on collision
            //PhysicsComponent physComp = EntityCoordinator::getInstance().GetComponent<PhysicsComponent>(entUserDataB->id);
            //physComp.box2dBody->SetLinearVelocity(b2Vec2(2.0, 0));
            cout << "enemy" << endl;
        }
    }
    else if (GetFirstContact(PLAYER, physicsComponentB->entityID)) {
        cout << "Player contact with: ";
        if (GetSecondContact(PLATFORM, physicsComponentA->entityID)) {
            cout << "platform" << endl;
        }
        else if (GetSecondContact(ENEMY, physicsComponentA->entityID)) {
            // testing enemy move on collision
            //PhysicsComponent physComp = EntityCoordinator::getInstance().GetComponent<PhysicsComponent>(entUserDataA->id);
            //physComp.box2dBody->SetLinearVelocity(b2Vec2(2.0, 0));
            cout << "enemy" << endl;
        }
    }

    if (GetFirstContact(ENEMY, physicsComponentA->entityID)) {
        cout << "Enemy contact with: ";
        if (GetSecondContact(PLATFORM, physicsComponentB->entityID)) {
            cout << "platform" << endl;
        }
        else if (GetSecondContact(PLAYER, physicsComponentB->entityID)) {
            cout << "player" << endl;
        }
        else if (GetSecondContact(BULLET, physicsComponentB->entityID)) {
            cout << "bullet" << endl;
            EntityCoordinator::getInstance().GetComponent<PhysicsComponent>(physicsComponentA->entityID).isFlaggedForDelete = true;
            EntityCoordinator::getInstance().GetComponent<PhysicsComponent>(physicsComponentB->entityID).isFlaggedForDelete = true;
        }
    }

    if (GetFirstContact(BULLET, physicsComponentA->entityID)) {
        cout << "bullet contact with: ";
        if (GetSecondContact(PLATFORM, physicsComponentB->entityID)) {
            cout << "platform" << endl;
            EntityCoordinator::getInstance().GetComponent<PhysicsComponent>(physicsComponentA->entityID).isFlaggedForDelete = true;
        }
    }
    else if (GetFirstContact(BULLET, physicsComponentB->entityID))
    {
        cout << "bullet contact with: ";
        if (GetSecondContact(PLATFORM, physicsComponentA->entityID))
        {
            cout << "platform" << endl;
            EntityCoordinator::getInstance().GetComponent<PhysicsComponent>(physicsComponentB->entityID).isFlaggedForDelete = true;
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
