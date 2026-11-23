#pragma once

#include <cstdint>

namespace mz
{
    // Correlacion y anchura estereo. Sin dependencias de JUCE; seguro para el hilo de audio.
    //
    // - correlation(): coeficiente de correlacion entre L y R, en [-1, +1].
    //     +1 = mono (L == R), 0 = descorrelado, -1 = fase invertida.
    //     Si L o R son silencio, devuelve 0.
    // - width(): energia lateral relativa, en [0, 1]. Con M = (L+R)/2 y S = (L-R)/2:
    //     width = rmsS / (rmsM + rmsS).
    //     0 = mono puro, 0.5 = solo un canal con senal, 1 = solo lateral (fase invertida).
    //     Si no hay senal, devuelve 0.
    class StereoAccumulator
    {
    public:
        void reset() noexcept;
        void process (const float* left, const float* right, int numSamples) noexcept;

        float correlation() const noexcept;
        float width() const noexcept;

        std::uint64_t sampleCount() const noexcept { return count; }   // muestras por canal

    private:
        double        sumLL = 0.0;
        double        sumRR = 0.0;
        double        sumLR = 0.0;
        double        sumMM = 0.0;
        double        sumSS = 0.0;
        std::uint64_t count = 0;
    };
}
