#include "ContactListener.h"

void ContactListener::BeginContact(b2Contact* contact) {
    std::cout << "Contact made!" << std::endl;

    // use the ID for entity queries
    EntityUserData* entUserDataA = reinterpret_cast<EntityUserData*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
    EntityUserData* entUserDataB = reinterpret_cast<EntityUserData*>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);

    PhysicsComponent physComponentA = EntityCoordinator::getInstance().GetComponent<PhysicsComponent>(entUserDataA->id);

    if (EntityCoordinator::getInstance().entityHasTag(Tag::PLATFORM, entUserDataA->id)
        || EntityCoordinator::getInstance().entityHasTag(Tag::PLATFORM, entUserDataB->id)) {

        cout << "Wall contact with: ";
        if (EntityCoordinator::getInstance().entityHasTag(Tag::PLAYER, entUserDataA->id)
            || EntityCoordinator::getInstance().entityHasTag(Tag::PLAYER, entUserDataB->id)) {
            cout << "player" << endl;
        }
        else if (EntityCoordinator::getInstance().entityHasTag(Tag::ENEMY, entUserDataA->id)
            || EntityCoordinator::getInstance().entityHasTag(Tag::ENEMY, entUserDataB->id)) {
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
