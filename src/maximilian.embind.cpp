/*
 contains all bindings for use with emscripten
 */
#ifndef Maxi_Emscripten_maxi_embind_h
#define Maxi_Emscripten_maxi_embind_h

#include <emscripten.h>
#include <emscripten/bind.h>
#include "maximilian.h"
// #include "libs/maxiFFT.h"
// #include "libs/maxiGrains.h"
// #include "libs/maxiMFCC.h"
// #include "libs/maxiReverb.h"
#include "libs/maxiSynths.h"
// #include "libs/fft.cpp"



//extern "C" {
//	//	class arrayTest{
//	//		int int_sqrt(int x) {
//	//			return sqrt(x);
//	//		}
//	//	};
//	//
//	//	arrayTest* a(){
//	//		return new arrayTest();
//	//	}
//
//	int sumArray(double* arr, int size){
//		double valOut = 0;
//		for(int i = 0; i < size; i++){
//			valOut += arr[i];
//		}
//		return valOut;
//	}
//}

// int main() {
//   EM_ASM( wasmReady() );
// }

class vectorTools {
public:
	static void clearVectorDbl(vector<double>& vecIn) {
		vecIn.clear();
	}
	static void clearVectorFloat(vector<float>& vecIn) {
		vecIn.clear();
	}

//	static void pr(){
//		EM_ASM_({
//			Module.print('I received: ' + $0);
//		}, 100);
//	}
};

using namespace emscripten;

EMSCRIPTEN_BINDINGS(my_module) {
	register_vector<int>("VectorInt");
	register_vector<double>("VectorDouble");
	register_vector<char>("VectorChar");
	register_vector<float>("VectorFloat");

	class_<vectorTools>("vectorTools")
	.constructor<>()
	.class_function("clearVectorDbl", &vectorTools::clearVectorDbl)
	.class_function("clearVectorFloat", &vectorTools::clearVectorFloat)
//	.class_function("print", &vectorTools::pr)
	;

	// maxi stuff
	class_<maxiSettings>("maxiSettings")
	.constructor<>()
	.class_function("setup", &maxiSettings::setup)
	.property("sampleRate", &maxiSettings::getSampleRate, &maxiSettings::setSampleRate)
	.property("channels", &maxiSettings::getNumChannels, &maxiSettings::setNumChannels)
	.property("bufferSize", &maxiSettings::getBufferSize, &maxiSettings::setBufferSize)
	;

	// MAXI OSC
	class_<maxiOsc>("maxiOsc")
	//	.constructor<>()
	/*
	 Using a smart_ptr_constructor ensures lifetime management  on the js side
	 by returning a smart_ptr when a constructor is used
	 */
	.smart_ptr_constructor("shared_ptr<maxiOsc>",&std::make_shared<maxiOsc>)
	.function("sinewave", &maxiOsc::sinewave)
	.function("coswave", &maxiOsc::coswave)
	.function("phasor", select_overload<double(double)>(&maxiOsc::phasor))
	.function("phasor", select_overload<double(double, double, double)>(&maxiOsc::phasor))
	.function("saw", &maxiOsc::saw)
	.function("triangle", &maxiOsc::triangle)
	.function("square", &maxiOsc::square)
	.function("pulse", &maxiOsc::pulse)
	.function("noise", &maxiOsc::noise)
	.function("sinebuf", &maxiOsc::sinebuf)
	.function("sinebuf4", &maxiOsc::sinebuf4)
	.function("sawn", &maxiOsc::sawn)
	.function("rect", &maxiOsc::rect)
	.function("phaseReset", &maxiOsc::phaseReset)
	;

	// MAXI ENVELOPE
	class_<maxiEnvelope>("maxiEnvelope")
	//	.constructor<>()
	.smart_ptr_constructor("shared_ptr<maxiEnvelope>",&std::make_shared<maxiEnvelope>)

	.function("line", &maxiEnvelope::line)
	//	.function("line", &maxiEnvelope::line, allow_raw_pointers()) // if using array version
	.function("trigger", &maxiEnvelope::trigger)

	.property("amplitude", &maxiEnvelope::getAmplitude, &maxiEnvelope::setAmplitude)
	.property("valindex", &maxiEnvelope::getValindex, &maxiEnvelope::setValindex)

	;

	// MAXI DELAYLINE
	class_<maxiDelayline>("maxiDelayline")
	//	.constructor<>()
	.smart_ptr_constructor("shared_ptr<maxiDelayline>",&std::make_shared<maxiDelayline>)
	.function("dl", select_overload<double(double, int, double)>(&maxiDelayline::dl))
	.function("dl", select_overload<double(double, int, double, int)>(&maxiDelayline::dl))
	;


	// MAXI FILTER
	class_<maxiFilter>("maxiFilter")
	//	.constructor<>()
	.smart_ptr_constructor("shared_ptr<maxiFilter>",&std::make_shared<maxiFilter>)
	.function("lores", &maxiFilter::lores)
	.function("hires", &maxiFilter::hires)
	.function("bandpass", &maxiFilter::bandpass)
	.function("lopass", &maxiFilter::lopass)
	.function("hipass", &maxiFilter::hipass)
	.property("cutoff", &maxiFilter::getCutoff, &maxiFilter::setCutoff)
	.property("resonance", &maxiFilter::getResonance, &maxiFilter::setResonance)
	;


	// MAXI MIX
	class_<maxiMix>("maxiMix")
	//	.constructor<>()
	.smart_ptr_constructor("shared_ptr<maxiMix>",&std::make_shared<maxiMix>)
	.function("stereo", &maxiMix::stereo, allow_raw_pointers())
	.function("quad", &maxiMix::quad,  allow_raw_pointers())
	.function("ambisonic", &maxiMix::ambisonic, allow_raw_pointers())
	;

	//	class_<TemplateClass<int>>("IntTemplateClass")
	//	.constructor<int, int, int>()
	//	.function("getMember", &TemplateClass<int>::getMember)
	//	;

	class_<maxiLagExp<double>>("maxiLagExp")
	//	.constructor<>()
	//	.constructor<double, double>()
	.smart_ptr_constructor("shared_ptr<maxiLagExp<double>>",&std::make_shared<maxiLagExp<double>>, allow_raw_pointers()) // not sure how to override constructors with smart_ptr
	//	.smart_ptr_constructor("shared_ptr<maxiLagExp<double>>",&std::make_shared<maxiLagExp<double>>)

	.function("init", &maxiLagExp<double>::init)
	.function("addSample", &maxiLagExp<double>::addSample)
	.function("value", &maxiLagExp<double>::value)
	.property("alpha", &maxiLagExp<double>::getAlpha, &maxiLagExp<double>::setAlpha)
	.property("alphaReciprocal", &maxiLagExp<double>::getAlphaReciprocal, &maxiLagExp<double>::setAlphaReciprocal)
	.property("val", &maxiLagExp<double>::value, &maxiLagExp<double>::setVal)

	;

	// MAXI SAMPLE
	// class_<maxiSample>("maxiSample")
	// //	.constructor<>()
	// .smart_ptr_constructor("shared_ptr<maxiSample>",&std::make_shared<maxiSample>)
	// //	.property("length", &maxiSample::getLength, &maxiSample::setLength) // no work???
	// .function("getLength", &maxiSample::getLength)
	// //	.function("setSample", &maxiSample::setSample)
	// .function("setSample", select_overload<void(vector<double>&)>(&maxiSample::setSample))
	// .function("setSample", select_overload<void(vector<double>&, int)>(&maxiSample::setSample))
	//
	// //	.function("getSummary", &maxiSample::getSummary)
	// .function("isReady", &maxiSample::isReady)
	//
	// .function("playOnce", select_overload<double()>(&maxiSample::playOnce))
	// .function("playOnce", select_overload<double(double)>(&maxiSample::playOnce))
	//
	// .function("play", select_overload<double()>(&maxiSample::play))
	// .function("play", select_overload<double(double)>(&maxiSample::play))
	// .function("play", select_overload<double(double, double, double)>(&maxiSample::play))
	// .function("play4", &maxiSample::play4)
	//
	// .function("trigger", &maxiSample::trigger)
	// .function("clear", &maxiSample::clear)



	//	.function("normalise", &maxiSample::normalise)
	//	.function("autoTrim", &maxiSample::autoTrim)

	//	.function("load", &maxiSample::load)
	//	.function("read", &maxiSample::read, allow_raw_pointers())
	;

	// MAXI MAP
	class_<maxiMap>("maxiMap")
	//	.constructor<>()
	.smart_ptr_constructor("shared_ptr<maxiMap>",&std::make_shared<maxiMap>)
	.function("linlin", &maxiMap::linlin)
	.function("linexp", &maxiMap::linexp)
	.function("explin", &maxiMap::explin)
	.function("clamp", &maxiMap::clamp<double>)
	;

	// MAXI DYN
	class_<maxiDyn>("maxiDyn")
	//	.constructor<>()
	.smart_ptr_constructor("shared_ptr<maxiDyn>",&std::make_shared<maxiDyn>)
	.function("gate", &maxiDyn::gate)
	.function("compressor", &maxiDyn::compressor)
	.function("compress", &maxiDyn::compress)
	.function("setAttack", &maxiDyn::setAttack)
	.function("setRelease", &maxiDyn::setRelease)
	.function("setThreshold", &maxiDyn::setThreshold)
	.function("setRatio", &maxiDyn::setRatio)
	;

	// MAXI ENV
	class_<maxiEnv>("maxiEnv")
	//	.constructor<>()
	.smart_ptr_constructor("shared_ptr<maxiEnv>",&std::make_shared<maxiEnv>)
	.function("ar", &maxiEnv::ar)
	//	.function("adsr", &maxiEnv::adsr)
	.function("adsr", select_overload<double(double,double,double,double,double,long,int)>(&maxiEnv::adsr))
	.function("adsr", select_overload<double(double,int)>(&maxiEnv::adsr))
	.function("setAttack", &maxiEnv::setAttack)
	.function("setRelease", &maxiEnv::setRelease)
	.function("setDecay", &maxiEnv::setDecay)
	.function("setSustain", &maxiEnv::setSustain)

	.property("trigger", &maxiEnv::getTrigger, &maxiEnv::setTrigger)

	;

	// CONVERT
	class_<convert>("convert")
	//	.constructor<>()
	.smart_ptr_constructor("shared_ptr<convert>",&std::make_shared<convert>)
	.function("mtof", &convert::mtof)
	//	.class_function("mtof", &convert::mtof)
	;


	// MAXI DISTORTION
	class_<maxiDistortion>("maxiDistortion")
	//	.constructor<>()
	.smart_ptr_constructor("shared_ptr<maxiDistortion>",&std::make_shared<maxiDistortion>)
	.function("fastAtan", &maxiDistortion::fastatan)
	.function("atanDist", &maxiDistortion::atanDist)
	.function("fastAtanDist", &maxiDistortion::fastAtanDist)
	;

	// MAXI FLANGER
	class_<maxiFlanger>("maxiFlanger")
	//	.constructor<>()
	.smart_ptr_constructor("shared_ptr<maxiFlanger>",&std::make_shared<maxiFlanger>)
	.function("flange", &maxiFlanger::flange)
	;

	// MAXI CHORUS
	class_<maxiChorus>("maxiChorus")
	//	.constructor<>()
	.smart_ptr_constructor("shared_ptr<maxiChorus>",&std::make_shared<maxiChorus>)
	.function("chorus", &maxiChorus::chorus)
	;

	// MAXI ENVELOPE FOLLOWER
	//	class_<maxiEnvelopeFollowerType<double>>("maxiEnvelopeFollower")
	//	//	.constructor<>()
	//	.smart_ptr_constructor("shared_ptr<maxiEnvelopeFollower>",&std::make_shared<maxiEnvelopeFollowerType<double>>)
	//	.function("setAttack", &maxiEnvelopeFollowerType<double>::setAttack<double>)
	//	.function("setRelease", &maxiEnvelopeFollowerType<double>::setRelease)
	//	.function("play", &maxiEnvelopeFollowerType<double>::play)
	//	.function("reset", &maxiEnvelopeFollowerType<double>::reset)
	//	;

	// MAXI DC BLOCKER
	class_<maxiDCBlocker>("maxiDCBlocker")
	//	.constructor<>()
	.smart_ptr_constructor("shared_ptr<maxiDCBlocker>",&std::make_shared<maxiDCBlocker>)
	.function("play", &maxiDCBlocker::play)
	;

	// MAXI SVF
	class_<maxiSVF>("maxiSVF")
	//	.constructor<>()
	.smart_ptr_constructor("shared_ptr<maxiSVF>",&std::make_shared<maxiSVF>)
	.function("setCutoff", &maxiSVF::setCutoff)
	.function("setResonance", &maxiSVF::setResonance)
	.function("play", &maxiSVF::play)
	;

	// TODO:FB – Uncomment – this is giving me compilation errors on EM
	// MAXI KICK
	// class_<maxiKick>("maxiKick")
	// //	.constructor<>()
	// // .function("setPitch", &maxiKick::setPitch)
	// .smart_ptr_constructor("shared_ptr<maxiKick>",&std::make_shared<maxiKick>)
	// .function("play", &maxiKick::play)
	// .function("setRelease", &maxiKick::setRelease)
	// .function("trigger", &maxiKick::trigger)
	// .property("pitch", &maxiKick::getPitch, &maxiKick::setPitch)
	// .property("distortion", &maxiKick::getDistortion, &maxiKick::setDistortion)
	// .property("cutoff", &maxiKick::getCutoff, &maxiKick::setCutoff)
	// .property("resonance", &maxiKick::getResonance, &maxiKick::setResonance)
	// .property("useDistortion", &maxiKick::getUseDistortion, &maxiKick::setUseDistortion)
	// .property("useLimiter", &maxiKick::getUseLimiter, &maxiKick::setUseLimiter)
	// .property("useFilter", &maxiKick::getUseFilter, &maxiKick::setUseFilter)
	// ;

	// MAXI SNARE
	// class_<maxiSnare>("maxiSnare")
	// //	.constructor<>()
	// .smart_ptr_constructor("shared_ptr<maxiSnare>",&std::make_shared<maxiSnare>)
	// .function("play", &maxiSnare::play)
	// .function("setPitch", &maxiSnare::setPitch)
	// .function("setRelease", &maxiSnare::setRelease)
	// .function("trigger", &maxiSnare::trigger)
	// ;

	// MAXI HATS
	// class_<maxiHats>("maxiHats")
	// //	.constructor<>()
	// .smart_ptr_constructor("shared_ptr<maxiHats>",&std::make_shared<maxiHats>)
	// .function("play", &maxiHats::play)
	// .function("setPitch", &maxiHats::setPitch)
	// .function("setRelease", &maxiHats::setRelease)
	// .function("trigger", &maxiHats::trigger)
	// ;


	// TODO:FB – Uncomment – this is giving me compilation errors on EM
	// MAXI CLOCK
	// class_<maxiClock>("maxiClock")
	// //	.constructor<>()
	// .smart_ptr_constructor("shared_ptr<maxiClock>",&std::make_shared<maxiClock>)
	// .function("ticker", &maxiClock::ticker)
	// .function("setTempo", &maxiClock::setTempo)
	// .function("setTicksPerBeat", &maxiClock::setTicksPerBeat)
	// .function("isTick", &maxiClock::isTick)
	// .property("currentCount", &maxiClock::getCurrentCount, &maxiClock::setCurrentCount)
	// .property("currentCount", &maxiClock::getLastCount, &maxiClock::setLastCount)
	// .property("playHead", &maxiClock::getPlayHead, &maxiClock::setPlayHead)
	// .property("bps", &maxiClock::getBps, &maxiClock::setBps)
	// .property("bpm", &maxiClock::getBpm, &maxiClock::setBpm)
	// .property("tick", &maxiClock::getTick, &maxiClock::setTick)
	// .property("ticks", &maxiClock::getTicks, &maxiClock::setTicks)
	// ;

  //	class_<testVectorHolder>("testVectorHolder")
	//	.constructor<>()
	/*
	 Using a smart_ptr_constructor ensures lifetime management  on the js side
	 by returning a smart_ptr when a constructor is used
	 */
	//	.smart_ptr_constructor("shared_ptr<testVectorHolder>",&std::make_shared<testVectorHolder>)
	//	.function("at", &testVectorHolder::at)
	//	.function("coswave", &maxiOsc::coswave)
	//	;

	// -----------------------------------------

	//	class_<maxiTest>("maxiTest")
	//	.constructor<>()
	/*
	 Using a smart_ptr_constructor ensures lifetime management  on the js side
	 by returning a smart_ptr when a constructor is used
	 */
	//	.smart_ptr_constructor("shared_ptr<maxiOsc>",&std::make_shared<maxiTest>)
	//	.function("sumArray", &maxiTest::sumArray, allow_raw_pointers())
	//	;
};
//
// EMSCRIPTEN_BINDINGS(my_module_maxiMFCC) {
//
//     // -------------------------------------------------------------------------------------------
//     // LIBS
//     // MAXI MFCC
//     class_<maxiMFCC>("maxiMFCC")
//     //    .constructor<>()
//     //    .constructor<int>()
//
//     .smart_ptr_constructor("shared_ptr<maxiMFCC>",&std::make_shared<maxiMFCC>)
//     .function("setup", &maxiMFCC::setup)
//     .function("mfcc", &maxiMFCC::mfcc, allow_raw_pointers())
//     ;
// };
//
// EMSCRIPTEN_BINDINGS(my_module_maxiGrains) {
//
//     // -------------------------------------------------------------------------------------------
//     // LIBS
//
//
//     // MAXI TIMESTRETCH
//     class_<maxiTimestretch<hannWinFunctor> >("maxiTimestretch")
//     .smart_ptr_constructor("shared_ptr<maxiTimestretch<hannWinFunctor> >",&std::make_shared<maxiTimestretch<hannWinFunctor> >)
//     //    .smart_ptr_constructor<maxiSample*>("shared_ptr<maxiTimestretch<hannWinFunctor> >",&std::make_shared<maxiTimestretch<hannWinFunctor> >)
//     .function("setSample", &maxiTimestretch<hannWinFunctor>::setSample, allow_raw_pointers())
//
//     .function("getNormalisedPosition", &maxiTimestretch<hannWinFunctor>::getNormalisedPosition)
//     .function("getPosition", &maxiTimestretch<hannWinFunctor>::getPosition)
//     .function("setPosition", &maxiTimestretch<hannWinFunctor>::setPosition)
//
//     .function("play", &maxiTimestretch<hannWinFunctor>::play)
//     .function("play2", &maxiTimestretch<hannWinFunctor>::play2)
//     ;
//
//     // MAXI PITCHSHIFT
//
//     class_<maxiPitchShift<hannWinFunctor> >("maxiPitchShift")
//     .smart_ptr_constructor("shared_ptr<maxiPitchShift<hannWinFunctor> >",&std::make_shared<maxiPitchShift<hannWinFunctor> >)
//     .function("setSample", &maxiPitchShift<hannWinFunctor>::setSample, allow_raw_pointers())
//
//     .function("play", &maxiPitchShift<hannWinFunctor>::play)
//     ;
//
//
//     // MAXI PITCHSTRETCH
//     class_<maxiPitchStretch<hannWinFunctor> >("maxiPitchStretch")
//     .smart_ptr_constructor("shared_ptr<maxiTimestretch<hannWinFunctor> >",&std::make_shared<maxiPitchStretch<hannWinFunctor> >)
//     //    .smart_ptr_constructor<maxiSample*>("shared_ptr<maxiTimestretch<hannWinFunctor> >",&std::make_shared<maxiTimestretch<hannWinFunctor> >)
//     .function("setSample", &maxiPitchStretch<hannWinFunctor>::setSample, allow_raw_pointers())
//
//     .function("getNormalisedPosition", &maxiPitchStretch<hannWinFunctor>::getNormalisedPosition)
//     .function("getPosition", &maxiPitchStretch<hannWinFunctor>::getPosition)
//     .function("setPosition", &maxiPitchStretch<hannWinFunctor>::setPosition)
//
//     .function("setLoopStart", &maxiPitchStretch<hannWinFunctor>::setLoopStart)
//     .function("setLoopEnd", &maxiPitchStretch<hannWinFunctor>::setLoopEnd)
//
//     .function("play", &maxiPitchStretch<hannWinFunctor>::play)
//     ;
// };
//
// EMSCRIPTEN_BINDINGS(my_module_maxiFFT) {
//
//     // -------------------------------------------------------------------------------------------
//     // LIBS
//
//
//     // MAXI FFT
//     class_<maxiFFT>("maxiFFT")
//     //    .constructor<>()
//     //    .constructor<int>()
//
//
//     .smart_ptr_constructor("shared_ptr<maxiFFT>",&std::make_shared<maxiFFT>)
//     .function("setup", &maxiFFT::setup)
//     .function("process", &maxiFFT::process)
//     .function("magsToDB", &maxiFFT::magsToDB)
//     .function("spectralFlatness", &maxiFFT::spectralFlatness)
//     .function("spectralCentroid", &maxiFFT::spectralCentroid)
//     .function("getMagnitude", &maxiFFT::getMagnitude)
//     .function("getMagnitudeDB", &maxiFFT::getMagnitudeDB)
//     .function("getPhase", &maxiFFT::getPhase)
//
//     .property("windowSize", &maxiFFT::getWindowSize, &maxiFFT::setWindowSize)
//     .property("hopSize", &maxiFFT::getHopSize, &maxiFFT::setHopSize)
//     .property("bins", &maxiFFT::getNumBins, &maxiFFT::setNumBins)
//     .property("magnitudes", &maxiFFT::getMagnitudes, &maxiFFT::setMagnitudes)
//     .property("phases", &maxiFFT::getPhases, &maxiFFT::setPhases)
//
//     ;
//
//     // MAXI IFFT
//     class_<maxiIFFT>("maxiIFFT")
//     //    .constructor<>()
//     //    .constructor<int>()
//
//     .smart_ptr_constructor("shared_ptr<maxiIFFT>",&std::make_shared<maxiIFFT>)
//     .function("setup", &maxiIFFT::setup)
//     .function("process", &maxiIFFT::process)
//
//     ;
//
//     // MAXI IFFT
//     class_<maxiFFTOctaveAnalyzer>("maxiFFTOctaveAnalyzer")
//     //    .constructor<>()
//     //    .constructor<int>()
//
//     .smart_ptr_constructor("shared_ptr<maxiFFTOctaveAnalyzer>",&std::make_shared<maxiFFTOctaveAnalyzer>)
//     .function("setup", &maxiFFTOctaveAnalyzer::setup)
//     .function("calculate", &maxiFFTOctaveAnalyzer::calculate)
//
//     //properties
//     .property("samplingRate", &maxiFFTOctaveAnalyzer::getSamplingRate, &maxiFFTOctaveAnalyzer::setSamplingRate)
//     .property("nSpectrum", &maxiFFTOctaveAnalyzer::getNSpectrum, &maxiFFTOctaveAnalyzer::setNSpectrum)
//     .property("nAverages", &maxiFFTOctaveAnalyzer::getNAverages, &maxiFFTOctaveAnalyzer::setNAverages)
//     .property("nAveragesPerOctave", &maxiFFTOctaveAnalyzer::getNAveragesPerOct, &maxiFFTOctaveAnalyzer::setNAveragesPerOct)
//     .property("spectrumFrequencySpan", &maxiFFTOctaveAnalyzer::getSpecFreqSpan, &maxiFFTOctaveAnalyzer::setSpecFreqSpan)
//     .property("firstOctaveFrequency", &maxiFFTOctaveAnalyzer::getFirstOctFreq, &maxiFFTOctaveAnalyzer::setFirstOctFreq)
//     .property("averageFrequencyIncrement", &maxiFFTOctaveAnalyzer::getAvgFreqIncr, &maxiFFTOctaveAnalyzer::setAvgFreqIncr)
//
//
//     .function("getAverage", &maxiFFTOctaveAnalyzer::getAverage)
//     .function("getPeak", &maxiFFTOctaveAnalyzer::getPeak)
//     .function("getPeakHoldTime", &maxiFFTOctaveAnalyzer::getPeakHoldTime)
//
//     .property("peakHoldTime", &maxiFFTOctaveAnalyzer::getPeakHoldTimeTotal, &maxiFFTOctaveAnalyzer::setPeakHoldTimeTotal)
//     .property("peakDecayRate", &maxiFFTOctaveAnalyzer::getPeakDecayRate, &maxiFFTOctaveAnalyzer::setPeakDecayRate)
//
//     .function("getSpe2Avg", &maxiFFTOctaveAnalyzer::getSpe2Avg)
//
//     .property("linearEQSlope", &maxiFFTOctaveAnalyzer::getLinEQSlope, &maxiFFTOctaveAnalyzer::setLinEQSlope)
//     .property("linearEQIntercept", &maxiFFTOctaveAnalyzer::getLinEQIntercept, &maxiFFTOctaveAnalyzer::setLinEQIntercept)
//     ;
//
// };

#endif