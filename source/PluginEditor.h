#pragma once

#include "PluginProcessor.h"

//==============================================================================
class AudioPluginAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor&);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void showElement();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioPluginAudioProcessor& processorRef;

    juce::Label noiseKnobLabel;
    juce::Slider noiseKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> noiseKnobAttach;

    // cutoff (filters)
    juce::Label lpfCutoffKnobLabel;
    juce::Slider lpfCutoffKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lpfCutoffKnobAttach;
    juce::Label hpfCutoffKnobLabel;
    juce::Slider hpfCutoffKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> hpfCutoffKnobAttach;

    // resonance (filters)
    juce::Label lpfResoKnobLabel;
    juce::Slider lpfResoKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lpfResoKnobAttach;
    juce::Label hpfResoKnobLabel;
    juce::Slider hpfResoKnob;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> hpfResoKnobAttach;

    juce::Image rundfunkWood;
    juce::Image rundfunkBrushedMetal;
    juce::Image rundfunkMetalGrid;
    juce::Image rundfunkMetallicBar;
    juce::Image rundfunkButtonPanel;
    juce::Image rundfunkAuthorPanel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};
