#include "BigRotarySlider.h"

BigRotarySlider::BigRotarySlider() : juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox)
{
    m_param = nullptr;
}

BigRotarySlider::BigRotarySlider(juce::RangedAudioParameter* rap, LabelPosition labelPosition) : 
    juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox),
    m_labelPosition(labelPosition)
{
    m_param = rap;
}

void BigRotarySlider::paint(juce::Graphics& g)
{
    auto sliderArea = this->getLocalBounds();
    float rectSide = jmin(sliderArea.getWidth(), sliderArea.getHeight());
    auto controlArea = Rectangle<float>(0, 0, rectSide, rectSide);

    // m_labelPosition is CENTER
    Point<float> dialCenter = Point<float>(sliderArea.getWidth() / 2, sliderArea.getTopLeft().y + rectSide * 0.5f);

    // Otherwise, modify
    int padding = 4;
    if (this->m_labelPosition == TOP_POSITION)
    {
        rectSide = jmin(sliderArea.getWidth(), sliderArea.getHeight() - this->textHeight - padding);
        controlArea = Rectangle<float>(0, 0, rectSide, rectSide);
        dialCenter = Point<float>(sliderArea.getWidth() / 2, this->textHeight + padding + sliderArea.getTopLeft().y + rectSide * 0.5f);
    }

    controlArea.setCentre(dialCenter);
    controlArea.reduce(this->lineThickness * .5f, this->lineThickness * .5f);
    g.setColour(Colours::white);
    g.drawEllipse(controlArea, this->lineThickness);

    // Button
    Path p;
    
    Rectangle<float> button(sliderArea.getCentreX() - buttonDiameter * 0.5f, controlArea.getTopLeft().y + rectSide * 0.1f, buttonDiameter, buttonDiameter);
    p.addEllipse(button);

    auto rotaryParameters = this->getRotaryParameters();
    float sliderAngle = rotaryParameters.startAngleRadians + this->valueToProportionOfLength(this->getValue()) * (rotaryParameters.endAngleRadians - rotaryParameters.startAngleRadians);

    p.applyTransform(AffineTransform().rotation(sliderAngle, controlArea.getCentreX(), controlArea.getCentreY()));
    g.fillPath(p);

    // Text
    Font font(juce::Font::getDefaultSansSerifFontName(), this->textHeight, juce::Font::bold);
    auto textArea = Rectangle<float>(0, 0, font.getStringWidth(this->getDisplayString()), this->textHeight);

    if (this->m_labelPosition == CENTER)
    {
        textArea.setCentre(controlArea.getCentre());
    }

    if (this->m_labelPosition == TOP_POSITION)
    {
        textArea.setY(sliderArea.getTopLeft().y);
        textArea.setX(sliderArea.getCentreX() - textArea.getWidth() * 0.5f);
    }

    g.setFont(font);
    g.drawFittedText(getDisplayString(), textArea.toNearestInt(), Justification::centred, 1);


    // left and right labels
    font = Font(Font::getDefaultSansSerifFontName(), labelHeight, Font::plain);
    g.setFont(font);

    if (leftLabel.i.get() != nullptr)
    {
        auto leftArea = Rectangle<float>(0, 0, this->labelHeight, this->labelHeight);
        auto bottomLeft = sliderArea.getBottomLeft();
        bottomLeft.addXY(0, -this->labelHeight);
        leftArea.setPosition(bottomLeft.toFloat());

        g.drawImage(*leftLabel.i, leftArea, RectanglePlacement::centred);
    }

    if (leftLabel.s.get() != nullptr)
    {
        auto leftArea = Rectangle<int>(0, 0, font.getStringWidthFloat(*leftLabel.s), this->labelHeight);
        auto bottomLeft = sliderArea.getBottomLeft();
        bottomLeft.addXY(padding, -this->labelHeight);
        leftArea.setPosition(bottomLeft);

        g.drawFittedText(*leftLabel.s, leftArea, Justification::centred, 1);
    }

    if (rightLabel.i.get() != nullptr)
    {
        auto rightArea = Rectangle<float>(0, 0, this->labelHeight, this->labelHeight);
        auto bottomRight = sliderArea.getBottomRight();
        bottomRight.addXY(-rightArea.getWidth(), -this->labelHeight);
        rightArea.setPosition(bottomRight.toFloat());

        g.drawImage(*rightLabel.i, rightArea, RectanglePlacement::centred);
    }

    if (rightLabel.s.get() != nullptr)
    {
        auto rightArea = Rectangle<int>(0, 0, font.getStringWidthFloat(*rightLabel.s), this->labelHeight);
        auto bottomRight = sliderArea.getBottomRight();
        bottomRight.addXY(-rightArea.getWidth() - padding, -this->labelHeight);
        rightArea.setPosition(bottomRight);

        g.drawFittedText(*rightLabel.s, rightArea, Justification::centred, 1);
    }

}

juce::String BigRotarySlider::getDisplayString() const
{
    return m_param->getName(256);
}
