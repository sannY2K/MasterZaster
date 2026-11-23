#include "Metering/StereoMeter.h"

#include <cmath>

namespace mz
{
    void StereoAccumulator::reset() noexcept
    {
        sumLL = sumRR = sumLR = sumMM = sumSS = 0.0;
        count = 0;
    }

    void StereoAccumulator::process (const float* left, const float* right, int numSamples) noexcept
    {
        if (left == nullptr || right == nullptr || numSamples <= 0)
            return;

        double ll = 0.0, rr = 0.0, lr = 0.0, mm = 0.0, ss = 0.0;

        for (int n = 0; n < numSamples; ++n)
        {
            const double l = (double) left[n];
            const double r = (double) right[n];
            const double m = 0.5 * (l + r);
            const double s = 0.5 * (l - r);

            ll += l * l;
            rr += r * r;
            lr += l * r;
            mm += m * m;
            ss += s * s;
        }

        sumLL += ll;  sumRR += rr;  sumLR += lr;  sumMM += mm;  sumSS += ss;
        count += (std::uint64_t) numSamples;
    }

    float StereoAccumulator::correlation() const noexcept
    {
        const double denom = std::sqrt (sumLL * sumRR);
        if (denom <= 0.0)
            return 0.0f;

        double c = sumLR / denom;
        if (c >  1.0) c =  1.0;   // proteccion ante redondeo
        if (c < -1.0) c = -1.0;
        return (float) c;
    }

    float StereoAccumulator::width() const noexcept
    {
        if (count == 0)
            return 0.0f;

        const double rmsM = std::sqrt (sumMM / (double) count);
        const double rmsS = std::sqrt (sumSS / (double) count);
        const double total = rmsM + rmsS;
        if (total <= 0.0)
            return 0.0f;

        return (float) (rmsS / total);
    }
}
