#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Parameters.h"

//==============================================================================
TrempolineProcessorEditor::TrempolineProcessorEditor (TrempolineProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p),
    speedSlider(processorRef.m_apvts.getParameter(SPEED_PARAM), CENTER),
    depthSlider(processorRef.m_apvts.getParameter(DEPTH_PARAM), TOP_POSITION),
    shapeSlider(processorRef.m_apvts.getParameter(SHAPE_PARAM), TOP_POSITION),
    driveSlider(processorRef.m_apvts.getParameter(DRIVE_PARAM), TOP_POSITION),
    makeupSlider(processorRef.m_apvts.getParameter(MAKEUP_PARAM)),
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

    lnf.setColour(juce::ResizableWindow::backgroundColourId, Colour(0x8F, 0x8F, 0x8F));
    this->setLookAndFeel(&lnf);
    
    
    setSize (541, 289);

    this->setResizable(true, true);
    this->setResizeLimits(541, 289, 1623, 867);
    

    this->getConstrainer()->setFixedAspectRatio(534.0 / 289.0); // Note: Not sure if it works?? Test this.
}

TrempolineProcessorEditor::~TrempolineProcessorEditor()
{
    this->setLookAndFeel(nullptr);
}

//==============================================================================
void TrempolineProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    auto controlsArea = getLocalBounds();
    g.setColour(Colours::aliceblue);

    controlsArea.reduce(28, 22);


    constexpr float controlsToTitleRatio = 238.f / 289.f;

    // Title area
    auto titleArea = controlsArea.removeFromBottom(static_cast<int>(controlsArea.getHeight() * (1.f - controlsToTitleRatio)));

    auto leftPoint = titleArea.getTopLeft();
    auto rightPoint = titleArea.getTopRight();

    leftPoint.addXY(0, this->getHeight() * 0.0467f);
    rightPoint.addXY(0, this->getHeight() * 0.0467f);
    Line<float> separator(leftPoint.toFloat(), rightPoint.toFloat());

    g.setColour(Colour(0x51, 0x51, 0x51));
    g.drawLine(separator, 1);

    juce::String title = "Trempoline";
    float titleHeight = this->getHeight() * 0.1107f;
    Font font(Font::getDefaultSansSerifFontName(), titleHeight, Font::bold);
    

    auto textArea = Rectangle<int>(titleArea.getPosition().x, titleArea.getPosition().y + this->getHeight() * 0.0467f, font.getStringWidth(title), static_cast<int>(titleHeight));
    g.setFont(font);
    g.drawFittedText(title, textArea, Justification::centredLeft, 1);

    juce::String subtitle = "just a tiny tremolo";
    float subtitleHeight = this->getHeight() * 0.0692f;
    font = Font(Font::getDefaultSansSerifFontName(), subtitleHeight, Font::bold);
    
    auto subtitleArea = Rectangle<int>(0, 0, font.getStringWidth(subtitle), subtitleHeight);
    subtitleArea.setPosition(titleArea.getRight() - subtitleArea.getWidth(), textArea.getBottom() - static_cast<int>(subtitleHeight));
    g.setFont(font);
    g.drawFittedText(subtitle, subtitleArea, Justification::centredLeft, 1);
}

void TrempolineProcessorEditor::resized()
{
    auto controlsArea = getLocalBounds();

    controlsArea.reduce(28, 22);

    constexpr float controlsToTitleRatio = 238.f / 289.f;

    // Title area
    auto titleArea = controlsArea.removeFromBottom(static_cast<int>(controlsArea.getHeight() * (1.f - controlsToTitleRatio)));

    // Controls Area
    controlsArea.removeFromBottom(controlsArea.getHeight() * 0.0688); // padding

    speedSlider.lineThickness = this->getHeight() * 0.0138f;
    speedSlider.buttonDiameter = this->getHeight() * 0.0484f;
    speedSlider.textHeight = static_cast<int>(this->getHeight() * 0.1107f);
    speedSlider.labelHeight = static_cast<int>(this->getHeight() * 0.0553f);
    speedSlider.leftLabel = { std::make_unique<String>("0.1 Hz"), nullptr };
    speedSlider.rightLabel = { std::make_unique<String>("10 Hz"), nullptr };
    speedSlider.setBounds(controlsArea.removeFromLeft(controlsArea.getWidth() * 0.3974f).toNearestInt());

    controlsArea.removeFromLeft(getWidth() * 0.0462f);

    // clusterArea is the area with 3 controls (shape, depth, drive)
    auto clusterArea = controlsArea.removeFromLeft(controlsArea.getHeight());
    auto topRow = clusterArea.removeFromTop(clusterArea.getHeight() * 0.5f);
    auto bottomRow = clusterArea;

    shapeSlider.lineThickness = this->getHeight() * 0.0138f;
    shapeSlider.buttonDiameter = this->getHeight() * 0.0242f;
    shapeSlider.textHeight = static_cast<int>(this->getHeight() * 0.0484f);
    shapeSlider.labelHeight = static_cast<int>(this->getHeight() * 0.0657f);

    Image triangle = ImageCache::getFromMemory(assets::triangle_png, assets::triangle_pngSize);
    Image smallSine = ImageCache::getFromMemory(assets::small_sine_png, assets::small_sine_pngSize);
    Image bigSine = ImageCache::getFromMemory(assets::big_sine_png, assets::big_sine_pngSize);

    shapeSlider.leftLabel = { nullptr, std::make_unique<Image>(triangle)};
    shapeSlider.rightLabel = { nullptr, std::make_unique<Image>(bigSine)};
    shapeSlider.setBounds(topRow.removeFromLeft(topRow.getWidth() / 2));


    depthSlider.lineThickness = this->getHeight() * 0.0138f;
    depthSlider.buttonDiameter = this->getHeight() * 0.0242f;
    depthSlider.textHeight = static_cast<int>(this->getHeight() * 0.0484f);
    depthSlider.labelHeight = static_cast<int>(this->getHeight() * 0.06f);
    depthSlider.leftLabel = { nullptr, std::make_unique<Image>(smallSine) };
    depthSlider.rightLabel = { nullptr, std::make_unique<Image>(bigSine) };
    depthSlider.setBounds(topRow);

    driveSlider.lineThickness = this->getHeight() * 0.0138f;
    driveSlider.buttonDiameter = this->getHeight() * 0.0242f;
    driveSlider.textHeight = static_cast<int>(this->getHeight() * 0.0484f);
    driveSlider.labelHeight = static_cast<int>(this->getHeight() * 0.0657f);
    driveSlider.leftLabel = { std::make_unique<String>("1"), nullptr };
    driveSlider.rightLabel = { std::make_unique<String>("3"), nullptr };
    driveSlider.setBounds(bottomRow.removeFromLeft(bottomRow.getWidth() * 0.5f));

    float titleHeight = this->getHeight() * 0.1107f;
    auto bypassButtonArea = Rectangle<int>(0, 0, (float)titleHeight, (float)titleHeight);
    bypassButtonArea.setCentre(titleArea.getCentreX(), 0);
    bypassButtonArea.setY(titleArea.getPosition().y + this->getHeight() * 0.0834f);
    bypassButton.setBounds(bypassButtonArea);

    makeupSlider.lineThickness = this->getHeight() * 0.0138f;;
    makeupSlider.buttonDiameter = this->getHeight() * 0.0484f;
    makeupSlider.textHeight = static_cast<int>(this->getHeight() * 0.0553f);
    makeupSlider.labelHeight = static_cast<int>(this->getHeight() * 0.0484f);
    makeupSlider.topLabel = "6 dB";
    makeupSlider.centreLabel = "0 dB";
    makeupSlider.bottomLabel = "-inf";
    makeupSlider.setBounds(controlsArea);


}
