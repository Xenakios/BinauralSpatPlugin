/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BinauralSpatAudioProcessorEditor::BinauralSpatAudioProcessorEditor (BinauralSpatAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
	startTimer(20);
}

BinauralSpatAudioProcessorEditor::~BinauralSpatAudioProcessorEditor()
{
}

//==============================================================================
void BinauralSpatAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
	int inavail;
	int outavail;
	processor.ringBufferInfo(inavail, outavail);
	float pos0x = *processor.m_state.getRawParameterValue("pos0x");
	g.drawFittedText (String(pos0x,2)+" "+String(inavail)+" "+String(outavail), getLocalBounds(), Justification::centred, 1);
}

void BinauralSpatAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void BinauralSpatAudioProcessorEditor::timerCallback()
{
	repaint();
}
