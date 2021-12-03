#include "Interpolator.h"

void Interpolator::setInterpolation(float domainStart_, float domainEnd_, float targetStart_, float targetEnd_) {
    domainStart = domainStart_;
    domainEnd = domainEnd_;
    targetStart = targetStart_;
    targetEnd = targetEnd_;
}

float Interpolator::interpolate(float value) {
    if (value > domainEnd || value < domainStart) throw std::runtime_error("Value out of range of interpolate domain.");
    return (value - domainStart) / (domainEnd - domainStart) * (targetEnd - targetStart) + targetStart;
}
