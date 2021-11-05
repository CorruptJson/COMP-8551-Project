#pragma once
#include <iostream>
#include "system.h"
#include "IObserver.h"

class PrinterSystem : public System, public IObserver
{
public:
    void Receive(Event event, EntityQuery* eq, list<int> indexList) override;
};

