#include "ContactListener.h"

void ContactListener::BeginContact(b2Contact* contact) {
    std::cout << "Contact made!" << std::endl;

    // use the ID for entity queries
    EntityUserData* entUserDataA = reinterpret_cast<EntityUserData*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
    EntityUserData* entUserDataB = reinterpret_cast<EntityUserData*>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);


    if (GetFirstContact(Tag::PLAYER, entUserDataA->id) || GetFirstContact(Tag::PLAYER, entUserDataB->id)) {
        cout << "Player contact with: ";
        if (GetSecondContact(Tag::PLATFORM, entUserDataA->id) || GetSecondContact(Tag::PLATFORM, entUserDataB->id)) {
            cout << "platform" << endl;
        }
        else if (GetSecondContact(Tag::ENEMY, entUserDataA->id) || GetSecondContact(Tag::ENEMY, entUserDataB->id)) {
            cout << "enemy" << endl;
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
