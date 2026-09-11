#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "BinaryData.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    rundfunkWood = juce::ImageCache::getFromMemory(
        BinaryData::rundfunkWood_png,
        BinaryData::rundfunkWood_pngSize
    );

    rundfunkBrushedMetal = juce::ImageCache::getFromMemory(
        BinaryData::rundfunkBrushedMetal_jpg,
        BinaryData::rundfunkBrushedMetal_jpgSize
    );

    rundfunkMetalGrid = juce::ImageCache::getFromMemory(
        BinaryData::rundfunkMetalGrid_jpg,
        BinaryData::rundfunkMetalGrid_jpgSize
    );

    rundfunkMetallicBar = juce::ImageCache::getFromMemory(
        BinaryData::rundfunkMetallicBar_png,
        BinaryData::rundfunkMetallicBar_pngSize
    );

    rundfunkButtonPanel = juce::ImageCache::getFromMemory(
        BinaryData::rundfunkButtonPanel_png,
        BinaryData::rundfunkButtonPanel_pngSize
    );

    rundfunkAuthorPanel = juce::ImageCache::getFromMemory(
        BinaryData::rundfunkAuthorPanel_png,
        BinaryData::rundfunkAuthorPanel_pngSize
    );

    // Noise
    noiseKnobLabel.setText("Noise", juce::dontSendNotification);
    noiseKnobLabel.setJustificationType(juce::Justification::centred);
    noiseKnobLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    noiseKnobLabel.setFont(juce::Font("Tahoma", 18.0f, juce::Font::plain));
    addAndMakeVisible(&noiseKnobLabel);
    addAndMakeVisible(&noiseKnob);
    noiseKnob.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    noiseKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 65, 25);
    noiseKnob.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    noiseKnob.setValue(0.0f);
    noiseKnobAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, "noise", noiseKnob);
    noiseKnob.textFromValueFunction = nullptr;
    noiseKnob.setNumDecimalPlacesToDisplay(2);
    noiseKnob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::lightgoldenrodyellow);
    noiseKnob.setColour(juce::Slider::thumbColourId, juce::Colours::white);

    // LPF Cutoff
    lpfCutoffKnobLabel.setText("LPF", juce::dontSendNotification);
    lpfCutoffKnobLabel.setJustificationType(juce::Justification::left);
    lpfCutoffKnobLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    lpfCutoffKnobLabel.setFont(juce::Font("Tahoma", 18.0f, juce::Font::plain));
    addAndMakeVisible(&lpfCutoffKnobLabel);
    addAndMakeVisible(&lpfCutoffKnob);
    lpfCutoffKnob.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    lpfCutoffKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 65, 25);
    lpfCutoffKnob.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    lpfCutoffKnob.setValue(0.0f);
    lpfCutoffKnobAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, "lpf_cutoff", lpfCutoffKnob);
    lpfCutoffKnob.textFromValueFunction = nullptr;
    lpfCutoffKnob.setNumDecimalPlacesToDisplay(2);
    lpfCutoffKnob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::lightgoldenrodyellow);
    lpfCutoffKnob.setColour(juce::Slider::thumbColourId, juce::Colours::white);

    // HPF Cutoff
    hpfCutoffKnobLabel.setText("HPF", juce::dontSendNotification);
    hpfCutoffKnobLabel.setJustificationType(juce::Justification::right);
    hpfCutoffKnobLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    hpfCutoffKnobLabel.setFont(juce::Font("Tahoma", 18.0f, juce::Font::plain));
    addAndMakeVisible(&hpfCutoffKnobLabel);
    addAndMakeVisible(&hpfCutoffKnob);
    hpfCutoffKnob.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    hpfCutoffKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 65, 25);
    hpfCutoffKnob.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    hpfCutoffKnob.setValue(0.0f);
    hpfCutoffKnobAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, "hpf_cutoff", hpfCutoffKnob);
    hpfCutoffKnob.textFromValueFunction = nullptr;
    hpfCutoffKnob.setNumDecimalPlacesToDisplay(2);
    hpfCutoffKnob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::lightgoldenrodyellow);
    hpfCutoffKnob.setColour(juce::Slider::thumbColourId, juce::Colours::white);

    // LPF Resonance
    lpfResoKnobLabel.setText("Reso", juce::dontSendNotification);
    lpfResoKnobLabel.setJustificationType(juce::Justification::left);
    lpfResoKnobLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    lpfResoKnobLabel.setFont(juce::Font("Tahoma", 18.0f, juce::Font::plain));
    addAndMakeVisible(&lpfResoKnobLabel);
    addAndMakeVisible(&lpfResoKnob);
    lpfResoKnob.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    lpfResoKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 15);
    lpfResoKnob.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    lpfResoKnob.setValue(0.0f);
    lpfResoKnobAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, "lpf_res", lpfResoKnob);
    lpfResoKnob.textFromValueFunction = nullptr;
    lpfResoKnob.setNumDecimalPlacesToDisplay(2);
    lpfResoKnob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::lightgoldenrodyellow);
    lpfResoKnob.setColour(juce::Slider::thumbColourId, juce::Colours::white);

    // HPF Resonance
    hpfResoKnobLabel.setText("Reso", juce::dontSendNotification);
    hpfResoKnobLabel.setJustificationType(juce::Justification::right);
    hpfResoKnobLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    hpfResoKnobLabel.setFont(juce::Font("Tahoma", 18.0f, juce::Font::plain));
    addAndMakeVisible(&hpfResoKnobLabel);
    addAndMakeVisible(&hpfResoKnob);
    hpfResoKnob.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    hpfResoKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 15);
    hpfResoKnob.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    hpfResoKnob.setValue(0.0f);
    hpfResoKnobAttach = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, "hpf_res", hpfResoKnob);
    hpfResoKnob.textFromValueFunction = nullptr;
    hpfResoKnob.setNumDecimalPlacesToDisplay(2);
    hpfResoKnob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::lightgoldenrodyellow);
    hpfResoKnob.setColour(juce::Slider::thumbColourId, juce::Colours::white);

    setSize (800, 300);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

void AudioPluginAudioProcessorEditor::showElement()
{
    // TODO: find a way to ensapsulate all necessary code for just getting a working slider up on the GUI here
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::rosybrown);
    g.drawImageWithin(rundfunkMetalGrid, getX(), getY(), getWidth(), getHeight(),juce::RectanglePlacement::stretchToFit); // middle amp grid
    g.drawImageWithin(rundfunkMetallicBar, 200, 0, 10, 300, juce::RectanglePlacement::stretchToFit);
    g.drawImageWithin(rundfunkMetallicBar, 590, 0, 10, 300, juce::RectanglePlacement::stretchToFit);

    g.drawImageWithin(rundfunkBrushedMetal, 0, 0, 200, 300, juce::RectanglePlacement::stretchToFit); // left panel
    g.drawImageWithin(rundfunkBrushedMetal, 600, 0, 200, 300, juce::RectanglePlacement::stretchToFit); // right panel
    g.drawImageWithin(rundfunkMetallicBar, 0, 0, 10, 300, juce::RectanglePlacement::stretchToFit);
    g.drawImageWithin(rundfunkMetallicBar, 790, 0, 10, 300, juce::RectanglePlacement::stretchToFit);

    g.drawImageWithin(rundfunkWood, 0, 0, 800, 50, juce::RectanglePlacement::stretchToFit); // top wood
    g.drawImageWithin(rundfunkMetallicBar, 0, 47.5, 800, 7.5, juce::RectanglePlacement::stretchToFit); // metal edge bar
    g.drawImageWithin(rundfunkBrushedMetal, 15, 5, 355, 35, juce::RectanglePlacement::stretchToFit); // metal title panel
    g.drawImageWithin(rundfunkAuthorPanel, 215, 10, 145, 25, juce::RectanglePlacement::stretchToFit); // author panel

    g.drawImageWithin(rundfunkWood, 0, 285, 800, 15, juce::RectanglePlacement::stretchToFit); // bottom wood
    g.drawImageWithin(rundfunkMetallicBar, 0, 280, 800, 7.5, juce::RectanglePlacement::stretchToFit); // metal edge bar

    // matte finish button panels
    //g.drawImageWithin(rundfunkButtonPanel, 15, 55, 145, 225, juce::RectanglePlacement::stretchToFit);
    //g.drawImageWithin(rundfunkButtonPanel, 635, 55, 145, 225, juce::RectanglePlacement::stretchToFit);
    g.drawImageWithin(rundfunkButtonPanel, 338, 75, 125, 185, juce::RectanglePlacement::stretchToFit);

    g.setColour (juce::Colours::black);
    g.setFont(juce::Font("Courier New", 35.0f, juce::Font::bold));
    auto titleText = getLocalBounds();

    titleText.setX(25);
    titleText.setY(5);
    g.drawFittedText("RUNDFUNK", titleText, juce::Justification::topLeft, 1);

    g.setColour(juce::Colours::white);
    g.setFont(juce::Font("Courier New", 20.0f, juce::Font::plain));
    titleText.setX(223);
    titleText.setY(13);
    g.drawFittedText("AUF LA FORME", titleText, juce::Justification::topLeft, 1);

}

void AudioPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    noiseKnob.setBounds(getWidth() / 2 - 75, getHeight() / 2 - 50, 150, 150);
    auto noiseKnobArea = juce::Rectangle(getWidth() / 2 - 75, getHeight() / 2 - 40, 150, 150);
    noiseKnobLabel.setBounds(noiseKnobArea.getX(), noiseKnobArea.getY() - 90, noiseKnobArea.getWidth(), noiseKnobArea.getHeight());

    // LPF Cutoff
    lpfCutoffKnob.setBounds(getWidth() / 20, getHeight() / 2 - 95, 125, 125);
    auto lpfCutoffArea = juce::Rectangle(getWidth() / 20, getHeight() / 2 - 100, 125, 125);
    lpfCutoffKnobLabel.setBounds(lpfCutoffArea.getX() - 20, lpfCutoffArea.getY() - 10, lpfCutoffArea.getWidth(), lpfCutoffArea.getHeight());

    // HPF Cutoff
    hpfCutoffKnob.setBounds(getWidth() - 170, getHeight() / 2 - 95, 125, 125);
    auto hpfCutoffArea = juce::Rectangle(getWidth() - 170, getHeight() / 2 - 100, 125, 125);
    hpfCutoffKnobLabel.setBounds(hpfCutoffArea.getX() + 20, hpfCutoffArea.getY() - 10, hpfCutoffArea.getWidth(), hpfCutoffArea.getHeight());

    // LPF Resonance
    lpfResoKnob.setBounds(getWidth() / 20, getHeight() / 2 + 35, 125, 90);
    auto lpfResoArea = juce::Rectangle(getWidth() / 20, getHeight() / 2 + 35, 125, 90);
    lpfResoKnobLabel.setBounds(lpfResoArea.getX() - 15, lpfResoArea.getY() - 10, lpfResoArea.getWidth(), lpfResoArea.getHeight());

    // HPF Resonance
    hpfResoKnob.setBounds(getWidth() - 170, getHeight() / 2 + 35, 125, 90);
    auto hpfResoArea = juce::Rectangle(getWidth() - 170, getHeight() / 2 + 35, 125, 90);
    hpfResoKnobLabel.setBounds(hpfResoArea.getX() + 15, hpfResoArea.getY() - 10, hpfResoArea.getWidth(), hpfResoArea.getHeight());
}
