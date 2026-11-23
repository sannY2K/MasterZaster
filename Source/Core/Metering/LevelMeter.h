#pragma once

#include <cstdint>

namespace mz
{
    // Acumulador de nivel: pico, RMS y factor de cresta. Sin dependencias de JUCE.
    //
    // - Seguro para el hilo de audio: sin asignaciones, sin locks, sin E/S.
    // - Se puede alimentar por bloques de cualquier tamano; el resultado es el mismo
    //   que procesando todo de una vez.
    // - RMS = raiz de la media de cuadrados de TODAS las muestras de TODOS los canales,
    //   sin compensacion de +3 dB (una senoidal de amplitud 1 da -3.01 dBFS).
    // - Silencio: pico y RMS en dB valen -100 (suelo), y el factor de cresta vale 0.
    class LevelAccumulator
    {
    public:
        static constexpr float floorDb = -100.0f;

        void reset() noexcept;

        // channels[c][n]: un puntero por canal, numSamples muestras por canal.
        void process (const float* const* channels, int numChannels, int numSamples) noexcept;

        float peak() const noexcept;      // lineal, >= 0
        float rms() const noexcept;       // lineal, >= 0
        float peakDb() const noexcept;    // dBFS
        float rmsDb() const noexcept;     // dBFS
        float crestFactorDb() const noexcept; // peakDb - rmsDb (0 si hay silencio)

        std::uint64_t sampleCount() const noexcept { return count; }   // muestras totales (todos los canales)

    private:
        float         peakValue  = 0.0f;
        double        sumSquares = 0.0;
        std::uint64_t count      = 0;
    };
}
