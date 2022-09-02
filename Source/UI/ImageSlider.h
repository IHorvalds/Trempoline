#pragma once

#include <JuceHeader.h>


class ImageSlider : public juce::Slider
{
public:

    ImageSlider() = default;
    ImageSlider(juce::RangedAudioParameter *rap, juce::Image image, int imageHeight);
    ~ImageSlider() = default;

    juce::Image m_image;
    int m_imageHeight = 0;

    void paint(juce::Graphics& g) override;
private:

    juce::RangedAudioParameter* m_param = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ImageSlider)
};