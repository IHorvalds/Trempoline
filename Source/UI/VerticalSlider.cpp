#include "VerticalSlider.h"

BigVerticalSlider::BigVerticalSlider() : Slider(Slider::LinearVertical, Slider::NoTextBox)
{
    m_param = nullptr;
}

BigVerticalSlider::BigVerticalSlider(RangedAudioParameter* rap) : Slider(Slider::LinearVertical, Slider::NoTextBox)
{
    m_param = rap;
}

void BigVerticalSlider::paint(Graphics& g)
{

    auto sliderArea = getLocalBounds();

    Font font(Font::getDefaultSansSerifFontName(), this->textHeight, Font::bold);
    
    auto textArea = Rectangle<int>(0, 0, font.getStringWidth(this->getDisplayText()), this->textHeight);
    textArea.setCentre(sliderArea.getCentreX(), sliderArea.getBottom() - this->textHeight * 0.5f);

    g.setFont(font);
    g.setColour(Colours::white);

    g.drawFittedText(this->getDisplayText(), textArea, Justification::centred, 1);

    int padding = 4;
    sliderArea.removeFromBottom(this->textHeight + padding);
    auto controlArea = sliderArea;
    controlArea.removeFromTop(padding + (int)(this->buttonDiameter) / 2);

    // slider line
    Line<float> line(Point<float>(controlArea.getCentreX(), controlArea.getY()), Point<float>(controlArea.getCentreX(), controlArea.getBottom()));
    g.drawLine(line, this->lineThickness);

    g.setColour(Colour(0xEA, 0x7C, 0x7C));
    line.setEnd(controlArea.getCentreX(), (float)controlArea.getY() + line.getLength() * 0.337f);
    g.drawLine(line, this->lineThickness);

    // labels
    font = Font(Font::getDefaultSansSerifFontName(), this->labelHeight, Font::plain);
    g.setFont(font);
    g.setColour(Colours::white);

    auto labelArea = Rectangle<int>(0, 0, font.getStringWidth(this->topLabel), this->labelHeight);

    labelArea.setX(controlArea.getCentreX() + this->lineThickness * 0.5f + padding);
    labelArea.setY(controlArea.getY());
    g.drawFittedText(this->topLabel, labelArea, Justification::centredLeft, 1);

    labelArea.setY(static_cast<int>(line.getEndY()) - this->labelHeight * 0.5f);
    g.drawFittedText(this->centreLabel, labelArea, Justification::centredLeft, 1);

    labelArea.setY(controlArea.getBottom() - this->labelHeight * 0.5f);
    g.drawFittedText(this->bottomLabel, labelArea, Justification::centredLeft, 1);

    // Button
    auto buttonArea = Rectangle<float>(0, 0, this->buttonDiameter, this->buttonDiameter);
    int sliderCentrePosition = controlArea.getBottom() + static_cast<int>(this->valueToProportionOfLength(this->getValue()) * static_cast<double>(controlArea.getY() - controlArea.getBottom()));

    buttonArea.setCentre(controlArea.getCentreX(), sliderCentrePosition);

    g.setColour(Colours::white);
    g.fillEllipse(buttonArea);
}

juce::String BigVerticalSlider::getDisplayText() const
{
    return this->m_param->getName(256);
}
