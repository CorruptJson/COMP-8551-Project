#include "ContactListener.h"

ContactListener::~ContactListener()
{
    delete userDataA;
    delete userDataB;
}

void ContactListener::BeginContact(b2Contact* contact) {
    //std::cout << "Contact made!" << std::endl;

    // use the ID for entity queries
    EntityUserData* entUserDataA = reinterpret_cast<EntityUserData*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
    EntityUserData* entUserDataB = reinterpret_cast<EntityUserData*>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);

    userDataA = entUserDataA;
    userDataB = entUserDataB;

    if (GetContact(PLAYER)) {
        cout << "Player contact with: ";
        if (GetContact(PLATFORM)) {
            cout << "platform" << endl;
        }
        else if (GetContact(ENEMY)) {
            cout << "enemy" << endl;
        }
    }

    if (GetContact(ENEMY)) {
        cout << "Enemy contact with: ";
        if (GetContact(PLATFORM)) {
            cout << "platform" << endl;
        }
        else if (GetContact(PLAYER)) {
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

bool ContactListener::GetContact(Tag entityTag) {
    return EntityCoordinator::getInstance().entityHasTag(entityTag, userDataA->id) 
        || EntityCoordinator::getInstance().entityHasTag(entityTag, userDataB->id);
}
