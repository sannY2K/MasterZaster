#pragma once

namespace mz
{
    // Conversiones dB <-> ganancia lineal. Sin dependencias de JUCE.
    float decibelsToGain (float decibels, float minusInfinityDb = -100.0f) noexcept;
    float gainToDecibels (float gain, float minusInfinityDb = -100.0f) noexcept;
}
