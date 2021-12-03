#pragma once
#include <string>
#include <vector>
#include "Event.h"

using namespace std;
class IObserver {
public:
    virtual ~IObserver() {};
    virtual void Receive(Event e, void* args) = 0;
};
