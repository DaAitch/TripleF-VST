
#include <public.sdk/source/main/pluginfactoryvst3.h>
#include <pluginterfaces/vst/ivstcomponent.h>
#include <pluginterfaces/vst/ivstaudioprocessor.h>
#include <pluginterfaces/vst/ivsteditcontroller.h>
#include <pluginterfaces/base/ftypes.h>

#include "TripleFProcessor.h"

#include "version.h"

using namespace Steinberg::Vst;



bool InitModule () { return true; }
bool DeinitModule () { return true; }


BEGIN_FACTORY_DEF(
	"Virtual Company",
	"http://www.froq.us/",
	"admin@froq.us")

	DEF_CLASS2(INLINE_UID_FROM_FUID(triplef::TripleFProcessor::uid),
		PClassInfo::kManyInstances,
		kVstAudioEffectClass,
		PLUGIN_DESCRIPTION " Processor",
		Vst::kDistributable,
		Vst::PlugType::kFxFilter,
		FULL_VERSION_STR,
		kVstVersionString,
		triplef::TripleFProcessor::createInstance)


END_FACTORY