/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

IPLerror (*_iplCreateContext)(IPLLogFunction logCallback,
	IPLAllocateFunction allocateCallback,
	IPLFreeFunction freeCallback,
	IPLhandle* context);
IPLvoid (*_iplDestroyContext)(IPLhandle* context);
IPLerror (*_iplCreateBinauralRenderer)(IPLhandle context, IPLRenderingSettings renderingSettings,
	IPLHrtfParams params, IPLhandle* renderer);
IPLerror (*_iplCreateBinauralEffect)(IPLhandle renderer, IPLAudioFormat inputFormat,
	IPLAudioFormat outputFormat, IPLhandle* effect);
IPLvoid (*_iplDestroyBinauralEffect)(IPLhandle* effect);
IPLvoid (*_iplDestroyBinauralRenderer)(IPLhandle* renderer);
IPLvoid (*_iplApplyBinauralEffect)(IPLhandle effect, IPLAudioBuffer inputAudio, IPLVector3 direction,
	IPLHrtfInterpolation interpolation, IPLAudioBuffer outputAudio);

String getPluginDllDirectoryPath()
{
	return File::getSpecialLocation(File::SpecialLocationType::currentExecutableFile).getParentDirectory().getFullPathName();
}

//==============================================================================
BinauralSpatAudioProcessor::BinauralSpatAudioProcessor()
{
	if (m_steamdll->getNativeHandle() == nullptr)
	{
		String dllfn = getPluginDllDirectoryPath() + "/phonon.dll";
		m_steamdll->open(dllfn);
		m_steamdll->initFunctions("iplCreateContext", _iplCreateContext, "iplCreateBinauralRenderer", _iplCreateBinauralRenderer,
			"iplCreateBinauralEffect",_iplCreateBinauralEffect,"iplDestroyContext",_iplDestroyContext,
			"iplDestroyBinauralEffect",_iplDestroyBinauralEffect,"iplDestroyBinauralRenderer",_iplDestroyBinauralRenderer,
			"iplApplyBinauralEffect",_iplApplyBinauralEffect);
	}
	_iplCreateContext(nullptr, nullptr, nullptr, &m_sacontext);
	memset(&m_input_format, 0, sizeof(IPLAudioFormat));
	memset(&m_output_format, 0, sizeof(IPLAudioFormat));
	m_input_format.channelLayoutType = IPL_CHANNELLAYOUTTYPE_SPEAKERS;
	m_input_format.channelLayout = IPL_CHANNELLAYOUT_MONO;
	m_input_format.channelOrder = IPL_CHANNELORDER_INTERLEAVED;
	m_output_format.channelLayoutType = IPL_CHANNELLAYOUTTYPE_SPEAKERS;
	m_output_format.channelLayout = IPL_CHANNELLAYOUT_STEREO;
	m_output_format.channelOrder = IPL_CHANNELORDER_INTERLEAVED;
	m_par_x = new AudioParameterFloat("x", "X pos", -1.0, 1.0, 0.0);
	addParameter(m_par_x);
	m_par_y = new AudioParameterFloat("y", "Y pos", -1.0, 1.0, 0.0);
	addParameter(m_par_y);
	m_par_z = new AudioParameterFloat("z", "Z pos", -1.0, 1.0, 0.0);
	addParameter(m_par_z);
}

BinauralSpatAudioProcessor::~BinauralSpatAudioProcessor()
{
	_iplDestroyContext(&m_sacontext);
}

//==============================================================================
const String BinauralSpatAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BinauralSpatAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BinauralSpatAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BinauralSpatAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BinauralSpatAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BinauralSpatAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BinauralSpatAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BinauralSpatAudioProcessor::setCurrentProgram (int index)
{
}

const String BinauralSpatAudioProcessor::getProgramName (int index)
{
    return {};
}

void BinauralSpatAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void BinauralSpatAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	ScopedLock locker(m_cs);
	m_sasettings = { (int)sampleRate,m_procgran };
	_iplCreateBinauralRenderer(m_sacontext, m_sasettings, m_sahrtfParams, &m_sarenderer);
	_iplCreateBinauralEffect(m_sarenderer, m_input_format, m_output_format, &m_saeffect);
	m_cb.clear();
	m_cbout.clear();
	m_procinbuf.resize(samplesPerBlock);
	m_procoutbuf.resize(samplesPerBlock*2);
}

void BinauralSpatAudioProcessor::releaseResources()
{
	_iplDestroyBinauralEffect(&m_saeffect);
	_iplDestroyBinauralRenderer(&m_sarenderer);
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BinauralSpatAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void BinauralSpatAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	ScopedLock locker(m_cs);
	ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
	for (int i = 0; i < buffer.getNumSamples(); ++i)
		m_cb.push(buffer.getSample(0, i));
	if (m_cb.available() >= buffer.getNumSamples()+m_procgran)
	{
		while (m_cbout.available() < 2*buffer.getNumSamples())
		{
			m_audio_cs->enter();
			for (int i = 0; i < m_procgran; ++i)
				m_procinbuf[i] = m_cb.get();
			IPLAudioBuffer inbuffer{ m_input_format, m_procgran, m_procinbuf.data() };
			IPLAudioBuffer outbuffer{ m_output_format, m_procgran, m_procoutbuf.data() };
			IPLVector3 posvec{ *m_par_x,*m_par_y,*m_par_z };
			_iplApplyBinauralEffect(m_saeffect,
				inbuffer,
				posvec,
				IPL_HRTFINTERPOLATION_BILINEAR,
				outbuffer);
			inbuffer.interleavedBuffer += m_procgran;

			for (int i = 0; i < m_procgran * 2; ++i)
			{
				m_cbout.push(m_procoutbuf[i]);
			}
			m_audio_cs->exit();
		}
	}
	if (m_cbout.available() >= 2*buffer.getNumSamples())
	{
		for (int i = 0; i < buffer.getNumSamples(); ++i)
		{
			buffer.setSample(0, i, m_cbout.get());
			buffer.setSample(1, i, m_cbout.get());
		}
	}

}

//==============================================================================
bool BinauralSpatAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* BinauralSpatAudioProcessor::createEditor()
{
    return new BinauralSpatAudioProcessorEditor (*this);
}

//==============================================================================
void BinauralSpatAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void BinauralSpatAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void BinauralSpatAudioProcessor::ringBufferInfo(int & inavail, int & outavail)
{
	inavail = m_cb.available();
	outavail = m_cbout.available();
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BinauralSpatAudioProcessor();
}
