#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessor::AudioPluginAudioProcessor()
     : AudioProcessor (BusesProperties()
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                       ), apvts(*this, nullptr, "Parameters", createParameterLayout())
{
}

AudioPluginAudioProcessor::~AudioPluginAudioProcessor()
{
   
}

//==============================================================================
const juce::String AudioPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AudioPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AudioPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioPluginAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String AudioPluginAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void AudioPluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void AudioPluginAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused(sampleRate, samplesPerBlock);
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();

    delayLine.prepare(spec);
    delayLine.setMaximumDelayInSamples(spec.sampleRate * 0.05);

    smoothedDelay.setCurrentAndTargetValue(22500.0f);

    delayModulator.initialise([](float x) { return std::sin(x); });
    delayModulator.setFrequency(2000.0f);
    delayModulator.prepare(spec);

    LPF.prepare(spec);
    LPF.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    HPF.prepare(spec);
    HPF.setType(juce::dsp::StateVariableTPTFilterType::highpass);

    reset();
}

void AudioPluginAudioProcessor::reset()
{
    LPF.reset();
    delayLine.reset();
    delayModulator.reset();
}

void AudioPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}

float baseDelay = 80.0f;
float modDepth = 20.0f;

void AudioPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    float noise = *apvts.getRawParameterValue("noise");

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        juce::ignoreUnused (channelData);
        // ..do something to the data...
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            float inputSample = channelData[sample]; // get current sample

            delayLine.pushSample(channel, inputSample); // push current sample at delayLine write pointer position

            float modValue = delayModulator.processSample(0.0f); // capture current modulator value (-1.0 to 1.0)

            float readOffset = baseDelay + (modValue * noise); // calculate distance for read pointer

            float delayed = delayLine.popSample(channel, readOffset, true); // capture delayed sample at readOffset

            channelData[sample] = inputSample + (delayed * 0.5f); // output the current sample + an attenuated delayed sample
        }
    }

    LPF.setCutoffFrequency(*apvts.getRawParameterValue("lpf_cutoff"));
    LPF.setResonance(*apvts.getRawParameterValue("lpf_res"));
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    LPF.process(context);

    HPF.setCutoffFrequency(*apvts.getRawParameterValue("hpf_cutoff"));
    HPF.setResonance(*apvts.getRawParameterValue("hpf_res"));
    HPF.process(context);
}

//==============================================================================
bool AudioPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AudioPluginAudioProcessor::createEditor()
{
    return new AudioPluginAudioProcessorEditor (*this);
}

//==============================================================================
void AudioPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused (destData);
}

void AudioPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused (data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPluginAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout AudioPluginAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        noiseParamID,
        "Noise",
        juce::NormalisableRange<float> {0.0f, 50.0f},
        0.0f
    ));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        lpfCutoffParamID,
        "LPF_Cutoff",
        juce::NormalisableRange<float> {20.0f, 20000.0f},
        20000.0f
    ));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        hpfCutoffParamID,
        "HPF_Cutoff",
        juce::NormalisableRange<float> {20.0f, 20000.0f},
        20.0f
    ));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        lpfResParamID,
        "LPF_Resonance",
        juce::NormalisableRange<float> {0.5f, 5.0f},
        0.707f
    ));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        hpfResParamID,
        "HPF_Resonance",
        juce::NormalisableRange<float> {0.5f, 5.0f},
        0.707f
    ));

    return layout;
}