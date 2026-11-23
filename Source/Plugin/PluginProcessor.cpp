#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "Dsp/Decibels.h"

MasterZasterAudioProcessor::MasterZasterAudioProcessor()
    : juce::AudioProcessor (BusesProperties()
                                .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                                .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      apvts (*this, nullptr, "MasterZasterState", createParameterLayout())
{
    outputGainParam = apvts.getRawParameterValue (outputGainId);
}

juce::AudioProcessorValueTreeState::ParameterLayout MasterZasterAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { outputGainId, 1 },
        "Output Gain",
        juce::NormalisableRange<float> (-24.0f, 24.0f, 0.1f),
        0.0f,
        juce::AudioParameterFloatAttributes().withLabel ("dB")));

    return layout;
}

void MasterZasterAudioProcessor::prepareToPlay (double sampleRate, int /*samplesPerBlock*/)
{
    gainSmoothed.reset (sampleRate, 0.02);
    gainSmoothed.setCurrentAndTargetValue (mz::decibelsToGain (outputGainParam->load()));

    liveMeters.prepare (sampleRate);
}

bool MasterZasterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    const auto& out = layouts.getMainOutputChannelSet();

    if (out != juce::AudioChannelSet::mono() && out != juce::AudioChannelSet::stereo())
        return false;

    return layouts.getMainInputChannelSet() == out;
}

void MasterZasterAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    // Limpia canales de salida sin entrada correspondiente.
    for (int ch = getTotalNumInputChannels(); ch < getTotalNumOutputChannels(); ++ch)
        buffer.clear (ch, 0, buffer.getNumSamples());

    const int numChannels = buffer.getNumChannels();
    const int numSamples  = buffer.getNumSamples();

    // M1: medicion de la entrada, ANTES de la ganancia de salida. No modifica el audio.
    liveMeters.process (buffer.getArrayOfReadPointers(), numChannels, numSamples);

    // El audio pasa sin cambios salvo la ganancia de salida (0 dB por defecto).
    gainSmoothed.setTargetValue (mz::decibelsToGain (outputGainParam->load()));

    if (gainSmoothed.isSmoothing())
    {
        for (int i = 0; i < numSamples; ++i)
        {
            const float g = gainSmoothed.getNextValue();
            for (int ch = 0; ch < numChannels; ++ch)
                buffer.getWritePointer (ch)[i] *= g;
        }
    }
    else
    {
        const float g = gainSmoothed.getTargetValue();
        if (g != 1.0f)
            buffer.applyGain (g);
    }
}

juce::AudioProcessorEditor* MasterZasterAudioProcessor::createEditor()
{
    return new MasterZasterAudioProcessorEditor (*this);
}

void MasterZasterAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    if (auto xml = apvts.copyState().createXml())
        copyXmlToBinary (*xml, destData);
}

void MasterZasterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary (data, sizeInBytes))
        if (xml->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xml));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MasterZasterAudioProcessor();
}
