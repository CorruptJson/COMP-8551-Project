#pragma once

#include <iostream>
#include <box2d/box2d.h>


class ContactListener : public b2ContactListener {
    void BeginContact(b2Contact* contact) {
        //check if fixture A was a ball
        void* bodyUserData = &contact->GetFixtureA()->GetBody()->GetUserData();
    
        //check if fixture B was a ball
        bodyUserData = &contact->GetFixtureB()->GetBody()->GetUserData();
       /* if (bodyUserData)
            static_cast<SomeShapeCreated*>(bodyUserData)->startContact();*/
        m_contacting = true;
        std::cout << "Contact made!" << std::endl;
    };
    void EndContact(b2Contact* contact) {};
    void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {};
    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {};
public:
    //Ball class member variable
    bool m_contacting;




};
