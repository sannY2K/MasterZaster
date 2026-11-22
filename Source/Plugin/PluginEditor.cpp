#include "PluginEditor.h"

MasterZasterAudioProcessorEditor::MasterZasterAudioProcessorEditor (MasterZasterAudioProcessor& p)
    : juce::AudioProcessorEditor (&p),
      processorRef (p),
      gainAttachment (p.apvts, MasterZasterAudioProcessor::outputGainId, gainSlider)
{
    gainSlider.setTextValueSuffix (" dB");
    addAndMakeVisible (gainSlider);

    gainLabel.setText ("Output Gain", juce::dontSendNotification);
    gainLabel.setJustificationType (juce::Justification::centred);
    gainLabel.attachToComponent (&gainSlider, false);
    addAndMakeVisible (gainLabel);

    setSize (480, 320);
}

void MasterZasterAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff111318));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (26.0f, juce::Font::bold));
    g.drawText ("MasterZaster", getLocalBounds().removeFromTop (60), juce::Justification::centred);

    g.setColour (juce::Colours::grey);
    g.setFont (juce::FontOptions (13.0f));
    g.drawText ("M0 - esqueleto (el audio pasa sin procesar)",
                getLocalBounds().removeFromBottom (30), juce::Justification::centred);
}

void MasterZasterAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced (40);
    area.removeFromTop (50);
    gainSlider.setBounds (area.withSizeKeepingCentre (160, 180).withTrimmedTop (20));
}
