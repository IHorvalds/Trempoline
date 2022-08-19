#include "Tremolo.h"
#include "Utils.h"

Tremolo::Tremolo()
{
}

void Tremolo::prepare(double samplingRate, int samplesPerBlock)
{
    m_triangleOscillator.setFrequency(1.f);
    m_sineOscillator.setFrequency(1.f);

    m_triangleOscillator.initialise(Trempoline::TriangleWave);
    m_sineOscillator.initialise(Trempoline::SineWave);

    dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = 1;
    spec.sampleRate = samplingRate;
    m_triangleOscillator.prepare(spec);
    m_sineOscillator.prepare(spec);
}

float Tremolo::nextValue()
{
    m_triangleOscillator.setFrequency(this->m_frequency);
    m_sineOscillator.setFrequency(this->m_frequency);


    return 1.f + this->m_amplitude * ( this->m_functionShape * m_triangleOscillator.processSample(0) + (1.f - this->m_functionShape) * m_sineOscillator.processSample(0) );
}
