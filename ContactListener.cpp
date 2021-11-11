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
    Tag tagA = EntityCoordinator::getInstance().getTagsForEntity(entUserDataA->id)[0];
    Tag tagB = EntityCoordinator::getInstance().getTagsForEntity(entUserDataB->id)[0];

    Tag tagFirst, tagSecond;
    EntityID entFirst, entSecond;

    if (tagA < tagB)
    {
        tagFirst = tagA;
        tagSecond = tagB;
        entFirst = entUserDataA->id;
        entSecond = entUserDataB->id;
    }
    else
    {
        tagFirst = tagB;
        tagSecond = tagA;
        entFirst = entUserDataB->id;
        entSecond = entUserDataA->id;
    }

    if (GetFirstContact(PLAYER, entUserDataA->id)) {

        cout << "Player contact with: ";
        if (GetSecondContact(PLATFORM, entUserDataB->id)) {
            cout << "platform" << endl;
        }
        else if (GetSecondContact(ENEMY, entUserDataB->id)) {
            // testing enemy move on collision
            //PhysicsComponent physComp = EntityCoordinator::getInstance().GetComponent<PhysicsComponent>(entUserDataB->id);
            //physComp.box2dBody->SetLinearVelocity(b2Vec2(2.0, 0));
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
            //PhysicsComponent physComp = EntityCoordinator::getInstance().GetComponent<PhysicsComponent>(entUserDataA->id);
            //physComp.box2dBody->SetLinearVelocity(b2Vec2(2.0, 0));
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
        else if (GetSecondContact(BULLET, entUserDataB->id)) {
            cout << "bullet" << endl;
            EntityCoordinator::getInstance().GetComponent<PhysicsComponent>(entUserDataA->id).isFlaggedForDelete = true;
            EntityCoordinator::getInstance().GetComponent<PhysicsComponent>(entUserDataB->id).isFlaggedForDelete = true;
        }
    }

    if (GetFirstContact(BULLET, entUserDataA->id)) {
        cout << "bullet contact with: ";
        if (GetSecondContact(PLATFORM, entUserDataB->id)) {
            cout << "platform" << endl;
            EntityCoordinator::getInstance().GetComponent<PhysicsComponent>(entUserDataA->id).isFlaggedForDelete = true;
        }
    }
    else if (GetFirstContact(BULLET, entUserDataB->id))
    {
        cout << "bullet contact with: ";
        if (GetSecondContact(PLATFORM, entUserDataA->id))
        {
            cout << "platform" << endl;
            EntityCoordinator::getInstance().GetComponent<PhysicsComponent>(entUserDataB->id).isFlaggedForDelete = true;
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
