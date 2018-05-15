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
    : AudioProcessorEditor (&p), processor (p), m_xy(p)
{
	addAndMakeVisible(&m_xy);
	setResizable(true, true);
	setSize (400, 400);
	startTimer(20);
	
	
}

BinauralSpatAudioProcessorEditor::~BinauralSpatAudioProcessorEditor()
{
}

//==============================================================================
void BinauralSpatAudioProcessorEditor::paint (Graphics& g)
{
	g.fillAll(Colours::lightgrey);
}

void BinauralSpatAudioProcessorEditor::resized()
{
	m_xy.setBounds(0, 0, getWidth(), getHeight() - 20);
}

void BinauralSpatAudioProcessorEditor::timerCallback()
{
	//repaint();
}

XYPadComponent::XYPadComponent(BinauralSpatAudioProcessor & p) : m_processor(p)
{
	m_xpos0 = m_processor.m_state.getParameterAsValue("pos0x");
	m_xpos0.addListener(this);
	m_ypos0 = m_processor.m_state.getParameterAsValue("pos0y");
	m_ypos0.addListener(this);
	m_zpos0 = m_processor.m_state.getParameterAsValue("pos0z");
	m_zpos0.addListener(this);
}

void XYPadComponent::valueChanged(Value & value)
{
	++m_valuechangecount;
	repaint();
}

void XYPadComponent::mouseDrag(const MouseEvent & ev)
{
	float zsize = jmap<float>(m_zpos0.getValue(), -1.0, 1.0, 1.0, 50.0);
	m_xpos0 = jlimit<float>(-1.0, 1.0, jmap<float>(ev.x, 0.0, getWidth(), -1.0, 1.0));
	m_ypos0 = jlimit<float>(-1.0, 1.0, jmap<float>(ev.y, 0.0, getHeight(), -1.0, 1.0));
}

void XYPadComponent::mouseWheelMove(const MouseEvent & ev, const MouseWheelDetails & det)
{
	float curval = m_zpos0.getValue();
	if (det.deltaY < 0.0)
		curval -= 0.05f;
	if (det.deltaY > 0.0)
		curval += 0.05f;
	m_zpos0 = jlimit(-1.0f, 1.0f, curval);
}

void XYPadComponent::paint(Graphics & g)
{
	g.fillAll(Colours::black);
	g.setColour(Colours::white);
	float xcor = jmap<float>(m_xpos0.getValue(), -1.0, 1.0, 0.0, getWidth());
	float ycor = jmap<float>(m_ypos0.getValue(), -1.0, 1.0, 0.0, getHeight());
	float zsize = jmap<float>(m_zpos0.getValue(), -1.0, 1.0, 2.0, 50.0);
	g.fillEllipse(xcor - zsize / 2.0, ycor - zsize / 2.0, zsize, zsize);
	g.drawText(String(m_valuechangecount), 10, 10, 100, 50, Justification::centred);
}
