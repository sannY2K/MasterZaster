#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "Metering/LiveMeters.h"

#include <algorithm>
#include <cmath>
#include <vector>

using Catch::Matchers::WithinAbs;

namespace
{
    constexpr double kPi = 3.14159265358979323846;

    // Genera 'total' muestras de una senoidal continua y las pasa al medidor por bloques.
    // gainR permite escalar el canal derecho (p. ej. -1 = fase invertida).
    void feedSine (mz::LiveMeters& m, int total, int block, double freq, double fs,
                   float amplitude, float gainR, bool stereo)
    {
        std::vector<float> l ((size_t) block), r ((size_t) block);
        int done = 0;
        while (done < total)
        {
            const int n = std::min (block, total - done);
            for (int i = 0; i < n; ++i)
            {
                const float s = (float) (amplitude * std::sin (2.0 * kPi * freq * (double) (done + i) / fs));
                l[(size_t) i] = s;
                r[(size_t) i] = s * gainR;
            }
            if (stereo)
            {
                const float* ch[] = { l.data(), r.data() };
                m.process (ch, 2, n);
            }
            else
            {
                const float* ch[] = { l.data() };
                m.process (ch, 1, n);
            }
            done += n;
        }
    }
}

TEST_CASE ("LiveMeters: valores iniciales")
{
    mz::LiveMeters m;
    m.prepare (48000.0);
    const auto s = m.snapshot();

    REQUIRE (s.peakDb == -100.0f);
    REQUIRE (s.rmsDb  == -100.0f);
    REQUIRE (s.crestDb == 0.0f);
    REQUIRE (s.correlation == 0.0f);
    REQUIRE (s.width == 0.0f);
}

TEST_CASE ("LiveMeters: no publica hasta completar la ventana")
{
    mz::LiveMeters m;
    m.prepare (44100.0);                       // ventana = 2205 muestras

    std::vector<float> x (2204, 0.5f);
    const float* ch[] = { x.data(), x.data() };
    m.process (ch, 2, 2204);
    REQUIRE (m.snapshot().peakDb == -100.0f);  // aun no

    m.process (ch, 2, 1);                      // completa la ventana
    REQUIRE_THAT (m.snapshot().peakDb, WithinAbs (-6.0206f, 1e-3));
}

TEST_CASE ("LiveMeters: senoidal mono por bloques pequenos")
{
    mz::LiveMeters m;
    m.prepare (48000.0);
    feedSine (m, 9600, 96, 100.0, 48000.0, 0.5f, 1.0f, false);   // 96 divide 2400: ventanas de 5 ciclos exactos

    const auto s = m.snapshot();
    REQUIRE_THAT (s.peakDb,      WithinAbs (-6.0206f, 1e-2));
    REQUIRE_THAT (s.rmsDb,       WithinAbs (-9.0309f, 1e-2));
    REQUIRE_THAT (s.crestDb,     WithinAbs (3.0103f,  1e-2));
    REQUIRE_THAT (s.correlation, WithinAbs (1.0f,     1e-4));   // mono -> L == R
    REQUIRE_THAT (s.width,       WithinAbs (0.0f,     1e-4));
}

TEST_CASE ("LiveMeters: estereo con fase invertida")
{
    mz::LiveMeters m;
    m.prepare (48000.0);
    feedSine (m, 9600, 512, 100.0, 48000.0, 0.5f, -1.0f, true);

    const auto s = m.snapshot();
    REQUIRE_THAT (s.correlation, WithinAbs (-1.0f, 1e-4));
    REQUIRE_THAT (s.width,       WithinAbs (1.0f,  1e-4));
}

TEST_CASE ("LiveMeters: tras la senal, el silencio vuelve al suelo")
{
    mz::LiveMeters m;
    m.prepare (48000.0);
    feedSine (m, 4800, 480, 100.0, 48000.0, 0.5f, 1.0f, true);
    REQUIRE (m.snapshot().peakDb > -10.0f);

    std::vector<float> z (480, 0.0f);
    const float* ch[] = { z.data(), z.data() };
    for (int i = 0; i < 10; ++i)               // 4800 muestras de silencio = 2 ventanas
        m.process (ch, 2, 480);

    const auto s = m.snapshot();
    REQUIRE (s.peakDb == -100.0f);
    REQUIRE (s.rmsDb  == -100.0f);
    REQUIRE (s.correlation == 0.0f);
}

TEST_CASE ("LiveMeters: bloques mas grandes que la ventana")
{
    mz::LiveMeters m;
    m.prepare (48000.0);                       // ventana = 2400
    std::vector<float> x (8192, 0.25f);
    const float* ch[] = { x.data(), x.data() };
    m.process (ch, 2, 8192);

    REQUIRE_THAT (m.snapshot().peakDb, WithinAbs (-12.0412f, 1e-3));
}

TEST_CASE ("LiveMeters: entradas invalidas se ignoran")
{
    mz::LiveMeters m;
    m.prepare (48000.0);
    m.process (nullptr, 2, 1000);

    float d[4] = { 1, 1, 1, 1 };
    const float* ch[] = { d };
    m.process (ch, 0, 4);
    m.process (ch, 1, 0);

    REQUIRE (m.snapshot().peakDb == -100.0f);
}
