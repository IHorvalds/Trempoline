#include "ImageSlider.h"

ImageSlider::ImageSlider(juce::RangedAudioParameter* rap, juce::Image image, int imageHeight) :
    juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox),
    m_image(image), m_imageHeight(imageHeight)
{
    m_param = rap;
}

void ImageSlider::paint(juce::Graphics& g)
{
    auto sliderArea = this->getLocalBounds();

    const double rotation = this->valueToProportionOfLength(this->getValue());
    const int frames = 512;
    const int frameId = (int)ceil(rotation * ((double)frames - 1.0));

    g.drawImage(m_image, sliderArea.getX(), sliderArea.getY(), sliderArea.getWidth(), sliderArea.getHeight(), 0, frameId * m_imageHeight, m_image.getWidth(), m_imageHeight);
}
