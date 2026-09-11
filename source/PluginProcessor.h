#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

const juce::ParameterID noiseParamID{ "noise", 1 };
const juce::ParameterID lpfCutoffParamID{ "lpf_cutoff", 1 };
const juce::ParameterID hpfCutoffParamID{ "hpf_cutoff", 1 };
const juce::ParameterID lpfResParamID{ "lpf_res", 1 };
const juce::ParameterID hpfResParamID{ "hpf_res", 1 };

//==============================================================================
class AudioPluginAudioProcessor final : public juce::AudioProcessor
{
public:
    //==============================================================================
    AudioPluginAudioProcessor();
    ~AudioPluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState apvts;

private:
    //==============================================================================
    juce::Random random;
    juce::dsp::StateVariableTPTFilter<float> LPF;
    juce::dsp::StateVariableTPTFilter<float> HPF;

    juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> delayLine;
    juce::dsp::Oscillator<float> delayModulator;

    juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedDelay;

    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    juce::AudioParameterFloat* noiseParam; 
    juce::AudioParameterFloat* lpfCutoffParam;
    juce::AudioParameterFloat* hpfCutoffParam;
    juce::AudioParameterFloat* lpfResParam;
    juce::AudioParameterFloat* hpfResParam;

    void reset() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessor)
};
