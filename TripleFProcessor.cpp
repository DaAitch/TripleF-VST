



#include "TripleFProcessor.h"

using namespace fff::Computing;
using namespace fff::Filesystem;

namespace triplef
{
	FUID TripleFProcessor::uid(0x84923C8D,0x7FD04829,0xACDEDAE8,0x0A324600);

	TripleFProcessor::TripleFProcessor()
		:
        m_compiler(NULL),
        m_fconv(NULL),
        m_env(NULL),
        m_x(NULL),
        m_y(NULL),
        m_ssize(0)
	{
	}

	TripleFProcessor::~TripleFProcessor()
	{
	}

	tresult PLUGIN_API TripleFProcessor::initialize(FUnknown *context)
	{
		tresult initbase = AudioEffect::initialize(context);

		if(initbase == kResultTrue)
		{
			if(removeAllBusses() != kResultTrue)
				return kResultFalse;

            addAudioInput (USTRING("Stereo In"), SpeakerArr::kStereo);
            addAudioOutput (USTRING("Stereo out"), SpeakerArr::kStereo);

            try
            {
                m_env = new OpenCLEnvironment(CL_DEVICE_TYPE_GPU);
                m_compiler = new Compiler(*m_env, path("%FFF%/cl").getPath());
                m_compiler->build();

                WaveReader<Sample> hReader("h.wav");

                UInt channelCount = hReader.getChannelCount();
                UInt sampleCount = hReader.getSampleCount();

                m_ssize = sampleCount - 1;


                ComputingData<Sample> cd(sampleCount);
                DeviceProperties devprops(m_env->getDevice());
                Mapper map(cd, devprops, 0);

                m_x = new mcf::hmcp;
                m_y = new mcf::hmcp;

                mcf::multichannel b, a;
                
                mcf::create(b, *m_env, CL_MEM_READ_ONLY, channelCount, fff_POW2(map.getLb2N()));

                mcf::create(a, *m_env, CL_MEM_READ_ONLY, channelCount, fff_POW2(map.getLb2N()));

                for(UInt ch = 0; ch < channelCount; ++ch)
                    a.host->getRawReal(ch)[0] = (Sample)20.f;


                mcf::create(m_H, *m_env, CL_MEM_READ_WRITE, channelCount, fff_POW2(map.getLb2N()));

                hReader.readFile(*(b.host));


                TransferFunction<Sample> tra(*m_compiler, *b, *a, *m_H);
                tra.invokeAndWait();
            }
            catch(...)
            {
                initbase = kResultFalse;
            }
            
		}

		return
			initbase;

	}

	tresult PLUGIN_API TripleFProcessor::terminate()
	{
        SafeDelete(m_fconv);
        SafeDelete(m_x);
        SafeDelete(m_y);
        SafeDelete(m_compiler);
        SafeDelete(m_env);

        m_ssize = 0;

		return
			AudioEffect::terminate();
	}

	tresult PLUGIN_API TripleFProcessor::setActive(TBool state)
	{
		return
			AudioEffect::setActive(state);
	}

	tresult PLUGIN_API TripleFProcessor::process(ProcessData &data)
	{
        if(data.numInputs!=1 || data.numOutputs!=1)
            return kResultFalse;

        if(data.numSamples==0)
            return kResultOk;

        if(data.inputs[0].numChannels != m_H.host->getChannelCount())
            return kResultFalse;

        try
        {
            if(m_fconv && data.numSamples > 0)
            {
                
                Sample **in = data.inputs[0].channelBuffers32;
                Sample **out = data.outputs[0].channelBuffers32;

                m_x->setPointer(in);
                m_y->setPointer(out);
                
                m_fconv->invoke(data.numSamples, *m_x, *m_y);
            }
        }
        catch(...)
        {
            return kResultFalse;
        }
        
		return
			kResultOk;
	}

	tresult PLUGIN_API TripleFProcessor::setupProcessing(ProcessSetup &setup)
	{
        try
        {
            m_x->alloc(m_H.host->getChannelCount(), setup.maxSamplesPerBlock);
            m_y->alloc(m_H.host->getChannelCount(), setup.maxSamplesPerBlock);

            SafeDelete(m_fconv);

            m_fconv = new FastConvolution<Sample>(
                *m_compiler,
                m_ssize,
                m_x->getSampleCount(),
                *m_H,
                0);

            
        }
        catch(...)
        {
            return kResultFalse;
        }

		return
			AudioEffect::setupProcessing(setup);
	}

	tresult PLUGIN_API TripleFProcessor::setBusArrangements(SpeakerArrangement* inputs, int32 numIns, SpeakerArrangement* outputs, int32 numOuts)
	{
		if(numIns != 1 || numOuts != 1)
			return kResultFalse;

        if(SpeakerArr::getChannelCount(inputs[0]) != 2 ||
            SpeakerArr::getChannelCount(outputs[0]) != 2)
            return kResultFalse;

        
		return
			AudioEffect::setBusArrangements(
			    inputs,
			    numIns,
			    outputs,
			    numOuts);
	}

	tresult PLUGIN_API TripleFProcessor::setState(IBStream *state)
	{
		return kResultTrue;
	}

	tresult PLUGIN_API TripleFProcessor::getState(IBStream *state)
	{
		return kResultTrue;
	}

}
