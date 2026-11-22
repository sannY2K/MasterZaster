#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "Dsp/Decibels.h"

using Catch::Matchers::WithinAbs;

TEST_CASE ("decibelsToGain: valores conocidos")
{
    REQUIRE_THAT (mz::decibelsToGain (0.0f),   WithinAbs (1.0f, 1e-6));
    REQUIRE_THAT (mz::decibelsToGain (-6.0f),  WithinAbs (0.501187f, 1e-5));
    REQUIRE_THAT (mz::decibelsToGain (6.0f),   WithinAbs (1.995262f, 1e-5));
    REQUIRE (mz::decibelsToGain (-100.0f) == 0.0f);
}

TEST_CASE ("gainToDecibels: valores conocidos")
{
    REQUIRE_THAT (mz::gainToDecibels (1.0f),  WithinAbs (0.0f, 1e-6));
    REQUIRE_THAT (mz::gainToDecibels (0.5f),  WithinAbs (-6.0206f, 1e-3));
    REQUIRE (mz::gainToDecibels (0.0f) == -100.0f);
}

TEST_CASE ("ida y vuelta dB -> ganancia -> dB")
{
    for (float db = -60.0f; db <= 12.0f; db += 3.0f)
        REQUIRE_THAT (mz::gainToDecibels (mz::decibelsToGain (db)), WithinAbs (db, 1e-3));
}
