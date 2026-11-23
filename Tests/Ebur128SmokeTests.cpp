#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <ebur128.h>

#include <cmath>
#include <memory>
#include <vector>

TEST_CASE("libebur128 enlazada: seno estereo 997 Hz a -23 dBFS", "[ebur128][smoke]")
{
    constexpr unsigned int channels = 2;
    constexpr unsigned long sampleRate = 48000;
    constexpr std::size_t frames = sampleRate * 10;
    constexpr double pi = 3.14159265358979323846;
    const double amp = std::pow(10.0, -23.0 / 20.0);

    std::vector<float> buf(frames * channels);
    for (std::size_t i = 0; i < frames; ++i)
    {
        const auto s = static_cast<float>(
            amp * std::sin(2.0 * pi * 997.0 * static_cast<double>(i) / static_cast<double>(sampleRate)));
        buf[i * channels] = s;
        buf[i * channels + 1] = s;
    }

    std::unique_ptr<ebur128_state, void (*)(ebur128_state*)> st(
        ebur128_init(channels, sampleRate,
                     EBUR128_MODE_I | EBUR128_MODE_LRA | EBUR128_MODE_TRUE_PEAK | EBUR128_MODE_HISTOGRAM),
        [](ebur128_state* s) { if (s) ebur128_destroy(&s); });
    REQUIRE(st != nullptr);

    REQUIRE(ebur128_add_frames_float(st.get(), buf.data(), frames) == EBUR128_SUCCESS);

    double momentary = 0, shortTerm = 0, integrated = 0, lra = 0, truePeak = 0;
    REQUIRE(ebur128_loudness_momentary(st.get(), &momentary) == EBUR128_SUCCESS);
    REQUIRE(ebur128_loudness_shortterm(st.get(), &shortTerm) == EBUR128_SUCCESS);
    REQUIRE(ebur128_loudness_global(st.get(), &integrated) == EBUR128_SUCCESS);
    REQUIRE(ebur128_loudness_range(st.get(), &lra) == EBUR128_SUCCESS);
    REQUIRE(ebur128_true_peak(st.get(), 0, &truePeak) == EBUR128_SUCCESS);

    CHECK(momentary == Catch::Approx(-23.0).margin(0.1));
    CHECK(shortTerm == Catch::Approx(-23.0).margin(0.1));
    CHECK(integrated == Catch::Approx(-23.0).margin(0.2)); // histograma: resolucion 0.1 LU
    CHECK(lra == Catch::Approx(0.0).margin(0.1));
    CHECK(20.0 * std::log10(truePeak) == Catch::Approx(-23.0).margin(0.3));
}
