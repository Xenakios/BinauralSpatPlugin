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
class BinauralSpatAudioProcessorEditor  : public AudioProcessorEditor, public Timer
{
public:
    BinauralSpatAudioProcessorEditor (BinauralSpatAudioProcessor&);
    ~BinauralSpatAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
	void timerCallback() override;
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    BinauralSpatAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BinauralSpatAudioProcessorEditor)
};
