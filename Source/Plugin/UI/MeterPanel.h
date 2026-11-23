#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

#include "Metering/LiveMeters.h"
#include "MZLookAndFeel.h"

#include <functional>

// Panel de medidores en vivo: pico (con retencion), RMS, cresta, correlacion y anchura.
// Solo dibuja: lee un MeterSnapshot por callback a ~30 Hz desde el hilo de la interfaz.
namespace mz::ui
{
    class MeterPanel final : public juce::Component,
                             private juce::Timer
    {
    public:
        using SnapshotSource = std::function<mz::MeterSnapshot()>;

        explicit MeterPanel (SnapshotSource source) : getSnapshot (std::move (source))
        {
            setOpaque (false);
            startTimerHz (30);
        }

        ~MeterPanel() override { stopTimer(); }

        void paint (juce::Graphics& g) override
        {
            auto area = getLocalBounds().toFloat();
            const float rowH = area.getHeight() / 5.0f;

            auto nextRow = [&area, rowH]() { return area.removeFromTop (rowH).reduced (0.0f, 2.5f); };

            drawLevelRow (g, nextRow(), "PEAK", peakShown, &peakHold);
            drawLevelRow (g, nextRow(), "RMS",  rmsShown,  nullptr);
            drawCorrelationRow (g, nextRow());
            drawWidthRow (g, nextRow());
            drawCrestRow (g, nextRow());
        }

    private:
        static constexpr float kMinDb = -60.0f;

        static constexpr float kLabelW = 52.0f;
        static constexpr float kValueW = 70.0f;

        void timerCallback() override
        {
            snap = getSnapshot();

            // Pico: sube al instante, cae 1 dB por cuadro (~30 dB/s).
            peakShown = juce::jmax (snap.peakDb, peakShown - 1.0f);
            if (snap.peakDb >= peakHold)
            {
                peakHold = snap.peakDb;
                holdTicks = 45;                       // ~1.5 s
            }
            else if (holdTicks > 0)
            {
                --holdTicks;
            }
            else
            {
                peakHold = juce::jmax (snap.peakDb, peakHold - 0.8f);
            }

            // RMS suavizado.
            rmsShown += 0.35f * (snap.rmsDb - rmsShown);

            repaint();
        }

        static float dbToFrac (float db) noexcept
        {
            return juce::jlimit (0.0f, 1.0f, (db - kMinDb) / (0.0f - kMinDb));
        }

        static juce::String dbText (float db)
        {
            return db <= kMinDb ? juce::String ("-inf") : juce::String (db, 1) + " dB";
        }

        static juce::Colour levelColour (float db)
        {
            if (db > -1.0f) return juce::Colour (0xffff4d5e);   // rojo
            if (db > -6.0f) return juce::Colour (0xffffb347);   // ambar
            return accent();
        }

        static void drawLabel (juce::Graphics& g, juce::Rectangle<float> row, const juce::String& text)
        {
            g.setColour (textDim());
            g.setFont (juce::FontOptions (11.0f, juce::Font::bold));
            g.drawText (text, row.withWidth (kLabelW), juce::Justification::centredLeft);
        }

        static void drawValue (juce::Graphics& g, juce::Rectangle<float> row, const juce::String& text,
                               juce::Colour colour)
        {
            g.setColour (colour);
            g.setFont (juce::FontOptions (12.0f));
            g.drawText (text, row.removeFromRight (kValueW), juce::Justification::centredRight);
        }

        static juce::Rectangle<float> barArea (juce::Rectangle<float> row)
        {
            row.removeFromLeft (kLabelW);
            row.removeFromRight (kValueW + 6.0f);
            return row.withSizeKeepingCentre (row.getWidth(), 8.0f);
        }

        static void drawTrack (juce::Graphics& g, juce::Rectangle<float> bar)
        {
            g.setColour (juce::Colours::black.withAlpha (0.65f));
            g.fillRoundedRectangle (bar.expanded (1.0f), 4.0f);
            g.setColour (juce::Colour (0xff1d232b));
            g.fillRoundedRectangle (bar, 3.0f);
        }

        static void fillGlow (juce::Graphics& g, juce::Rectangle<float> fill, juce::Colour c)
        {
            if (fill.getWidth() < 1.0f)
                return;

            g.setColour (c.withAlpha (0.12f));
            g.fillRoundedRectangle (fill.expanded (0.0f, 3.0f), 4.0f);
            g.setColour (c);
            g.fillRoundedRectangle (fill, 3.0f);
        }

        void drawLevelRow (juce::Graphics& g, juce::Rectangle<float> row, const juce::String& label,
                           float db, const float* holdDb) const
        {
            drawLabel (g, row, label);
            auto bar = barArea (row);
            drawTrack (g, bar);

            fillGlow (g, bar.withWidth (bar.getWidth() * dbToFrac (db)), levelColour (db));

            if (holdDb != nullptr && *holdDb > kMinDb)
            {
                const float x = bar.getX() + bar.getWidth() * dbToFrac (*holdDb);
                g.setColour (levelColour (*holdDb));
                g.fillRect (x - 1.0f, bar.getY() - 2.0f, 2.0f, bar.getHeight() + 4.0f);
            }

            drawValue (g, row, dbText (db), levelColour (db));
        }

        void drawCorrelationRow (juce::Graphics& g, juce::Rectangle<float> row) const
        {
            drawLabel (g, row, "CORR");
            auto bar = barArea (row);
            drawTrack (g, bar);

            const float c = juce::jlimit (-1.0f, 1.0f, snap.correlation);
            const float cx = bar.getCentreX();
            const float half = bar.getWidth() * 0.5f;
            const auto colour = c < 0.0f ? juce::Colour (0xffff4d5e) : accent();

            juce::Rectangle<float> fill (juce::jmin (cx, cx + c * half), bar.getY(),
                                         std::abs (c) * half, bar.getHeight());
            fillGlow (g, fill, colour);

            g.setColour (juce::Colours::white.withAlpha (0.35f));
            g.fillRect (cx - 0.5f, bar.getY() - 2.0f, 1.0f, bar.getHeight() + 4.0f);

            drawValue (g, row, (c >= 0.0f ? "+" : "") + juce::String (c, 2), colour);
        }

        void drawWidthRow (juce::Graphics& g, juce::Rectangle<float> row) const
        {
            drawLabel (g, row, "WIDTH");
            auto bar = barArea (row);
            drawTrack (g, bar);

            const float w = juce::jlimit (0.0f, 1.0f, snap.width);
            fillGlow (g, bar.withWidth (bar.getWidth() * w), accent());

            drawValue (g, row, juce::String (juce::roundToInt (w * 100.0f)) + " %", accent());
        }

        void drawCrestRow (juce::Graphics& g, juce::Rectangle<float> row) const
        {
            drawLabel (g, row, "CREST");

            g.setColour (accent());
            g.setFont (juce::FontOptions (12.0f));
            g.drawText (juce::String (snap.crestDb, 1) + " dB",
                        row.withTrimmedLeft (kLabelW), juce::Justification::centredLeft);
        }

        SnapshotSource getSnapshot;
        mz::MeterSnapshot snap;

        float peakShown = kMinDb;
        float peakHold  = kMinDb;
        float rmsShown  = kMinDb;
        int   holdTicks = 0;
    };
}
