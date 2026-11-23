#include "Metering/LiveMeters.h"

namespace mz
{
    void LiveMeters::publishDefaults() noexcept
    {
        peakDbValue.store      (LevelAccumulator::floorDb, std::memory_order_relaxed);
        rmsDbValue.store       (LevelAccumulator::floorDb, std::memory_order_relaxed);
        crestDbValue.store     (0.0f,                      std::memory_order_relaxed);
        correlationValue.store (0.0f,                      std::memory_order_relaxed);
        widthValue.store       (0.0f,                      std::memory_order_relaxed);
    }

    void LiveMeters::prepare (double sampleRate, double windowSeconds) noexcept
    {
        const double samples = sampleRate * windowSeconds;
        windowSamples = samples >= 1.0 ? (std::uint64_t) samples : 1u;
        windowCount   = 0;

        level.reset();
        stereo.reset();
        publishDefaults();
    }

    void LiveMeters::process (const float* const* channels, int numChannels, int numSamples) noexcept
    {
        if (channels == nullptr || numChannels <= 0 || numSamples <= 0)
            return;

        level.process (channels, numChannels, numSamples);

        const float* left  = channels[0];
        const float* right = numChannels >= 2 ? channels[1] : channels[0];
        stereo.process (left, right, numSamples);

        windowCount += (std::uint64_t) numSamples;

        if (windowCount >= windowSamples)
        {
            peakDbValue.store      (level.peakDb(),        std::memory_order_relaxed);
            rmsDbValue.store       (level.rmsDb(),         std::memory_order_relaxed);
            crestDbValue.store     (level.crestFactorDb(), std::memory_order_relaxed);
            correlationValue.store (stereo.correlation(),  std::memory_order_relaxed);
            widthValue.store       (stereo.width(),        std::memory_order_relaxed);

            level.reset();
            stereo.reset();
            windowCount = 0;
        }
    }

    MeterSnapshot LiveMeters::snapshot() const noexcept
    {
        MeterSnapshot s;
        s.peakDb      = peakDbValue.load      (std::memory_order_relaxed);
        s.rmsDb       = rmsDbValue.load       (std::memory_order_relaxed);
        s.crestDb     = crestDbValue.load     (std::memory_order_relaxed);
        s.correlation = correlationValue.load (std::memory_order_relaxed);
        s.width       = widthValue.load       (std::memory_order_relaxed);
        return s;
    }
}
