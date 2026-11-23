#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#include "Metering/LiveMeters.h"

class MasterZasterAudioProcessor final : public juce::AudioProcessor
{
public:
    static constexpr const char* outputGainId = "outputGain";

    MasterZasterAudioProcessor();
    ~MasterZasterAudioProcessor() override = default;

    // Audio
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override {}
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using juce::AudioProcessor::processBlock;

    // Editor
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    // Metadatos
    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    // Programas (no usados)
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int) override {}
    const juce::String getProgramName (int) override { return {}; }
    void changeProgramName (int, const juce::String&) override {}

    // Estado (lo guarda y restaura Ableton con el proyecto)
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // Mediciones de la ENTRADA (antes de la ganancia de salida). Seguro desde cualquier hilo.
    mz::MeterSnapshot getMeterSnapshot() const noexcept { return liveMeters.snapshot(); }

    juce::AudioProcessorValueTreeState apvts;

private:
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    std::atomic<float>* outputGainParam = nullptr;
    juce::SmoothedValue<float> gainSmoothed { 1.0f };

    mz::LiveMeters liveMeters;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MasterZasterAudioProcessor)
};
