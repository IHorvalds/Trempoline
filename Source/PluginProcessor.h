#pragma once

#include <JuceHeader.h>

#include "DSP/Tremolo.h"

using APVTS = juce::AudioProcessorValueTreeState;

//==============================================================================
class TrempolineProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    TrempolineProcessor();
    ~TrempolineProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;


    APVTS::ParameterLayout createParameterLayout();
    
    APVTS m_apvts {*this, nullptr, "Trempoline Params", createParameterLayout()};

private:

    Tremolo m_tremolo;

    juce::dsp::WaveShaper<float> m_waveShaper;
    juce::dsp::Gain<float> m_makeupProcessor;

    juce::SmoothedValue<float> m_smoothedDepth;
    juce::SmoothedValue<float> m_smoothedShape;
    juce::SmoothedValue<float> m_smoothedDrive;
    juce::SmoothedValue<float> m_smoothedMakeupGain;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TrempolineProcessor)
};
