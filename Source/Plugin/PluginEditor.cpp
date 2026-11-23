#include "PluginEditor.h"

namespace
{
    constexpr int kWidth        = 520;
    constexpr int kMeterH       = 130;   // panel de medidores (bajo el knob)
    constexpr int kHeight       = 380 + kMeterH;
    constexpr int kHeaderH      = 64;
    constexpr int kFooterH      = 36;
    constexpr int kKnobAreaSize = 200;   // zona del knob (cuadrada)
    constexpr int kTextBoxH     = 26;    // caja de valor bajo el knob
    constexpr int kMeterMargin  = 40;    // margen lateral del panel de medidores

    // Linea horizontal que se desvanece hacia los bordes, con halo.
    void drawGlowLine (juce::Graphics& g, float y, float width)
    {
        juce::ColourGradient grad (juce::Colours::transparentBlack, 0.0f, y,
                                   juce::Colours::transparentBlack, width, y, false);
        grad.addColour (0.5, mz::ui::accent().withAlpha (0.9f));

        g.setGradientFill (grad);
        g.fillRect (0.0f, y - 0.5f, width, 1.5f);

        juce::ColourGradient halo (juce::Colours::transparentBlack, 0.0f, y,
                                   juce::Colours::transparentBlack, width, y, false);
        halo.addColour (0.5, mz::ui::accent().withAlpha (0.15f));
        g.setGradientFill (halo);
        g.fillRect (0.0f, y - 3.0f, width, 6.0f);
    }

    // Esquinas tipo HUD.
    void drawCornerBrackets (juce::Graphics& g, juce::Rectangle<float> r, float len)
    {
        juce::Path p;
        p.startNewSubPath (r.getX(), r.getY() + len);          p.lineTo (r.getX(), r.getY());                 p.lineTo (r.getX() + len, r.getY());
        p.startNewSubPath (r.getRight() - len, r.getY());      p.lineTo (r.getRight(), r.getY());             p.lineTo (r.getRight(), r.getY() + len);
        p.startNewSubPath (r.getX(), r.getBottom() - len);     p.lineTo (r.getX(), r.getBottom());            p.lineTo (r.getX() + len, r.getBottom());
        p.startNewSubPath (r.getRight() - len, r.getBottom()); p.lineTo (r.getRight(), r.getBottom());        p.lineTo (r.getRight(), r.getBottom() - len);

        g.setColour (mz::ui::accent().withAlpha (0.55f));
        g.strokePath (p, juce::PathStrokeType (1.5f));
    }
}

MasterZasterAudioProcessorEditor::MasterZasterAudioProcessorEditor (MasterZasterAudioProcessor& p)
    : juce::AudioProcessorEditor (&p),
      processorRef (p),
      gainAttachment (p.apvts, MasterZasterAudioProcessor::outputGainId, gainSlider),
      meterPanel ([&p] { return p.getMeterSnapshot(); })
{
    gainSlider.setLookAndFeel (&lookAndFeel);
    gainSlider.setTextValueSuffix (" dB");
    gainSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 110, kTextBoxH);
    addAndMakeVisible (gainSlider);
    addAndMakeVisible (meterPanel);

    setSize (kWidth, kHeight);
}

MasterZasterAudioProcessorEditor::~MasterZasterAudioProcessorEditor()
{
    gainSlider.setLookAndFeel (nullptr);
}

void MasterZasterAudioProcessorEditor::paint (juce::Graphics& g)
{
    const auto bounds = getLocalBounds().toFloat();
    const float w = bounds.getWidth();
    const float h = bounds.getHeight();

    // Fondo: degradado de carbono
    g.setGradientFill (juce::ColourGradient::vertical (mz::ui::panelMid(), 0.0f, mz::ui::panelDark(), h));
    g.fillAll();

    // Rejilla tecnica muy tenue
    g.setColour (juce::Colours::white.withAlpha (0.025f));
    for (int gx = 0; gx < getWidth(); gx += 20)  g.drawVerticalLine (gx, 0.0f, h);
    for (int gy = 0; gy < getHeight(); gy += 20) g.drawHorizontalLine (gy, 0.0f, w);

    // Resplandor ambiental tras el knob
    const juce::Point<float> knobCentre ((float) gainSlider.getBounds().getCentreX(),
                                         (float) gainSlider.getBounds().getY() + kKnobAreaSize * 0.5f);
    g.setGradientFill (juce::ColourGradient (mz::ui::accent().withAlpha (0.10f), knobCentre.x, knobCentre.y,
                                             juce::Colours::transparentBlack, knobCentre.x + 170.0f, knobCentre.y, true));
    g.fillAll();

    // Vineta
    g.setGradientFill (juce::ColourGradient (juce::Colours::transparentBlack, w * 0.5f, h * 0.5f,
                                             juce::Colours::black.withAlpha (0.55f), 0.0f, 0.0f, true));
    g.fillAll();

    // Cabecera
    juce::AttributedString title;
    title.setJustification (juce::Justification::centred);
    title.append ("MASTER", juce::Font (juce::FontOptions (26.0f, juce::Font::bold)), juce::Colours::white);
    title.append ("ZASTER", juce::Font (juce::FontOptions (26.0f, juce::Font::bold)), mz::ui::accent());
    title.draw (g, juce::Rectangle<float> (0.0f, 0.0f, w, (float) kHeaderH));
    drawGlowLine (g, (float) kHeaderH, w);

    // Etiqueta del control
    g.setColour (mz::ui::textDim());
    g.setFont (juce::FontOptions (11.0f, juce::Font::bold));
    g.drawText ("OUTPUT GAIN",
                juce::Rectangle<int> (0, gainSlider.getY() - 20, getWidth(), 16),
                juce::Justification::centred);

    // Separador sobre los medidores
    const float meterTop = (float) meterPanel.getY() - 6.0f;
    g.setColour (mz::ui::accent().withAlpha (0.25f));
    g.fillRect ((float) kMeterMargin, meterTop, w - 2.0f * kMeterMargin, 1.0f);

    // Pie (estado real del hito)
    g.setColour (mz::ui::textDim().withAlpha (0.8f));
    g.setFont (juce::FontOptions (11.0f));
    g.drawText ("M1 - METERING (INPUT)",
                getLocalBounds().removeFromBottom (kFooterH),
                juce::Justification::centred);

    // Marco y esquinas
    g.setColour (juce::Colours::white.withAlpha (0.07f));
    g.drawRoundedRectangle (bounds.reduced (0.5f), 6.0f, 1.0f);
    drawCornerBrackets (g, bounds.reduced (10.0f), 16.0f);
}

void MasterZasterAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    area.removeFromTop (kHeaderH);
    area.removeFromBottom (kFooterH);

    meterPanel.setBounds (area.removeFromBottom (kMeterH).reduced (kMeterMargin, 8));

    gainSlider.setBounds (area.withSizeKeepingCentre (kKnobAreaSize, kKnobAreaSize + kTextBoxH)
                              .translated (0, 8));
}
