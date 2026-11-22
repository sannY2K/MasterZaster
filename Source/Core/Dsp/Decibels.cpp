#include "Decibels.h"

#include <cmath>

namespace mz
{
    float decibelsToGain (float decibels, float minusInfinityDb) noexcept
    {
        return decibels > minusInfinityDb ? std::pow (10.0f, decibels * 0.05f) : 0.0f;
    }

    float gainToDecibels (float gain, float minusInfinityDb) noexcept
    {
        return gain > 0.0f ? std::fmax (minusInfinityDb, 20.0f * std::log10 (gain)) : minusInfinityDb;
    }
}
