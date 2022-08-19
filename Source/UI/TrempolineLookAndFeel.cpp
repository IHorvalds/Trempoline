#include "TrempolineLookAndFeel.h"
#include "BigRotarySlider.h"

//void TrempolineLookAndFeel::drawLinearSlider(Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle style, Slider& slider)
//{
//    
//}

//void TrempolineLookAndFeel::drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle, Slider& slider)
//{
    /*ignoreUnused(slider);
    int rectSide = std::min(width, height);
    Rectangle<float> sliderArea(0, 0, rectSide, rectSide);

    sliderArea.setCentre(static_cast<float>(x) + width * .5f, static_cast<float>(y) + height * .5f);
    g.setColour(Colours::white);*/
    

    //float lineThickness = 4.f;
    //float diameter = 14.f;
    //try
    //{
    //    BigRotarySlider& brs = dynamic_cast<BigRotarySlider&>(slider);
    //    lineThickness = brs.lineThickness;
    //    diameter = brs.buttonDiameter;
    //} catch (const std::bad_cast& e) { }

    //sliderArea.reduce(lineThickness * .5f, lineThickness * .5f);
    //g.drawEllipse(sliderArea, lineThickness);

    //// button
    //float sliderAngle =  rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);
    //
    //Rectangle<float> button(sliderArea.getCentreX() - diameter * 0.5f, sliderArea.getTopLeft().y + rectSide * 0.05f, diameter, diameter);
    //Path p;
    //p.addEllipse(button);

    //p.applyTransform(AffineTransform().rotated(sliderAngle, sliderArea.getCentreX(), sliderArea.getCentreY()));
    //
    //g.fillPath(p);
//}
