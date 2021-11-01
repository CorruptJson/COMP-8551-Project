#pragma once
#include <string>
using namespace std;
class IObserver {
public:
    virtual ~IObserver() {};
    virtual void Update(const string& message) = 0;
};
