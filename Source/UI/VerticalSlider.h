#pragma once

#include <JuceHeader.h>

class BigVerticalSlider : public juce::Slider
{
public:

    BigVerticalSlider();
    BigVerticalSlider(RangedAudioParameter *rap);
    ~BigVerticalSlider() = default;

    float lineThickness = 14.f;
    float buttonDiameter = 4.f;
    int textHeight = 14;
    int labelHeight = 4;

    juce::String bottomLabel;
    juce::String centreLabel;
    juce::String topLabel;

    void paint(Graphics& g) override;
    juce::String getDisplayText() const;

private:
    RangedAudioParameter *m_param;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BigVerticalSlider)
};