#pragma once

#include <JuceHeader.h>

namespace Trempoline{
    inline float TriangleWave(float x)
    {
        return std::asin(jlimit(-1.f, 1.f, juce::dsp::FastMathApproximations::cos(x))) * (2.f / juce::MathConstants<float>::pi);
    }

    inline float SineWave(float x)
    {
        return jlimit(-1.f, 1.f, juce::dsp::FastMathApproximations::sin(x));
    }

    inline float SoftClipping(float x)
    {
        return (2.f / juce::MathConstants<float>::pi) * std::atan(x); // Maybe try another soft clipping function at some point.
    }
}