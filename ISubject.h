#pragma once
#include "IObserver.h"
#include "Event.h"

using namespace std;
class IObserver;
class ISubject {
public:
    virtual ~ISubject() {};
    void Attach(IObserver* observer) { observerList.push_back(observer); };
    void Detach(IObserver* observer) { observerList.remove(observer); };
    virtual void Notify(Event e, void* args) {};

protected:
    list<IObserver*> observerList;
    string message;
};
