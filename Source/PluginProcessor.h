/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "phonon.h"

template<typename Cont, typename T>
inline void fill_container(Cont& c, const T& x)
{
	std::fill(std::begin(c), std::end(c), x);
}


template<typename T>
class CircularBuffer final
{
public:
	CircularBuffer(int size)
	{
		m_buf.resize(size);
	}
	void clear()
	{
		m_avail = 0;
		m_readpos = 0;
		m_writepos = 0;
		fill_container(m_buf, T());
	}
	void push(T x)
	{
		m_buf[m_writepos] = x;
		++m_writepos;
		++m_avail;
		if (m_writepos >= m_buf.size())
			m_writepos = 0;
	}
	T get()
	{
		jassert(m_avail > 0);
		T x = m_buf[m_readpos];
		++m_readpos;
		--m_avail;
		if (m_readpos >= m_buf.size())
			m_readpos = 0;
		return x;
	}
	int available() { return m_avail; }
	int getToBuf(T* buf, int len)
	{
		jassert(m_avail > 0);
		if (len > m_avail)
			len = m_avail;
		for (int i = 0; i < len; ++i)
			buf[i] = get();
		return len;
	}
	int getFromBuf(T* buf, int len)
	{
		for (int i = 0; i < len; ++i)
			push(buf[i]);
		return len;
	}
	int getSize() { return (int)m_buf.size(); }
	void resize(int size)
	{
		m_avail = 0;
		m_readpos = 0;
		m_writepos = 0;
		m_buf.resize(size);
	}
private:
	int m_writepos = 0;
	int m_readpos = 0;
	int m_avail = 0;
	std::vector<T> m_buf;
};



//==============================================================================
/**
*/
class BinauralSpatAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    BinauralSpatAudioProcessor();
    ~BinauralSpatAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
	IPLhandle m_sacontext{ nullptr };
	IPLRenderingSettings m_sasettings;
	IPLhandle m_sarenderer{ nullptr };
	IPLhandle m_saeffect{ nullptr };
	IPLHrtfParams m_sahrtfParams{ IPL_HRTFDATABASETYPE_DEFAULT, nullptr, 0, nullptr, nullptr, nullptr };
	IPLAudioFormat m_input_format;
	IPLAudioFormat m_output_format;
	std::vector<float> m_procinbuf;
	std::vector<float> m_procoutbuf;
	CircularBuffer<float> m_cb{ 16384 };
	CircularBuffer<float> m_cbout{ 16384 };
	int m_procgran = 128;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BinauralSpatAudioProcessor)
};
