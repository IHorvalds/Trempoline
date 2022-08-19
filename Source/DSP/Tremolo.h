#pragma once

#include <JuceHeader.h>

class Tremolo
{
public:
    Tremolo();
    ~Tremolo() = default;

    void prepare(double samplingRate, int samplesPerBlock);
    float nextValue();

    inline void updateParams(float frequency, float amplitude, float shape) {
        this->m_frequency = frequency;
        this->m_amplitude = amplitude;
        this->m_functionShape = shape;
    }
private:
    juce::dsp::Oscillator<float> m_triangleOscillator;
    juce::dsp::Oscillator<float> m_sineOscillator;

    float m_frequency = 1.f;
    float m_amplitude = 1.f;
    /// <summary>
    /// Shape belongs to [0, 1]
    /// output = (triangle(x) * shape + sine(x) * (1-shape)) * m_amplitude
    /// </summary>
    float m_functionShape = 0.f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Tremolo)
};