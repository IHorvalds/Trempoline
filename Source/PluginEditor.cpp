#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Parameters.h"

//==============================================================================
TrempolineProcessorEditor::TrempolineProcessorEditor (TrempolineProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p),
    speedSlider(processorRef.m_apvts.getParameter(SPEED_PARAM), juce::ImageCache::getFromMemory(assets::freq_stitched_png, assets::freq_stitched_pngSize), 140),
    depthSlider(processorRef.m_apvts.getParameter(DEPTH_PARAM), juce::ImageCache::getFromMemory(assets::depth_stitched_png, assets::depth_stitched_pngSize), 37),
    shapeSlider(processorRef.m_apvts.getParameter(SHAPE_PARAM), juce::ImageCache::getFromMemory(assets::shape_stitched_png, assets::shape_stitched_pngSize), 37),
    driveSlider(processorRef.m_apvts.getParameter(DRIVE_PARAM), juce::ImageCache::getFromMemory(assets::drive_stitched_png, assets::drive_stitched_pngSize), 37),
    makeupSlider(processorRef.m_apvts.getParameter(MAKEUP_PARAM), juce::ImageCache::getFromMemory(assets::volume_stitched_png, assets::volume_stitched_pngSize), 140),
    bypassButton(),
    speedSliderAttachment(processorRef.m_apvts, SPEED_PARAM, speedSlider),
    depthSliderAttachment(processorRef.m_apvts, DEPTH_PARAM, depthSlider),
    shapeSliderAttachment(processorRef.m_apvts, SHAPE_PARAM, shapeSlider),
    driveSliderAttachment(processorRef.m_apvts, DRIVE_PARAM, driveSlider),
    makeupSliderAttachment(processorRef.m_apvts, MAKEUP_PARAM, makeupSlider),
    bypassButtonAttachment(processorRef.m_apvts, BYPASS_PARAM, bypassButton)
{
    juce::ignoreUnused (processorRef);

    addAndMakeVisible(speedSlider);
    addAndMakeVisible(depthSlider);
    addAndMakeVisible(shapeSlider);
    addAndMakeVisible(driveSlider);
    addAndMakeVisible(makeupSlider);
    addAndMakeVisible(bypassButton);
    
    bypassButton.setImages(false, true, true, juce::ImageCache::getFromMemory(assets::button_on_cropped_png, assets::button_on_cropped_pngSize), 1.f, juce::Colours::transparentBlack, juce::Image(),
                           0.f, juce::Colours::transparentBlack, juce::ImageCache::getFromMemory(assets::button_off_cropped_png, assets::button_off_cropped_pngSize), 1.f, juce::Colours::transparentBlack);
    bypassButton.setToggleable(true);
    bypassButton.setClickingTogglesState(true);

    setSize (1024, 576);

    //this->setResizable(true, true);
    //this->setResizeLimits(848, 480, 1366, 768);
    

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

    //const float resizeRatio = (float)editorArea.getWidth() / 1024.f;
    constexpr float resizeRatio = 1.f;

    // Frequency button
    juce::Rectangle<int> freqArea(273.5 * resizeRatio, 277 * resizeRatio, 140 * resizeRatio, 140 * resizeRatio);
    speedSlider.setBounds(freqArea);

    // Shape button
    juce::Rectangle<int> shapeArea(430 * resizeRatio, 286 * resizeRatio, 37 * resizeRatio, 37 * resizeRatio);
    shapeSlider.setBounds(shapeArea);

    // Depth button
    juce::Rectangle<int> depthArea(498 * resizeRatio, 286 * resizeRatio, 37 * resizeRatio, 37 * resizeRatio);
    depthSlider.setBounds(depthArea);

    // Drive button
    juce::Rectangle<int> driveArea(566 * resizeRatio, 286 * resizeRatio, 37 * resizeRatio, 37 * resizeRatio);
    driveSlider.setBounds(driveArea);

    // Volume button
    juce::Rectangle<int> volumeArea(611 * resizeRatio, 277 * resizeRatio, 140 * resizeRatio, 140 * resizeRatio);
    makeupSlider.setBounds(volumeArea);


    juce::Rectangle<int> buttonArea(560 * resizeRatio, 323 * resizeRatio, 48 * resizeRatio, 86 * resizeRatio);
    bypassButton.setBounds(buttonArea);
}
