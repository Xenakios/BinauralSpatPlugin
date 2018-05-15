/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class BinauralSpatAudioProcessorEditor  : public AudioProcessorEditor, public Timer, public Value::Listener
{
public:
    BinauralSpatAudioProcessorEditor (BinauralSpatAudioProcessor&);
    ~BinauralSpatAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
	void timerCallback() override;
	void valueChanged(Value& value) override;
	void mouseDrag(const MouseEvent& ev) override;
	void mouseWheelMove(const MouseEvent& ev, const MouseWheelDetails& det) override;
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    BinauralSpatAudioProcessor& processor;
	Value m_xpos0;
	Value m_ypos0;
	Value m_zpos0;
	int m_valuechangecount = 0;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BinauralSpatAudioProcessorEditor)
};
