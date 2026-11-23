#pragma once

#include "Metering/LevelMeter.h"
#include "Metering/StereoMeter.h"

#include <atomic>
#include <cstdint>

namespace mz
{
    // Valores publicados para la interfaz (ventana de medicion mas reciente).
    struct MeterSnapshot
    {
        float peakDb      = LevelAccumulator::floorDb;
        float rmsDb       = LevelAccumulator::floorDb;
        float crestDb     = 0.0f;
        float correlation = 0.0f;
        float width       = 0.0f;
    };

    // Medidor en vivo: el hilo de audio llama a process(); la interfaz lee snapshot().
    //
    // - process(): un solo hilo (el de audio). Sin asignaciones, sin locks.
    // - snapshot(): cualquier hilo. Cada campo se lee de forma atomica por separado;
    //   dos campos pueden pertenecer a ventanas consecutivas distintas (irrelevante
    //   para una pantalla).
    // - Cada ventana (por defecto 50 ms, como minimo) se mide, se publica y se reinicia.
    //   La ventana se cierra al final del bloque en que alcanza ese tamano, asi que su
    //   duracion real puede ser algo mayor. El pico es
    //   el maximo de la ventana, no un pico retenido: la retencion/caida visual la
    //   decide la interfaz.
    // - Con una sola senal (mono), L y R se toman como el mismo canal: correlacion +1
    //   y anchura 0 si hay senal.
    // - prepare() NO es seguro llamarlo mientras process() se ejecuta.
    class LiveMeters
    {
    public:
        void prepare (double sampleRate, double windowSeconds = 0.05) noexcept;
        void process (const float* const* channels, int numChannels, int numSamples) noexcept;
        MeterSnapshot snapshot() const noexcept;

    private:
        void publishDefaults() noexcept;

        LevelAccumulator  level;
        StereoAccumulator stereo;

        std::uint64_t windowSamples = 2400;
        std::uint64_t windowCount   = 0;

        std::atomic<float> peakDbValue      { LevelAccumulator::floorDb };
        std::atomic<float> rmsDbValue       { LevelAccumulator::floorDb };
        std::atomic<float> crestDbValue     { 0.0f };
        std::atomic<float> correlationValue { 0.0f };
        std::atomic<float> widthValue       { 0.0f };
    };
}
