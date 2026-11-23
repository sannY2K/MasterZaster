#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "Metering/LevelMeter.h"
#include "Metering/StereoMeter.h"

#include <algorithm>
#include <cmath>
#include <vector>

using Catch::Matchers::WithinAbs;

namespace
{
    constexpr double kPi = 3.14159265358979323846;

    // Senoidal con un numero ENTERO de ciclos (RMS exacto): amplitud * sin(2*pi*cycles*n/N)
    std::vector<float> sine (int numSamples, int cycles, float amplitude, double phase = 0.0)
    {
        std::vector<float> v ((size_t) numSamples);
        for (int n = 0; n < numSamples; ++n)
            v[(size_t) n] = (float) (amplitude * std::sin (2.0 * kPi * cycles * n / numSamples + phase));
        return v;
    }
}

// ---------------------------------------------------------------- LevelAccumulator

TEST_CASE ("LevelAccumulator: senoidal de amplitud 1")
{
    const auto x = sine (48000, 100, 1.0f);
    const float* ch[] = { x.data() };

    mz::LevelAccumulator m;
    m.process (ch, 1, (int) x.size());

    REQUIRE_THAT (m.peak(),          WithinAbs (1.0f,      1e-4));
    REQUIRE_THAT (m.rms(),           WithinAbs (0.707107f, 1e-4));
    REQUIRE_THAT (m.rmsDb(),         WithinAbs (-3.0103f,  1e-2));
    REQUIRE_THAT (m.peakDb(),        WithinAbs (0.0f,      1e-2));
    REQUIRE_THAT (m.crestFactorDb(), WithinAbs (3.0103f,   1e-2));
}

TEST_CASE ("LevelAccumulator: cuadrada a escala completa tiene cresta 0 dB")
{
    std::vector<float> x (1000);
    for (size_t n = 0; n < x.size(); ++n)
        x[n] = (n % 2 == 0) ? 1.0f : -1.0f;
    const float* ch[] = { x.data() };

    mz::LevelAccumulator m;
    m.process (ch, 1, (int) x.size());

    REQUIRE_THAT (m.rms(),           WithinAbs (1.0f, 1e-6));
    REQUIRE_THAT (m.crestFactorDb(), WithinAbs (0.0f, 1e-4));
}

TEST_CASE ("LevelAccumulator: DC constante")
{
    std::vector<float> x (500, 0.25f);
    const float* ch[] = { x.data() };

    mz::LevelAccumulator m;
    m.process (ch, 1, (int) x.size());

    REQUIRE_THAT (m.peak(),   WithinAbs (0.25f, 1e-6));
    REQUIRE_THAT (m.rms(),    WithinAbs (0.25f, 1e-6));
    REQUIRE_THAT (m.peakDb(), WithinAbs (-12.0412f, 1e-3));
}

TEST_CASE ("LevelAccumulator: silencio y sin datos")
{
    mz::LevelAccumulator m;
    REQUIRE (m.peakDb() == -100.0f);
    REQUIRE (m.rmsDb()  == -100.0f);
    REQUIRE (m.crestFactorDb() == 0.0f);

    std::vector<float> x (256, 0.0f);
    const float* ch[] = { x.data() };
    m.process (ch, 1, (int) x.size());

    REQUIRE (m.peakDb() == -100.0f);
    REQUIRE (m.rmsDb()  == -100.0f);
    REQUIRE (m.crestFactorDb() == 0.0f);
}

TEST_CASE ("LevelAccumulator: entradas invalidas se ignoran")
{
    mz::LevelAccumulator m;
    m.process (nullptr, 2, 100);

    float dummy[4] = { 1, 1, 1, 1 };
    const float* ch[] = { dummy };
    m.process (ch, 0, 4);
    m.process (ch, 1, 0);

    REQUIRE (m.sampleCount() == 0);
}

TEST_CASE ("LevelAccumulator: procesar por bloques equivale a procesar todo junto")
{
    const auto x = sine (48000, 37, 0.8f);
    const float* whole[] = { x.data() };

    mz::LevelAccumulator a, b;
    a.process (whole, 1, (int) x.size());

    const int block = 480;
    for (int start = 0; start < (int) x.size(); start += block)
    {
        const float* part[] = { x.data() + start };
        b.process (part, 1, block);
    }

    REQUIRE_THAT (b.peak(), WithinAbs (a.peak(), 1e-7));
    REQUIRE_THAT (b.rms(),  WithinAbs (a.rms(),  1e-7));
    REQUIRE (a.sampleCount() == b.sampleCount());
}

TEST_CASE ("LevelAccumulator: multicanal usa todos los canales")
{
    std::vector<float> loud (1000, 0.5f), quiet (1000, 0.0f);
    const float* ch[] = { loud.data(), quiet.data() };

    mz::LevelAccumulator m;
    m.process (ch, 2, 1000);

    REQUIRE_THAT (m.peak(), WithinAbs (0.5f, 1e-6));
    // media de cuadrados: (0.25 + 0) / 2 = 0.125 -> rms = 0.353553
    REQUIRE_THAT (m.rms(),  WithinAbs (0.353553f, 1e-5));
    REQUIRE (m.sampleCount() == 2000);
}

TEST_CASE ("LevelAccumulator: reset")
{
    std::vector<float> x (100, 1.0f);
    const float* ch[] = { x.data() };

    mz::LevelAccumulator m;
    m.process (ch, 1, 100);
    m.reset();

    REQUIRE (m.sampleCount() == 0);
    REQUIRE (m.peak() == 0.0f);
    REQUIRE (m.rmsDb() == -100.0f);
}

// ---------------------------------------------------------------- StereoAccumulator

TEST_CASE ("StereoAccumulator: L == R es mono (correlacion +1, anchura 0)")
{
    const auto x = sine (48000, 50, 0.7f);
    mz::StereoAccumulator s;
    s.process (x.data(), x.data(), (int) x.size());

    REQUIRE_THAT (s.correlation(), WithinAbs (1.0f, 1e-5));
    REQUIRE_THAT (s.width(),       WithinAbs (0.0f, 1e-5));
}

TEST_CASE ("StereoAccumulator: fase invertida (correlacion -1, anchura 1)")
{
    const auto l = sine (48000, 50, 0.7f);
    std::vector<float> r (l.size());
    for (size_t n = 0; n < l.size(); ++n)
        r[n] = -l[n];

    mz::StereoAccumulator s;
    s.process (l.data(), r.data(), (int) l.size());

    REQUIRE_THAT (s.correlation(), WithinAbs (-1.0f, 1e-5));
    REQUIRE_THAT (s.width(),       WithinAbs (1.0f,  1e-5));
}

TEST_CASE ("StereoAccumulator: seno y coseno estan descorrelados")
{
    const auto l = sine (48000, 50, 0.7f, 0.0);
    const auto r = sine (48000, 50, 0.7f, kPi / 2.0);

    mz::StereoAccumulator s;
    s.process (l.data(), r.data(), (int) l.size());

    REQUIRE_THAT (s.correlation(), WithinAbs (0.0f, 1e-4));
}

TEST_CASE ("StereoAccumulator: solo un canal con senal tiene anchura 0.5")
{
    const auto l = sine (48000, 50, 0.7f);
    std::vector<float> r (l.size(), 0.0f);

    mz::StereoAccumulator s;
    s.process (l.data(), r.data(), (int) l.size());

    REQUIRE_THAT (s.width(),       WithinAbs (0.5f, 1e-5));
    REQUIRE_THAT (s.correlation(), WithinAbs (0.0f, 1e-6));   // un canal silencioso -> 0
}

TEST_CASE ("StereoAccumulator: silencio y entradas invalidas")
{
    mz::StereoAccumulator s;
    s.process (nullptr, nullptr, 100);
    REQUIRE (s.sampleCount() == 0);
    REQUIRE (s.correlation() == 0.0f);
    REQUIRE (s.width() == 0.0f);

    std::vector<float> z (128, 0.0f);
    s.process (z.data(), z.data(), 128);
    REQUIRE (s.correlation() == 0.0f);
    REQUIRE (s.width() == 0.0f);
}

TEST_CASE ("StereoAccumulator: bloques equivalen a todo junto")
{
    const auto l = sine (48000, 31, 0.6f, 0.3);
    const auto r = sine (48000, 31, 0.4f, 1.1);

    mz::StereoAccumulator a, b;
    a.process (l.data(), r.data(), (int) l.size());
    for (int start = 0; start < (int) l.size(); start += 512)
    {
        const int n = std::min (512, (int) l.size() - start);
        b.process (l.data() + start, r.data() + start, n);
    }

    REQUIRE_THAT (b.correlation(), WithinAbs (a.correlation(), 1e-6));
    REQUIRE_THAT (b.width(),       WithinAbs (a.width(),       1e-6));
}
