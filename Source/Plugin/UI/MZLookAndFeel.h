#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

// Estilo visual de MasterZaster: futurista y realista (carbono oscuro, bisel
// metalico, acento cian con brillo). Solo dibuja; no contiene logica de audio.
namespace mz::ui
{
    inline juce::Colour accent()     { return juce::Colour (0xff35e0ff); }
    inline juce::Colour accentDeep() { return juce::Colour (0xff0a7fa8); }
    inline juce::Colour panelDark()  { return juce::Colour (0xff080a0d); }
    inline juce::Colour panelMid()   { return juce::Colour (0xff161b22); }
    inline juce::Colour textDim()    { return juce::Colour (0xff7b8794); }

    class MZLookAndFeel final : public juce::LookAndFeel_V4
    {
    public:
        MZLookAndFeel()
        {
            setColour (juce::Slider::textBoxTextColourId,      accent());
            setColour (juce::Slider::textBoxBackgroundColourId, juce::Colours::transparentBlack);
            setColour (juce::Slider::textBoxOutlineColourId,    juce::Colours::transparentBlack);
            setColour (juce::Slider::textBoxHighlightColourId,  accentDeep());
        }

        void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height,
                               float sliderPos, float startAngle, float endAngle,
                               juce::Slider&) override
        {
            auto bounds = juce::Rectangle<float> ((float) x, (float) y, (float) width, (float) height).reduced (4.0f);
            const float diameter = juce::jmin (bounds.getWidth(), bounds.getHeight());
            const auto  centre   = bounds.getCentre();
            const float outerR   = diameter * 0.5f;
            const float arcR     = outerR - 14.0f;
            const float bodyR    = outerR - 32.0f;
            const float angle    = startAngle + sliderPos * (endAngle - startAngle);

            // Marcas de escala
            constexpr int numTicks = 31;
            for (int i = 0; i < numTicks; ++i)
            {
                const float t = (float) i / (float) (numTicks - 1);
                const float a = startAngle + t * (endAngle - startAngle);
                const bool major = (i % 5) == 0;
                const float r1 = outerR - (major ? 7.0f : 4.0f);
                const bool lit = t <= sliderPos;

                g.setColour (lit ? accent().withAlpha (0.9f) : textDim().withAlpha (0.35f));
                g.drawLine (juce::Line<float> (centre.getPointOnCircumference (r1, a),
                                               centre.getPointOnCircumference (outerR, a)),
                            major ? 1.6f : 1.0f);
            }

            // Pista del arco
            juce::Path track;
            track.addCentredArc (centre.x, centre.y, arcR, arcR, 0.0f, startAngle, endAngle, true);
            g.setColour (juce::Colours::black.withAlpha (0.7f));
            g.strokePath (track, juce::PathStrokeType (7.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
            g.setColour (juce::Colour (0xff1d232b));
            g.strokePath (track, juce::PathStrokeType (4.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

            // Arco de valor con brillo
            if (sliderPos > 0.001f)
            {
                juce::Path value;
                value.addCentredArc (centre.x, centre.y, arcR, arcR, 0.0f, startAngle, angle, true);

                const float glowW[]     = { 15.0f, 10.0f, 6.0f };
                const float glowAlpha[] = { 0.06f, 0.10f, 0.18f };
                for (int i = 0; i < 3; ++i)
                {
                    g.setColour (accent().withAlpha (glowAlpha[i]));
                    g.strokePath (value, juce::PathStrokeType (glowW[i], juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
                }
                g.setColour (accent());
                g.strokePath (value, juce::PathStrokeType (3.5f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
            }

            // Sombra del cuerpo
            juce::Path body;
            body.addEllipse (centre.x - bodyR, centre.y - bodyR, bodyR * 2.0f, bodyR * 2.0f);
            juce::DropShadow (juce::Colours::black.withAlpha (0.8f), 14, { 0, 6 }).drawForPath (g, body);

            // Bisel metalico
            const float bezelR = bodyR + 4.0f;
            g.setGradientFill (juce::ColourGradient::vertical (juce::Colour (0xff59626d), centre.y - bezelR,
                                                               juce::Colour (0xff0a0c0f), centre.y + bezelR));
            g.fillEllipse (centre.x - bezelR, centre.y - bezelR, bezelR * 2.0f, bezelR * 2.0f);

            // Cara del knob
            g.setGradientFill (juce::ColourGradient (juce::Colour (0xff3d4550), centre.x - bodyR * 0.35f, centre.y - bodyR * 0.45f,
                                                     juce::Colour (0xff0d1014), centre.x + bodyR * 0.8f,  centre.y + bodyR * 0.9f,
                                                     true));
            g.fillEllipse (centre.x - bodyR, centre.y - bodyR, bodyR * 2.0f, bodyR * 2.0f);

            // Anillos concentricos (aluminio mecanizado)
            g.setColour (juce::Colours::white.withAlpha (0.05f));
            for (int i = 1; i <= 6; ++i)
            {
                const float r = bodyR * (0.30f + 0.11f * (float) i);
                g.drawEllipse (centre.x - r, centre.y - r, r * 2.0f, r * 2.0f, 0.6f);
            }

            // Reflejo especular
            const float hlW = bodyR * 1.6f;
            const float hlH = bodyR * 0.9f;
            const float hlY = centre.y - bodyR * 0.95f;
            g.setGradientFill (juce::ColourGradient::vertical (juce::Colours::white.withAlpha (0.20f), hlY,
                                                               juce::Colours::white.withAlpha (0.0f),  hlY + hlH));
            g.fillEllipse (centre.x - hlW * 0.5f, hlY, hlW, hlH);

            // Filo de acento
            g.setColour (accent().withAlpha (0.25f));
            g.drawEllipse (centre.x - bodyR, centre.y - bodyR, bodyR * 2.0f, bodyR * 2.0f, 1.0f);

            // Indicador
            juce::Path pointer;
            pointer.startNewSubPath (centre.getPointOnCircumference (bodyR * 0.42f, angle));
            pointer.lineTo          (centre.getPointOnCircumference (bodyR * 0.86f, angle));
            g.setColour (accent().withAlpha (0.18f));
            g.strokePath (pointer, juce::PathStrokeType (8.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
            g.setColour (accent());
            g.strokePath (pointer, juce::PathStrokeType (3.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

            // Tapa central
            const float capR = bodyR * 0.16f;
            g.setGradientFill (juce::ColourGradient::vertical (juce::Colour (0xff2a3038), centre.y - capR,
                                                               juce::Colour (0xff050607), centre.y + capR));
            g.fillEllipse (centre.x - capR, centre.y - capR, capR * 2.0f, capR * 2.0f);
        }
    };
}
