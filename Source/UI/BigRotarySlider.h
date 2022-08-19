#pragma once

#include <JuceHeader.h>

typedef unsigned int LabelPosition;
#define CENTER 0
#define TOP_POSITION 1

struct LabelType
{
    std::unique_ptr<juce::String> s;
    std::unique_ptr<juce::Image> i;
};

class BigRotarySlider : public juce::Slider
{
public:

    BigRotarySlider();
    BigRotarySlider(juce::RangedAudioParameter *rap, LabelPosition labelPosition);
    ~BigRotarySlider() = default;

    float lineThickness = 4.f;
    float buttonDiameter = 14.f;
    int textHeight = 14;
    int labelHeight = 10;
    LabelType leftLabel;
    LabelType rightLabel;

    void paint(juce::Graphics& g) override;
    juce::String getDisplayString() const;
private:

    juce::RangedAudioParameter* m_param;
    LabelPosition m_labelPosition = CENTER;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BigRotarySlider)
};