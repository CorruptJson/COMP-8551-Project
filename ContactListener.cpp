#include "ContactListener.h"

ContactListener::~ContactListener()
{
    //delete userDataA;
    //delete userDataB;
}

void ContactListener::BeginContact(b2Contact* contact) {
    //std::cout << "Contact made!" << std::endl;

    // use the ID for entity queries
    EntityUserData* entUserDataA = reinterpret_cast<EntityUserData*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
    EntityUserData* entUserDataB = reinterpret_cast<EntityUserData*>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);

    //userDataA = entUserDataA;
    //userDataB = entUserDataB;

    if (GetFirstContact(PLAYER, entUserDataA->id)) {

        cout << "Player contact with: ";
        if (GetSecondContact(PLATFORM, entUserDataB->id)) {
            cout << "platform" << endl;
        }
        else if (GetSecondContact(ENEMY, entUserDataB->id)) {
            // testing enemy move on collision
            PhysicsComponent physComp = EntityCoordinator::getInstance().GetComponent<PhysicsComponent>(entUserDataB->id);
            physComp.box2dBody->SetLinearVelocity(b2Vec2(2.0, 0));
            cout << "enemy" << endl;
        }
    }
    else if (GetFirstContact(PLAYER, entUserDataB->id)) {
        cout << "Player contact with: ";
        if (GetSecondContact(PLATFORM, entUserDataA->id)) {
            cout << "platform" << endl;
        }
        else if (GetSecondContact(ENEMY, entUserDataA->id)) {
            // testing enemy move on collision
            PhysicsComponent physComp = EntityCoordinator::getInstance().GetComponent<PhysicsComponent>(entUserDataA->id);
            physComp.box2dBody->SetLinearVelocity(b2Vec2(2.0, 0));
            cout << "enemy" << endl;
        }
    }

    if (GetFirstContact(ENEMY, entUserDataA->id)) {
        cout << "Enemy contact with: ";
        if (GetSecondContact(PLATFORM, entUserDataB->id)) {
            cout << "platform" << endl;
        }
        else if (GetSecondContact(PLAYER, entUserDataB->id)) {
            cout << "player" << endl;
        }
    }
    
    printf("Contact bodyA X-Pos: %0.2f Y-Pos %0.2f\n", contact->GetFixtureA()->GetBody()->GetPosition().x, contact->GetFixtureA()->GetBody()->GetPosition().y);
    printf("Contact bodyB X-Pos: %0.2f Y-Pos %0.2f\n", contact->GetFixtureB()->GetBody()->GetPosition().x, contact->GetFixtureB()->GetBody()->GetPosition().y);


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
