#pragma once

#include "PluginProcessor.h"
#include "UI/MZLookAndFeel.h"
#include "UI/MeterPanel.h"

class MasterZasterAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit MasterZasterAudioProcessorEditor (MasterZasterAudioProcessor&);
    ~MasterZasterAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    MasterZasterAudioProcessor& processorRef;

    // Debe declararse antes que los componentes que lo usan (se destruye al final).
    mz::ui::MZLookAndFeel lookAndFeel;

    juce::Slider gainSlider { juce::Slider::RotaryHorizontalVerticalDrag, juce::Slider::TextBoxBelow };
    juce::AudioProcessorValueTreeState::SliderAttachment gainAttachment;

    mz::ui::MeterPanel meterPanel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MasterZasterAudioProcessorEditor)
};
