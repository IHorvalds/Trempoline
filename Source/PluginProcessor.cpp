#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Parameters.h"
#include "DSP/Utils.h"

//==============================================================================
TrempolineProcessor::TrempolineProcessor()
     : juce::AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
{
    m_waveShaper.functionToUse = Trempoline::SoftClipping;
}

TrempolineProcessor::~TrempolineProcessor()
{
}

//==============================================================================
const juce::String TrempolineProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TrempolineProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TrempolineProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TrempolineProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TrempolineProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TrempolineProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TrempolineProcessor::getCurrentProgram()
{
    return 0;
}

void TrempolineProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String TrempolineProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void TrempolineProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void TrempolineProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = 1;
    spec.sampleRate = sampleRate;

    m_tremolo.prepare(sampleRate, samplesPerBlock);

    m_waveShaper.prepare(spec);
    m_makeupProcessor.prepare(spec);

    m_smoothedDepth.reset(sampleRate, 0.002f);
    m_smoothedShape.reset(sampleRate, 0.003f);
    m_smoothedDrive.reset(sampleRate, 0.002f);
    m_smoothedMakeupGain.reset(sampleRate, 0.002f);
}

void TrempolineProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool TrempolineProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}

void TrempolineProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);

    juce::ScopedNoDenormals noDenormals;

    // update smoothed params
    float depth = m_apvts.getParameterAsValue(DEPTH_PARAM).getValue();
    float shape = m_apvts.getParameterAsValue(SHAPE_PARAM).getValue();
    float drive = m_apvts.getParameterAsValue(DRIVE_PARAM).getValue();
    float makeupGain = m_apvts.getParameterAsValue(MAKEUP_PARAM).getValue();

    m_smoothedDepth.setTargetValue(depth);
    m_smoothedShape.setTargetValue(shape);
    m_smoothedDrive.setTargetValue(drive);
    m_smoothedMakeupGain.setTargetValue(makeupGain);

    // is it bypassed?
    bool bypassed = m_apvts.getParameterAsValue(BYPASS_PARAM).getValue();
    if (!bypassed)
    {
        m_makeupProcessor.setGainDecibels(m_smoothedMakeupGain.getNextValue());
    
        // update tremolo params
        float frequency = m_apvts.getParameterAsValue(SPEED_PARAM).getValue();
        m_tremolo.updateParams(frequency, m_smoothedDepth.getNextValue(), m_smoothedShape.getNextValue());

        float **inputs = buffer.getArrayOfWritePointers();
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            float tremoloValue = m_tremolo.nextValue();

            for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
            {
                inputs[channel][sample] *= tremoloValue;

                inputs[channel][sample] += m_waveShaper.processSample(inputs[channel][sample] * m_smoothedDrive.getNextValue());
                inputs[channel][sample] *= 0.5f;

                inputs[channel][sample] = m_makeupProcessor.processSample(inputs[channel][sample]);
            }
        }
    }
}

//==============================================================================
bool TrempolineProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* TrempolineProcessor::createEditor()
{
    return new TrempolineProcessorEditor (*this);
}

//==============================================================================
void TrempolineProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream mos(destData, true);
    m_apvts.state.writeToStream(mos);
}

void TrempolineProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData(data, sizeInBytes);

    if (tree.isValid()) {
        m_apvts.replaceState(tree);
        return;
    }
}

APVTS::ParameterLayout TrempolineProcessor::createParameterLayout()
{
    APVTS::ParameterLayout layout = APVTS::ParameterLayout();

    layout.add(std::make_unique<juce::AudioParameterBool>(BYPASS_PARAM, BYPASS_PARAM, false));

    layout.add(std::make_unique<juce::AudioParameterFloat>(SPEED_PARAM, SPEED_PARAM, juce::NormalisableRange<float>(0.1f, 10.f, 0.05f), 0.5f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(DEPTH_PARAM, DEPTH_PARAM, juce::NormalisableRange<float>(0.1f, 1.f, 0.01f), 0.4f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(SHAPE_PARAM, SHAPE_PARAM, juce::NormalisableRange<float>(0.f, 1.f, 0.01f), 0.99f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(DRIVE_PARAM, DRIVE_PARAM, juce::NormalisableRange<float>(0.1f, 30.f, 0.01f), 1.f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(MAKEUP_PARAM, MAKEUP_PARAM, juce::NormalisableRange<float>(-50.f, 6.f, 0.01f, 6.5f), 0.f));

    return layout;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TrempolineProcessor();
}
