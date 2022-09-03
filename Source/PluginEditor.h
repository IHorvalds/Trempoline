#pragma once

#include "PluginProcessor.h"
#include "UI/ImageSlider.h"

//==============================================================================
class TrempolineProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    explicit TrempolineProcessorEditor (TrempolineProcessor&);
    ~TrempolineProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TrempolineProcessor& processorRef;

    float aspectRatio = 0.f;

    using APVTS = juce::AudioProcessorValueTreeState;
    using SliderAttachment = APVTS::SliderAttachment;
    using ButtonAttachment = APVTS::ButtonAttachment;

    ImageSlider speedSlider;
    ImageSlider depthSlider;
    ImageSlider shapeSlider;
    ImageSlider driveSlider;
    ImageSlider makeupSlider;

     //bypassButton;
    ImageButton bypassButton;

    SliderAttachment speedSliderAttachment;
    SliderAttachment depthSliderAttachment;
    SliderAttachment shapeSliderAttachment;
    SliderAttachment driveSliderAttachment;
    SliderAttachment makeupSliderAttachment;

    ButtonAttachment bypassButtonAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TrempolineProcessorEditor)
};
