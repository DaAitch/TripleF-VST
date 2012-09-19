
#ifndef __triplefprocessor_h__included__
#	define __triplefprocessor_h__included__

#include <public.sdk/source/vst/vstaudioeffect.h>
#include <pluginterfaces/vst/ivstevents.h>
#include <pluginterfaces/vst/ivstparameterchanges.h>
#include <pluginterfaces/base/ustring.h>
#include <pluginterfaces/base/ibstream.h>

#include "fffinit.h"
#include "vsttools.h"

using namespace Steinberg;
using namespace Steinberg::Vst;
using namespace fff::Computing;
using namespace fff::Computing::Kernel;
using namespace fff::Buffer::Complex::Host;

namespace triplef
{

    typedef fff::Factory::UbiMultiChannelFactory<Sample> mcf;

	class TripleFProcessor
		: public AudioEffect
	{
	public:
		TripleFProcessor();
		~TripleFProcessor();

		static FUID uid;

		static FUnknown *createInstance(
			void *)
		{
			return (IAudioProcessor *)new TripleFProcessor;
		}

		tresult PLUGIN_API initialize(FUnknown *contex);
		tresult PLUGIN_API terminate();

		
		tresult PLUGIN_API process(ProcessData &data);
		tresult PLUGIN_API setupProcessing(ProcessSetup &setup);
		tresult PLUGIN_API setActive(TBool state);
		tresult PLUGIN_API setBusArrangements(SpeakerArrangement *inputs, int32 numIns, SpeakerArrangement *outputs, int32 numOuts);
		tresult PLUGIN_API setState(IBStream *state);
		tresult PLUGIN_API getState(IBStream *state);

		


	private:

		TripleFProcessor(
			const TripleFProcessor &)
		{
		}

		TripleFProcessor &operator=(
			const TripleFProcessor &)
		{
		}

        Compiler *m_compiler;

        mcf::multichannel
            m_H;

        UInt m_ssize;

        mcf::hmcp
            *m_x,
            *m_y;

        FastConvolution<Sample>
            *m_fconv;

        OpenCLEnvironment
            *m_env;
	};

	

}

#endif