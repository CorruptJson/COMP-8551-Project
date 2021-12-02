#pragma once
#include <stdexcept>

class Interpolator {
public:
    void setInterpolation(float domainStart_, float domainEnd_, float targetStart_, float targetEnd_);
    float interpolate(float value);
private:
    float domainStart; 
    float domainEnd; 
    float targetStart;
    float targetEnd;
};
