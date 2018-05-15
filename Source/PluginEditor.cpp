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
	m_xpos0 = processor.m_state.getParameterAsValue("pos0x");
	m_xpos0.addListener(this);
	m_ypos0 = processor.m_state.getParameterAsValue("pos0y");
	m_ypos0.addListener(this);
	m_zpos0 = processor.m_state.getParameterAsValue("pos0z");
	m_zpos0.addListener(this);

}

BinauralSpatAudioProcessorEditor::~BinauralSpatAudioProcessorEditor()
{
}

//==============================================================================
void BinauralSpatAudioProcessorEditor::paint (Graphics& g)
{
	g.fillAll(Colours::black);
	g.setColour(Colours::white);
	float xcor = jmap<float>(m_xpos0.getValue(), -1.0, 1.0, 0.0, getWidth());
	float ycor = jmap<float>(m_ypos0.getValue(), -1.0, 1.0, 0.0, getHeight());
	float zsize = jmap<float>(m_zpos0.getValue(), -1.0, 1.0, 1.0, 50.0);
	g.fillEllipse(xcor-zsize/2.0, ycor-zsize/2.0, zsize, zsize);
	g.drawText(String(m_valuechangecount), 10, 10, 100, 50, Justification::centred);
}

void BinauralSpatAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void BinauralSpatAudioProcessorEditor::timerCallback()
{
	//repaint();
}

void BinauralSpatAudioProcessorEditor::valueChanged(Value & value)
{
	++m_valuechangecount;
	repaint();
}

void BinauralSpatAudioProcessorEditor::mouseDrag(const MouseEvent & ev)
{
	float zsize = jmap<float>(m_zpos0.getValue(), -1.0, 1.0, 1.0, 50.0);
	m_xpos0 = jlimit<float>(-1.0,1.0, jmap<float>(ev.x, 0.0, getWidth(), -1.0, 1.0));
	m_ypos0 = jlimit<float>(-1.0,1.0, jmap<float>(ev.y, 0.0, getHeight(), -1.0, 1.0));
}

void BinauralSpatAudioProcessorEditor::mouseWheelMove(const MouseEvent & ev, const MouseWheelDetails & det)
{
	float curval = m_zpos0.getValue();
	if (det.deltaY < 0.0)
		curval -= 0.05f;
	if (det.deltaY > 0.0)
		curval += 0.05f;
	m_zpos0 = jlimit(-1.0f, 1.0f, curval);
}
