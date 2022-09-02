#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Parameters.h"

//==============================================================================
TrempolineProcessorEditor::TrempolineProcessorEditor (TrempolineProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p),
    speedSlider(processorRef.m_apvts.getParameter(SPEED_PARAM), juce::ImageCache::getFromMemory(assets::freq_stitched_png, assets::freq_stitched_pngSize), 260),
    depthSlider(processorRef.m_apvts.getParameter(DEPTH_PARAM), juce::ImageCache::getFromMemory(assets::depth_stitched_png, assets::depth_stitched_pngSize), 65),
    shapeSlider(processorRef.m_apvts.getParameter(SHAPE_PARAM), juce::ImageCache::getFromMemory(assets::shape_stitched_png, assets::shape_stitched_pngSize), 65),
    driveSlider(processorRef.m_apvts.getParameter(DRIVE_PARAM), juce::ImageCache::getFromMemory(assets::drive_stitched_png, assets::drive_stitched_pngSize), 65),
    makeupSlider(processorRef.m_apvts.getParameter(MAKEUP_PARAM), juce::ImageCache::getFromMemory(assets::volume_stitched_png, assets::volume_stitched_pngSize), 260),
    //bypassButton(),
    speedSliderAttachment(processorRef.m_apvts, SPEED_PARAM, speedSlider),
    depthSliderAttachment(processorRef.m_apvts, DEPTH_PARAM, depthSlider),
    shapeSliderAttachment(processorRef.m_apvts, SHAPE_PARAM, shapeSlider),
    driveSliderAttachment(processorRef.m_apvts, DRIVE_PARAM, driveSlider),
    makeupSliderAttachment(processorRef.m_apvts, MAKEUP_PARAM, makeupSlider)
    //bypassButtonAttachment(processorRef.m_apvts, BYPASS_PARAM, bypassButton)
{
    juce::ignoreUnused (processorRef);

    addAndMakeVisible(speedSlider);
    addAndMakeVisible(depthSlider);
    addAndMakeVisible(shapeSlider);
    addAndMakeVisible(driveSlider);
    addAndMakeVisible(makeupSlider);
    //addAndMakeVisible(bypassButton);

    //this->getLookAndFeel().setColour(juce::ResizableWindow::backgroundColourId, Colour(0x8F, 0x8F, 0x8F));
    
    
    setSize (848, 480);

    this->setResizable(true, true);
    this->setResizeLimits(848, 480, 1920, 1080);
    

    this->getConstrainer()->setFixedAspectRatio(16.0 / 9.0);
}

TrempolineProcessorEditor::~TrempolineProcessorEditor()
{
}

//==============================================================================
void TrempolineProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    juce::Image background = juce::ImageCache::getFromMemory(assets::big_ui_png, assets::big_ui_pngSize);

    auto bounds = getLocalBounds();
    g.drawImage(background, bounds.toFloat(), juce::RectanglePlacement::fillDestination);
}

void TrempolineProcessorEditor::resized()
{
    auto editorArea = this->getLocalBounds();

    const float resizeRatio = (float)editorArea.getWidth() / 1920.f;

    // Frequency button
    juce::Rectangle<int> freqArea(515 * resizeRatio, 521 * resizeRatio, 260 * resizeRatio, 260 * resizeRatio);
    speedSlider.setBounds(freqArea);

    // Shape button
    juce::Rectangle<int> shapeArea(809 * resizeRatio, 537 * resizeRatio, 65 * resizeRatio, 65 * resizeRatio);
    shapeSlider.setBounds(shapeArea);

    // Depth button
    juce::Rectangle<int> depthArea(936 * resizeRatio, 537 * resizeRatio, 65 * resizeRatio, 65 * resizeRatio);
    depthSlider.setBounds(depthArea);

    // Drive button
    juce::Rectangle<int> driveArea(1063 * resizeRatio, 537 * resizeRatio, 65 * resizeRatio, 65 * resizeRatio);
    driveSlider.setBounds(driveArea);

    // Volume button
    juce::Rectangle<int> volumeArea(1149 * resizeRatio, 521 * resizeRatio, 260 * resizeRatio, 260 * resizeRatio);
    makeupSlider.setBounds(volumeArea);
}
