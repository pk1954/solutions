// NNetParameters.cpp
//
// NNetModel

module NNetModel:NNetParameters;

import Debug;
import Types;
import Signals;
import :ParamType;

float NNetParameters::GetParameterValue(ParamType::Value const param) const
{
	SigGenStaticData const& sigGenData { m_pSignalParameters->GetSigGenStaticData() };
	switch (param)
	{
		using enum ParamType::Value;
	case inputPeakTime:  return sigGenData.GetPeakTime().GetValue();
	case inputPeakVolt:  return sigGenData.GetAmplitude().Peak().GetValue();
	case inputPeakFreq:  return sigGenData.GetFrequency().Peak().GetValue();
	case inputBaseFreq:  return sigGenData.GetFrequency().Base().GetValue();
	case inputBaseVolt:  return sigGenData.GetAmplitude().Base().GetValue();
	case pulseFreqMax:   return m_freqMax.GetValue();
	case neuronPeakVolt: return m_neuronPeakVolt.GetValue();
	case threshold:      return m_threshold.GetValue();
	case synapseDelay:   return m_synapseDelay.GetValue();
	case pulseSpeed:	 return m_pulseSpeed.GetValue();
	case pulseWidth:	 return m_pulseWidth.GetValue();
	case pixelScanTime:  return m_usPixelScanTime.GetValue();
	case nrOfScans:      return static_cast<float>(m_iNrOfScans);
	case timeResolution: return TimeResolution().GetValue();
	case scanHorzPixels: return static_cast<float>(ScanSize().m_x);
	case scanVertPixels: return static_cast<float>(ScanSize().m_y);
	case scanResolution: return ScanResolution().GetValue();
	default: Assert(false);
	}
	return 0.f;
}

void NNetParameters::SetParameterValue
(
	ParamType::Value const param,
	float            const fNewValue
)
{
	SigGenStaticData& sigGenData { m_pSignalParameters->GetSigGenStaticData() };
	switch (param)
	{
		using enum ParamType::Value;
	case pulseFreqMax:   m_freqMax = (static_cast<fHertz>(fNewValue));
                         m_usPulseDistMin = PulseDuration(m_freqMax);
                         break;
	case inputPeakTime:  sigGenData.SetPeakTime            (static_cast<fMicroSecs> (fNewValue)); break;
	case inputPeakFreq:  sigGenData.SetFreqPeak            (static_cast<fHertz>     (fNewValue)); break;
	case inputBaseFreq:  sigGenData.SetFreqBase            (static_cast<fHertz>     (fNewValue)); break;
	case inputPeakVolt:  sigGenData.SetAmplPeak            (static_cast<mV>         (fNewValue)); break;
	case inputBaseVolt:  sigGenData.SetAmplBase            (static_cast<mV>         (fNewValue)); break;
	case pulseSpeed:	 m_pulseSpeed      =                static_cast<meterPerSec>(fNewValue);  break;
	case pulseWidth:	 m_pulseWidth      =                static_cast<fMicroSecs> (fNewValue);  break;
	case threshold:      m_threshold       =                static_cast<mV>         (fNewValue);  break;
	case synapseDelay:   m_synapseDelay    =                static_cast<fMicroSecs> (fNewValue);  break;
	case neuronPeakVolt: m_neuronPeakVolt  =                static_cast<mV>         (fNewValue);  break;
	case pixelScanTime:  m_usPixelScanTime =                static_cast<fMicroSecs> (fNewValue);  break;
	case nrOfScans:      m_iNrOfScans      =                Cast2Int                (fNewValue);  break;
	case timeResolution: m_pSignalParameters->SetResolution(static_cast<fMicroSecs> (fNewValue)); break;
	case scanResolution: m_pScanRaster      ->SetResolution(static_cast<MicroMeter> (fNewValue)); break;
	case scanHorzPixels: m_pScanRaster      ->SetWidth     (static_cast<RasterIndex>(fNewValue)); break;
	case scanVertPixels: m_pScanRaster      ->SetHeight    (static_cast<RasterIndex>(fNewValue)); break;
	default: Assert(false);
	}
	NotifyAll();
}
