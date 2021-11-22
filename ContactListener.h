#pragma once

#include <iostream>
#include "TextComponent.h"
#include "Libraries/include/box2d/box2d.h"
#include "EntityUserData.h"

class ContactListener : public b2ContactListener, public ISubject {
public:

    ~ContactListener();

    void BeginContact(b2Contact* contact);
    void EndContact(b2Contact* contact);
    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
    void Notify(Event e, void* arg) override;
private:
    //EntityUserData* userDataA;
    //EntityUserData* userDataB;

    bool GetFirstContact(Tag entityTag, EntityID id);
    bool GetSecondContact(Tag entityTag, EntityID id);
};
