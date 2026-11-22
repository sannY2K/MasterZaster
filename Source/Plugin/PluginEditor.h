#pragma once

#include "PluginProcessor.h"

class MasterZasterAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit MasterZasterAudioProcessorEditor (MasterZasterAudioProcessor&);
    ~MasterZasterAudioProcessorEditor() override = default;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    MasterZasterAudioProcessor& processorRef;

    juce::Slider gainSlider { juce::Slider::RotaryHorizontalVerticalDrag, juce::Slider::TextBoxBelow };
    juce::Label  gainLabel;
    juce::AudioProcessorValueTreeState::SliderAttachment gainAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MasterZasterAudioProcessorEditor)
};
