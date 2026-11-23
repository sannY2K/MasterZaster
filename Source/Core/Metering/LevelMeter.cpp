#include "Metering/LevelMeter.h"
#include "Dsp/Decibels.h"

#include <cmath>

namespace mz
{
    void LevelAccumulator::reset() noexcept
    {
        peakValue  = 0.0f;
        sumSquares = 0.0;
        count      = 0;
    }

    void LevelAccumulator::process (const float* const* channels, int numChannels, int numSamples) noexcept
    {
        if (channels == nullptr || numChannels <= 0 || numSamples <= 0)
            return;

        for (int c = 0; c < numChannels; ++c)
        {
            const float* data = channels[c];
            if (data == nullptr)
                continue;

            float  localPeak = peakValue;
            double localSum  = 0.0;

            for (int n = 0; n < numSamples; ++n)
            {
                const float s = data[n];
                const float a = std::fabs (s);
                if (a > localPeak)
                    localPeak = a;
                localSum += (double) s * (double) s;
            }

            peakValue   = localPeak;
            sumSquares += localSum;
            count      += (std::uint64_t) numSamples;
        }
    }

    float LevelAccumulator::peak() const noexcept { return peakValue; }

    float LevelAccumulator::rms() const noexcept
    {
        if (count == 0)
            return 0.0f;

        return (float) std::sqrt (sumSquares / (double) count);
    }

    float LevelAccumulator::peakDb() const noexcept { return gainToDecibels (peak(), floorDb); }
    float LevelAccumulator::rmsDb()  const noexcept { return gainToDecibels (rms(),  floorDb); }

    float LevelAccumulator::crestFactorDb() const noexcept
    {
        if (peak() <= 0.0f || rms() <= 0.0f)
            return 0.0f;

        return peakDb() - rmsDb();
    }
}
